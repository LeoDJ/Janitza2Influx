#include <Arduino.h>
#include "globals.h"
#include "janitza.h"
#include "janitzaRegDefs/UMG96RM.h"
#include "eth.h"
#include "ArduinoJson.h"
#include "mqtt.h"

Janitza janitza;
EthernetClient client;

JsonDocument doc;

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
            DBG.println("InfluxDB request successful.");
        }
        else {
            DBG.println("InfluxDB request failed!");
            DBG.write(buf, len);
        }
    }
}

void sendInfluxRequest(char *lineProtocolCommand) {
    if (!client.connected()) {
        if (!client.connect(INFLUX_HOST, INFLUX_PORT)) {
            DBG.printf("Connection to InfluxDB failed\n");
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

    DBG.begin(115200);
    DBG.println("\nJanitza UMG96RM Power Analyzer to Influx\n");
    MODUBS_SERIAL.begin(MODBUS_BAUD);

    initEthernet();
    connectEthernet();
    mqttInit();

    janitza.setDebugSerial(DBG);
    janitza.useRS485(MODBUS_DE_PIN, MODBUS_RE_PIN, preTransmission, postTransmission);
    #if USE_INFLUXDB
    janitza.setInfluxSendCallback(sendInfluxRequest, INFLUX_MEASUREMENT);
    #endif
    janitza.init(MODUBS_SERIAL, MODBUS_ADDR, regDef_UMG96RM, sizeof(regDef_UMG96RM));  //blocks until modbus connected


    // janitza.read();

}

uint32_t lastUpdate = 0;

void loop () {
    if (millis() - lastUpdate >= UPDATE_INTERVAL) {
        lastUpdate = millis();

        #if USE_INFLUXDB
        janitza.readAndSendToInflux();
        #endif

        #if USE_MQTT
        if (janitza.read()) {
            doc = janitza.generateJson();
            uint8_t buf[4096];
            size_t len = serializeJson(doc, buf);
            String topic = MQTT_BASE_TOPIC;
            topic += String(janitza.getSerialNumber());
            topic += "/json";
            mqttPublish(topic.c_str(), buf, len);
        }
        #endif
    }
   
    handleHttpResponse();
    mqttLoop();
}