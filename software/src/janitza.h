#pragma once

#include <Arduino.h>
#include <ModbusMaster.h>
#include <ArduinoJson.h>

// workaround needed because of void pointer debacle
// #include "globals.h"
// #define _rs485deRePin MODBUS_DE_PIN

typedef struct {
    uint16_t address;
    uint8_t multiplier;
    uint8_t applyCtRatio : 1;
    uint8_t type : 3;
    uint8_t phaseTag : 3;
    const char *nameStr;
    int8_t index;
} registerDefinition_t;


class Janitza {
    public:
        enum registerDataTypes {
            CHAR,   // uint8
            BYTE,   // int8
            SHORT,  // int16
            INT,    // int32 (on UMG96S INT = int16)
            UINT,   // uint32
            LONG,   // int64
            FLOAT,  // float32
            DOUBLE, // double64
        };

        const uint8_t registerDataTypeSize[DOUBLE+1] {
            2,  // char (technically 1 byte long, but modbus always reads 2)
            2,  // byte (technically 1 byte long, but modbus always reads 2)
            2,  // short
            4,  // int
            4,  // uint
            8,  // long
            4,  // float
            8   // double
        };

        enum phaseTag {
            P_ALL,
            P_L1,
            P_L2,
            P_L3,
            P_L1L2,
            P_L2L3,
            P_L3L1,
            P_NONE,     // used when it's not phase-relevant
            P_TAG_NUM   // length of enum
        };

        const char *phaseTagStr[P_TAG_NUM] = {
            "All",
            "L1",
            "L2",
            "L3",
            "L1-L2",
            "L2-L3",
            "L3-L1",
            "All",      // shouldn't be used
        };


        // will block until modbus read is successful
        void init(Stream &serialPort, uint16_t modbusAddress, registerDefinition_t *registerDef, size_t regDefSize);
        
        void setDebugSerial(Stream &serialPort);
        void setInfluxSendCallback(void (*influxSendRequest)(char *lineProtocolCommand), const char *measurementName);
        void useRS485(uint32_t dePin, uint32_t rePin, void (*preTransmission)(), void (*postTransmission)());         // supply pin for RS485 transceiver direction (DE / RE)
        uint32_t readSerialNumber();
        uint32_t getSerialNumber();
        bool read();
        JsonDocument generateJson();
        bool generateInfluxCommands();
        void readAndSendToInflux();

        // only public because of function pointer workaround
        void rs485Transmit();
        void rs485Receive();

    protected:
        bool modbusReadBulk(int16_t *destBuf, uint16_t startAddr, uint16_t count);
        uint16_t getRequiredModbusBufferSize(uint16_t regStart, uint16_t regEnd);
        uint16_t getContiguousRegisters(uint16_t startIndex);
        float getValue(registerDefinition_t *regDef, uint16_t mbBufIdx);

        Stream *_debug = nullptr;       // Debug serial port
        Stream *_serial;                // Serial port pointer for Modbus
        uint32_t _rs485DePin;           // DE Pin for RS485 transceiver direction
        uint32_t _rs485RePin;           // RE Pin for RS485 transceiver direction
        uint16_t _mbAddr;               // Modbus address
        ModbusMaster _mb;
        uint16_t *_mbBuf;
        registerDefinition_t *_regDef;
        size_t _regDefLen;
        uint32_t _serialNumber;
        uint16_t _ctRatio = 1;  // TODO: implement
        char _influxQueryBuf[4096];
        const char *_influxMeasurement;
        void (*_influxSendRequest)(char *lineProtocolCommand);
};