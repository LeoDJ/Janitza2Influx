#include <Arduino.h>
#include <Ethernet.h>

#include "globals.h"
#include "janitza.h"
#include "janitzaRegDefs/UMG96RM.h"
#include "eth.h"

Janitza janitza;
EthernetClient client;

// workaround, because of function pointer weirdness
void preTransmission() {
    janitza.rs485Transmit();
}

void postTransmission() {
    janitza.rs485Receive();
}

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

void setup() {
    Serial.begin(115200);
    MODUBS_SERIAL.begin(MODBUS_BAUD);
    janitza.setDebugSerial(Serial);
    janitza.useRS485(MODBUS_DE_PIN, preTransmission, postTransmission);
    janitza.setInfluxSendCallback(sendInfluxRequest, INFLUX_MEASUREMENT);
    janitza.init(MODUBS_SERIAL, MODBUS_ADDR, regDef_UMG96RM, sizeof(regDef_UMG96RM));

    initEthernet();
    connectEthernet();
}

uint32_t lastUpdate = 0;

void loop () {
        if (millis() - lastUpdate >= UPDATE_INTERVAL) {
        lastUpdate = millis();
        janitza.readAndSendToInflux();
    }
   
    handleHttpResponse();
}