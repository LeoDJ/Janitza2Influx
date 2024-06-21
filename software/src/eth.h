#pragma once
#ifdef ARDUINO_ARCH_STM32
#include <SPI.h>
#include <Ethernet.h>
#endif
#ifdef ARDUINO_ARCH_CH32V
#include <CH32Ethernet.h>
#include <CH32EthernetClient.h>
#endif

#include "globals.h"

extern uint8_t *macAddr;
extern char macStr[13];
extern EthernetClient net;

void initEthernet();
bool connectEthernet();