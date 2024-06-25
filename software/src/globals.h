#pragma once
#include <stdint.h>

#include "secrets.h"    // don't forget to create secrets.h based on the .sample file


#define USE_INFLUXDB    0
#define USE_MQTT        1

#define DBG             Serial1

#define UPDATE_INTERVAL 5000    // ms

#ifdef ARDUINO_ARCH_STM32
    #define MODUBS_SERIAL   Serial3
    #define MODBUS_DE_PIN   PB1
    #define MODBUS_RE_PIN   MODBUS_DE_PIN
#endif
#ifdef ARDUINO_ARCH_CH32V
    // #define MODUBS_SERIAL   Serial1  // RS485 1
    // #define MODBUS_DE_PIN   PA8
    // #define MODBUS_RE_PIN   PA15
    // #define MODUBS_SERIAL   Serial2  // RS485 2
    // #define MODBUS_DE_PIN   PB2
    // #define MODBUS_RE_PIN   PD5
    #define MODUBS_SERIAL   Serial3 // RS485 0
    #define MODBUS_DE_PIN   PB2
    #define MODBUS_RE_PIN   PD5
#endif

#define MODBUS_BAUD     115200
#define MODBUS_ADDR     1

// put InfluxDB credentials into secrets.h

#define SPI2_MOSI   PB15
#define SPI2_MISO   PB14
#define SPI2_SCK    PB13
#define SPI2_CS     PB12

#define SPI1_MOSI   PA7
#define SPI1_MISO   PA6
#define SPI1_SCK    PA5
#define SPI1_CS     PA4