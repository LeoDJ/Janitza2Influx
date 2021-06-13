#include <Arduino.h>
#include <ModbusMaster.h>
#include <Ethernet.h>

#include "globals.h"
#include "janitza.h"
#include "eth.h"



ModbusMaster mb;
uint32_t _serialNumber;
EthernetClient client;

void handleHttpResponse() {
    // print response from HTTP request
    if (client.available()) {
        int len = client.available();
        uint8_t buf[256];
        if (len > 256) {
            len = 256;
        }
        client.read(buf, len);

        DEBUG.write(buf, len);
    }
}

void sendInfluxRequest(char *lineProtocolCommand) {
    if (!client.connected()) {
        if (!client.connect(INFLUX_HOST, INFLUX_PORT)) {
            DEBUG.printf("Connection to InfluxDB failed\n");
            return;
        }
    }

    client.printf("POST /write?db=%s HTTP/1.1\n", INFLUX_DB);
    client.printf("Host: %s\n", INFLUX_HOST);
    client.printf("Authorization: Basic %s\n", INFLUX_AUTH_BASE64);
    client.printf("Content-Length: %d\n", strlen(lineProtocolCommand));
    client.printf("Connection: close\n");
    client.printf("\n");
    client.write(lineProtocolCommand);
    client.printf("\n");
    client.flush();
    while (client.connected()) {
        handleHttpResponse();
    }
}

uint32_t readSerialNumber() {
    uint8_t ret = mb.readHoldingRegisters(REG_SERIAL, 2);
    if (ret != mb.ku8MBSuccess) {
        DEBUG.printf("Error reading from ModBus. Status = %02X\n", ret);
        return 0;
    }
    return mb.getResponseBuffer(0) << 16 | mb.getResponseBuffer(1);
}

registerDefinition_t *getRegDef(uint16_t address) {
    // currently the definition is still continuous, so we don't actually have to search through it
    if (address >= REG_DEF_START_ADDR && address <= REG_DEF_START_ADDR + REG_DEF_NUM) {
        return &registerDefinition[address - REG_DEF_START_ADDR];
    }
    else {
        return nullptr;
    }
}

#define MB_CHUNK_SIZE   64

bool modbusReadBulk(int16_t *destBuf, uint16_t startAddr, uint16_t count) {
    int iterations = (count + (MB_CHUNK_SIZE - 1)) / MB_CHUNK_SIZE; // division, but rounded up
    for (int i = 0; i < iterations; i++) {
        uint16_t mbStart = startAddr + i * MB_CHUNK_SIZE;
        uint8_t mbCount = MB_CHUNK_SIZE;
        if (MB_CHUNK_SIZE * (i + 1) > count) {
            mbCount = count % MB_CHUNK_SIZE;
        }

        DEBUG.printf("Reading Modbus Addr: %d, Count: %d\n", mbStart, mbCount);

        uint8_t ret = mb.readHoldingRegisters(mbStart, mbCount);

        if (ret != mb.ku8MBSuccess) {
            DEBUG.printf("Error reading from ModBus. Status = %02X\n", ret);
            return false;
        }

        for (int j = 0; j < mbCount; j++) {
            destBuf[i * MB_CHUNK_SIZE + j] = mb.getResponseBuffer(j);
        }
    }
    return true;

}

int16_t modbusBuf[REG_DEF_NUM];

void readMeter() {
    // readMeterChunk(200, 64);
    bool success = modbusReadBulk(modbusBuf, 200, 226);

    if (!success) {
        DEBUG.printf("Error reading bulk from ModBus\n");
        return;
    }

    char influxQueryBuf[512];

    for (int phase = 0; phase < P_TAG_NUM; phase++) {
        const char* phaseStr = phaseTagStr[phase];

        int influxQueryLen = 0;
        influxQueryLen += sprintf(influxQueryBuf + influxQueryLen, "%s", INFLUX_MEASUREMENT); // measurement name
        influxQueryLen += sprintf(influxQueryBuf + influxQueryLen, ",serial=%d,phase=%s ", _serialNumber, phaseStr); // print tags here
    

        for (int i = 0; i < REG_DEF_NUM; i++) {
            registerDefinition_t *regDef = &registerDefinition[i];

            if (regDef->address == 0) { // skip "disabled" addresses
                continue;
            }

            if (regDef->phaseTag == phase) {
                int32_t mbValue = modbusBuf[i];
                if (regDef->type == LONG) {
                    mbValue = mbValue << 16 | (modbusBuf[i + 1] & 0xFFFF);
                    i++;    // increment counter / skip next address
                }

                float val = (float)mbValue / (float)regDef->multiplier;

                // apply current transformer ratio if needed
                if (regDef->applyCtRatio) {
                    val *= (float)CT_RATIO;
                }

                influxQueryLen += sprintf(influxQueryBuf + influxQueryLen, "%s=%g,", regDef->influxStr, val);
            }
        }

        influxQueryBuf[influxQueryLen - 1] = 0; // clear trailing comma

        DEBUG.write(influxQueryBuf);
        DEBUG.println();
        
        sendInfluxRequest(influxQueryBuf);
        handleHttpResponse();
    }

}

void setup() {
    DEBUG.begin(115200);
    DEBUG.println("\nJanitza Power Meter to Influx\n");

    MODUBS_SERIAL.begin(MODBUS_BAUD);
    mb.begin(MODBUS_ADDR, MODUBS_SERIAL);

    _serialNumber = readSerialNumber();
    DEBUG.printf("Serial: %d\n", _serialNumber);
    // readMeter();

    initEthernet();
    connectEthernet();

    readMeter();
}

uint32_t lastUpdate = 0;

void loop() {
    if (millis() - lastUpdate >= UPDATE_INTERVAL) {
        lastUpdate = millis();
        readMeter();
    }
   
}