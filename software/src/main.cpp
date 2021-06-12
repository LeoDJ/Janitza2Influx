#include <Arduino.h>
#include <ModbusMaster.h>
#include <Ethernet.h>

#include "globals.h"
#include "janitza.h"
#include "eth.h"



ModbusMaster mb;
uint32_t _serialNumber;
EthernetClient client;

void sendInfluxRequest(char *lineProtocolCommand) {
    if (client.connect(INFLUX_HOST, INFLUX_PORT)) {
        client.printf("POST /write?db=%s HTTP/1.1\n", INFLUX_DB);
        client.printf("Host: %s\n", INFLUX_HOST);
        client.printf("Authorization: Basic %s\n", INFLUX_AUTH_BASE64);
        client.printf("Content-Length: %d\n", strlen(lineProtocolCommand));
        client.printf("Connection: close\n");
        client.printf("\n");
        client.write(lineProtocolCommand);
        client.printf("\n");
    }
    else {
        DEBUG.printf("Connection to InfluxDB failed\n");
    }

}

uint32_t readSerialNumber() {
    uint8_t ret = mb.readInputRegisters(REG_SERIAL, 2);
    if (ret != mb.ku8MBSuccess) {
        DEBUG.printf("Error reading from ModBus. Status = %02X\n", ret);
        return 0;
    }
    // TODO: check endianness
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

void readMeterChunk(uint16_t startAddr, uint8_t count) {
    uint8_t ret = mb.readInputRegisters(startAddr, count);

    if (ret != mb.ku8MBSuccess) {
        DEBUG.printf("Error reading from ModBus. Status = %02X\n", ret);
        return;
    }

    char influxQueryBuf[2048];
    int influxQueryLen = 0;
    influxQueryLen += sprintf(influxQueryBuf + influxQueryLen, "%s", INFLUX_MEASUREMENT); // measurement name
    influxQueryLen += sprintf(influxQueryBuf + influxQueryLen, ",serial=%d ", _serialNumber); // print tags here
    
    for (int i = 0; i < count; i++) {
        uint16_t addr = startAddr + i;
        int32_t mbValue = (int16_t)mb.getResponseBuffer(i); // get value and handle as signed integer
        registerDefinition_t *regDef = getRegDef(addr);
        

        if (regDef) {
            if (regDef->type == LONG) {
                // TODO: check endianness
                uint16_t lowWord = mb.getResponseBuffer(i + 1);
                mbValue = mbValue << 16 | lowWord;
                i++;    // increment counter / skip next address
            }

            float val = (float)mbValue / (float)regDef->multiplier;

            // apply current transformer ratio if needed
            if (regDef->applyCtRatio) {
                val *= (float)CT_RATIO;
            }

            // TODO: potentially need to enable printf float support
            printf("%3d %s = %f\n", addr, regDef->influxStr, val);
            influxQueryLen += sprintf(influxQueryBuf + influxQueryLen, "%s=%f,", regDef->influxStr, val);
        }
        else {
            // will be just ignored later on
            printf("%3d unknown = %d\n", addr, mbValue);
        }
    }

    influxQueryBuf[influxQueryLen - 1] = 0; // clear trailing comma

    DEBUG.write(influxQueryBuf);

}

void readMeter() {
    readMeterChunk(200, 64);
}

void setup() {
    DEBUG.begin(115200);
    DEBUG.println("\nJanitza Power Meter to Influx\n");

    MODUBS_SERIAL.begin(MODBUS_BAUD);
    mb.begin(MODBUS_ADDR, MODUBS_SERIAL);

    _serialNumber = readSerialNumber();
    DEBUG.printf("Serial: %d\n", _serialNumber);
    readMeter();

    initEthernet();
    connectEthernet();
}

uint32_t lastUpdate = 0;

void loop() {
    if (millis() - lastUpdate >= UPDATE_INTERVAL) {
        lastUpdate = UPDATE_INTERVAL;
        // readMeter();
    }

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