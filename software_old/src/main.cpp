#include <Arduino.h>
#include <ModbusMaster.h>
#include <Ethernet.h>

#include "globals.h"
#include "janitza.h"
#include "eth.h"


ModbusMaster mb;
uint32_t _serialNumber;
uint16_t _ctRatio = 1;
EthernetClient client;


void handleHttpResponse() {
    // print response from HTTP request
    if (client.available()) {
        int len = client.available();
        uint8_t buf[512];
        if (len > 512) {
            len = 512;
        }
        client.read(buf, len);

        if (strncmp((const char *)buf, "HTTP/1.1 204", 12) == 0) {
            DEBUG.println("InfluxDB request successful.");
        }
        else {
            DEBUG.println("InfluxDB request failed!");
            DEBUG.write(buf, len);
        }
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
        DEBUG.printf("Error reading from ModBus in readSerialNumber. Status = %02X\n", ret);
        return 0;
    }
    return mb.getResponseBuffer(0) << 16 | mb.getResponseBuffer(1);
}

uint16_t readCtRatio() {
    uint8_t ret = mb.readHoldingRegisters(REG_CT_PRIM, 2);
    if (ret != mb.ku8MBSuccess) {
        DEBUG.printf("Error reading from ModBus in readCtRatio. Status = %02X\n", ret);
        return 0;
    }
    // calculate CT ratio, only integers supported currently
    return mb.getResponseBuffer(0) / mb.getResponseBuffer(1);
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

#define MB_CHUNK_SIZE   32

bool modbusReadBulk(int16_t *destBuf, uint16_t startAddr, uint16_t count) {
    int iterations = (count + (MB_CHUNK_SIZE - 1)) / MB_CHUNK_SIZE; // division, but rounded up
    for (int i = 0; i < iterations; i++) {
        uint16_t mbStart = startAddr + i * MB_CHUNK_SIZE;
        uint8_t mbCount = MB_CHUNK_SIZE;
        if (MB_CHUNK_SIZE * (i + 1) > count) {
            mbCount = count % MB_CHUNK_SIZE;
        }

        DEBUG.printf("Reading Modbus %d/%d (Addr: %d, Count: %d)         \r", i+1, iterations, mbStart, mbCount);

        uint8_t ret = mb.readHoldingRegisters(mbStart, mbCount);

        if (ret != mb.ku8MBSuccess) {
            DEBUG.printf("\nError reading from ModBus. Status = %02X\n", ret);
            return false;
        }

        for (int j = 0; j < mbCount; j++) {
            destBuf[i * MB_CHUNK_SIZE + j] = mb.getResponseBuffer(j);
        }
    }
    DEBUG.printf("\n");
    return true;

}

int16_t modbusBuf[REG_DEF_NUM];
char influxQueryBuf[512*10];

void readMeter() {
    bool success = modbusReadBulk(modbusBuf, 200, 226);

    if (!success) {
        DEBUG.printf("Error reading bulk from ModBus\n");
        return;
    }

    int influxQueryLen = 0;

    for (int phase = 0; phase < P_TAG_NUM; phase++) {
        const char* phaseStr = phaseTagStr[phase];

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
                    val *= (float)_ctRatio;
                }

                influxQueryLen += sprintf(influxQueryBuf + influxQueryLen, "%s=%g,", regDef->influxStr, val);
            }
        }

        influxQueryBuf[influxQueryLen - 1] = '\n'; // replace trailing comma with newline
    }
    // DEBUG.write(influxQueryBuf);
    DEBUG.print("Sending InfluxDB Request... ");
    sendInfluxRequest(influxQueryBuf);

}

void setup() {
    DEBUG.begin(115200);
    DEBUG.println("\nJanitza Power Meter to Influx\n");

    MODUBS_SERIAL.begin(MODBUS_BAUD);
    mb.begin(MODBUS_ADDR, MODUBS_SERIAL);

    while (_serialNumber == 0) {
        _serialNumber = readSerialNumber();
        if (_serialNumber != 0) {
            DEBUG.printf("Serial: %d\n", _serialNumber);
        }
        else {
            DEBUG.printf("Error: Could not read serial number, make sure the meter is connected and address and baud rate are correct. Retrying in 5s...\n");
            delay(5000);
        }
    }

    _ctRatio = readCtRatio();
    DEBUG.printf("CT Ratio: %d\n", _ctRatio);

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
   
    handleHttpResponse();
}