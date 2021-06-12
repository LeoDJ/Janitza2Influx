#pragma once
#include <stdint.h>

#include "secrets.h"

#define DEBUG           Serial1

#define UPDATE_INTERVAL 5000    // ms

#define CT_RATIO        (40/5)  // calculated current transformer ratio (e.g. 40:5 -> 8)
#define MODUBS_SERIAL   Serial2
#define MODBUS_BAUD     38400
#define MODBUS_ADDR     1

#define SPI2_MOSI   PB15
#define SPI2_MISO   PB14
#define SPI2_SCK    PB13
#define SPI2_CS     PB12

#define SPI1_MOSI   PA7
#define SPI1_MISO   PA6
#define SPI1_SCK    PA5
#define SPI1_CS     PA4