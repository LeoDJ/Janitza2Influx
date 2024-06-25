
#include <PubSubClient.h>
#include "eth.h"

EthernetClient ethClient;
PubSubClient mqttClient(ethClient);
uint32_t mqttLastReconnect = 0;

bool mqttPublish(const char* topic, const uint8_t* payload, size_t plength, bool retained = false) {
        DBG.printf("MQTT publishing %s [%d]: ", topic, plength);
        DBG.write(payload, plength);
        DBG.printf("\n");
    if (mqttClient.connected()) {
        bool success = mqttClient.publish(topic, payload, plength, retained);
        if (!success) {
            DBG.println("MQTT publish error");
        }
        return success;
    }
    DBG.println("MQTT publish error: Not connected");
    return false;
}

bool mqttPublish(const char* topic, const uint8_t* payload, bool retained = false) {
    if (mqttClient.connected()) {
        return mqttClient.publish(topic, payload, retained);
    }
    return false;
}

void mqttCallback(char* topic, byte* payload, unsigned int length) {
    // handle message arrived
    DBG.printf("MQTT /%s: ", topic);
    DBG.write(payload, length);
    DBG.println();
}

bool mqttReconnect() {
    DBG.print("MQTT connecting... ");
    if (mqttClient.connect(HOSTNAME, MQTT_USER, MQTT_PASSWORD)) {
        // mqttClient.publish("test", "hello world");
        // mqttClient.subscribe("test");
        DBG.println("success!");
    }
    else {
        DBG.println("failed.");
    }
    return mqttClient.connected();
}

void mqttLoop() {
    #if USE_MQTT
    if (!mqttClient.connected()) {
        if (millis() - mqttLastReconnect > 5000) {
            mqttLastReconnect = millis();
            mqttReconnect();
        }
    }
    mqttClient.loop();
    #endif
}

void mqttInit() {
    DBG.println("MQTT init");
    mqttClient.setServer(MQTT_HOST, MQTT_PORT);
    mqttClient.setCallback(mqttCallback);
    mqttClient.setBufferSize(4096);
    mqttReconnect();
}