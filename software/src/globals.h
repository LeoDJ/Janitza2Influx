#pragma once
#include <stdint.h>

#include "secrets.h"    // don't forget to create secrets.h based on the .sample file

#define DEBUG           Serial1

#define UPDATE_INTERVAL 5000    // ms

#define MODUBS_SERIAL   Serial3
#define MODBUS_BAUD     38400
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