#include "janitza.h"
#include <functional>

#define _debugPrintf \
    if (_debug)      \
    _debug->printf
#define MB_CHUNK_SIZE 32       // number of addresses to read at once
#define REG_DEF_START 3        // skip over serial number and CT_ratio register defs
#define FLOAT_CUTOFF 0.000001f // send values under this cutoff as o

void Janitza::init(Stream& serialPort, uint16_t modbusAddress, registerDefinition_t* registerDef, size_t regDefSize) {
    _serial = &serialPort;
    _mbAddr = modbusAddress;
    _regDef = registerDef;
    _regDefLen = regDefSize / sizeof(registerDefinition_t);

    _mb.begin(_mbAddr, *_serial);

    while (_serialNumber == 0) {
        _serialNumber = readSerialNumber();
        if (_serialNumber != 0) {
            _debugPrintf("Serial: %d\n", _serialNumber);
        }
        else {
            _debugPrintf(
                "Error: Could not read serial number, make sure the meter is connected and address and baud rate are correct. Retrying in 5s...\n");
            delay(5000);
        }
    }

    _mbBuf = (uint16_t*)malloc(getRequiredModbusBufferSize(REG_DEF_START, _regDefLen - 1));
    if (_mbBuf == NULL) {
        _debugPrintf("Error: Not enough memory available to allocate Modbus buffer!\n");
    }
}

bool Janitza::read() {
    // get contiguous register areas
    // do reads
    // generate influx queries on request and call callback

    uint16_t regDefIndex = REG_DEF_START;
    uint16_t mbBufIdx = 0;

    do {
        uint16_t regDefIndexLastContig = getContiguousRegisters(regDefIndex);
        uint16_t startAddr = _regDef[regDefIndex].address;
        uint16_t regCount =
            (_regDef[regDefIndexLastContig].address + registerDataTypeSize[_regDef[regDefIndexLastContig].type]) -
            startAddr;

        _debugPrintf("Start: %d (%d), Count: %d (%d)\n", regDefIndexLastContig, startAddr, regCount,
                     _regDef[regDefIndexLastContig].address);

        bool success = modbusReadBulk((int16_t*)(_mbBuf + mbBufIdx), startAddr, regCount);
        if (!success) {
            _debugPrintf("Error reading bulk from ModBus\n");
            return false;
        }

        mbBufIdx += regCount * 2; // modbus uses 16bit long registers

        regDefIndex = regDefIndexLastContig + 1;
    } while (regDefIndex < _regDefLen);

    return true;
}

float Janitza::getValue(registerDefinition_t* regDef, uint16_t mbBufIdx) {
    float mbValue;
    switch (regDef->type) {
        // case registerDataTypes::BYTE:
        case registerDataTypes::SHORT:
            mbValue = (int16_t)_mbBuf[mbBufIdx];
            break;
        case registerDataTypes::CHAR:
            mbValue = (uint16_t)_mbBuf[mbBufIdx];
            break;
        case registerDataTypes::INT:
            mbValue = _mbBuf[mbBufIdx] << 16 | (_mbBuf[mbBufIdx + 1] & 0xFFFF);
            break;
        case registerDataTypes::FLOAT:
            memcpy(&mbValue, &_mbBuf[mbBufIdx + 1], 2);
            memcpy(((uint8_t*)&mbValue) + 2, &_mbBuf[mbBufIdx], 2);
            if (fabsf(mbValue) < FLOAT_CUTOFF) {
                mbValue = 0;
            }
            break;
        default:
            _debugPrintf("Error: Current register type not implemented yet! addr = %d, type = %d\n", regDef->address,
                         regDef->type);
            return NAN;
    }

    float val = mbValue / (float)regDef->multiplier;

    // apply current transformer ratio if needed (reading actual CT ratio currently not implemented, TODO)
    if (regDef->applyCtRatio) {
        val *= (float)_ctRatio;
    }

    return val;
}

bool Janitza::generateInfluxCommands() {
    int influxQueryLen = 0;

    for (int phase = 0; phase < P_TAG_NUM; phase++) {
        const char* phaseStr = phaseTagStr[phase];
        uint16_t mbBufIdx = 0;

        influxQueryLen += sprintf(_influxQueryBuf + influxQueryLen, "%s", _influxMeasurement); // measurement name
        influxQueryLen += sprintf(_influxQueryBuf + influxQueryLen, ",serial=%lu,phase=%s ", _serialNumber,
                                  phaseStr); // print tags here

        for (size_t i = REG_DEF_START; i < _regDefLen; i++) {
            registerDefinition_t* regDef = &_regDef[i];

            // if (regDef->address == 0) { // skip "disabled" addresses
            //     continue;
            // }

            if (regDef->phaseTag == phase) {

                float val = getValue(regDef, mbBufIdx);
                if (!isnanf(val)) {
                    influxQueryLen += sprintf(_influxQueryBuf + influxQueryLen, "%s=%g,", regDef->nameStr, val);
                }
            }
            mbBufIdx += registerDataTypeSize[regDef->type] / 2; // increment modbus buffer index by type size
        }

        _influxQueryBuf[influxQueryLen - 1] = '\n'; // replace trailing comma with newline
    }
    return true;
}

void Janitza::readAndSendToInflux() {
    bool success = read();
    if (success) {
        success = generateInfluxCommands();
        if (success) {
            _debugPrintf("Sending InfluxDB Request...\n");
            _debugPrintf("%s\n", _influxQueryBuf);
            _influxSendRequest(_influxQueryBuf);
        }
    }
}

JsonDocument Janitza::generateJson() {
    JsonDocument doc;
    doc["SerialNumber"] = _serialNumber;

    uint16_t mbBufIdx = 0;
    for (size_t i = REG_DEF_START; i < _regDefLen; i++) {
        registerDefinition_t* regDef = &_regDef[i];

        float val = getValue(regDef, mbBufIdx);

        mbBufIdx += registerDataTypeSize[regDef->type] / 2; // increment modbus buffer index by type size

        if (regDef->phaseTag != P_NONE) {
            const char *phaseStr = phaseTagStr[regDef->phaseTag];

            if (regDef->index == -1) {
                doc["Values"][regDef->nameStr][phaseStr] = val;
            }
            else {
                JsonArray arr = doc["Values"][regDef->nameStr][phaseStr].to<JsonArray>();
                arr.add(val); // only works when definitions are sorted
            }
        }
        else {
            if (regDef->index == -1) {
                doc["Values"][regDef->nameStr] = val;
            }
            else {
                JsonArray arr = doc["Values"][regDef->nameStr].to<JsonArray>();
                arr.add(val); // only works when definitions are sorted
            }
        }
    }

    return doc;
}

// returns last index of contiguous register definition area (for bulk modbus reading)
uint16_t Janitza::getContiguousRegisters(uint16_t startIndex) {
    if (startIndex >= _regDefLen - 1) { // if at or over last element, return that
        return _regDefLen - 1;
    }

    for (size_t i = startIndex; i < _regDefLen - 1; i++) { // loop until second to last element
        registerDefinition_t def = _regDef[i];
        uint16_t nextAddr = def.address + (registerDataTypeSize[def.type] / 2);
        if (_regDef[i + 1].address != nextAddr) {
            return i;
        }
    }

    // if the for loop completes, return the last element
    return _regDefLen - 1;
}

void Janitza::setInfluxSendCallback(void (*influxSendRequest)(char* lineProtocolCommand), const char* measurementName) {
    _influxSendRequest = influxSendRequest;
    _influxMeasurement = measurementName;
}

void Janitza::useRS485(uint32_t dePin, uint32_t rePin, void (*preTransmission)(), void (*postTransmission)()) {
    _rs485DePin = dePin;
    _rs485RePin = rePin;

    pinMode(_rs485DePin, OUTPUT);
    pinMode(_rs485RePin, OUTPUT);
    digitalWrite(_rs485DePin, 0);
    digitalWrite(_rs485RePin, 0);

    // need to pass functions from external, because apparently I can't easily generate a function inside this class
    // that uses the private pin variable but is also convertable to a simple function pointer
    // could potentially be implemented better by this: https://stackoverflow.com/a/29817048
    _mb.preTransmission(preTransmission);
    _mb.postTransmission(postTransmission);
}

void Janitza::rs485Transmit() {
    digitalWrite(_rs485DePin, 1);
    digitalWrite(_rs485RePin, 1);
}

void Janitza::rs485Receive() {
    digitalWrite(_rs485DePin, 0);
    digitalWrite(_rs485RePin, 0);
}

void Janitza::setDebugSerial(Stream& serialPort) {
    _debug = &serialPort;
}

uint16_t Janitza::getRequiredModbusBufferSize(uint16_t regStart, uint16_t regEnd) {
    uint16_t requiredSize = 0;
    for (int i = regStart; i <= regEnd; i++) {
        uint8_t typeSize = registerDataTypeSize[_regDef[i].type];
        requiredSize += typeSize;
    }
    return requiredSize;
}

uint32_t Janitza::readSerialNumber() {
    uint8_t ret = _mb.readHoldingRegisters(_regDef[0].address, 2);
    if (ret != _mb.ku8MBSuccess) {
        _debugPrintf("Error reading from ModBus in readSerialNumber. Status = %02X\n", ret);
        return 0;
    }
    return _mb.getResponseBuffer(0) << 16 | _mb.getResponseBuffer(1);
}

uint32_t Janitza::getSerialNumber() {
    return _serialNumber;
}

bool Janitza::modbusReadBulk(int16_t* destBuf, uint16_t startAddr, uint16_t count) {
    int iterations = (count + (MB_CHUNK_SIZE - 1)) / MB_CHUNK_SIZE; // division, but rounded up
    for (int i = 0; i < iterations; i++) {
        uint16_t mbStart = startAddr + i * MB_CHUNK_SIZE;
        uint8_t mbCount = MB_CHUNK_SIZE;
        if (MB_CHUNK_SIZE * (i + 1) > count) {
            mbCount = count % MB_CHUNK_SIZE;
        }

        _debugPrintf("Reading Modbus %d/%d (Addr: %d, Count: %d)         \r", i + 1, iterations, mbStart, mbCount);

        uint8_t ret = _mb.readHoldingRegisters(mbStart, mbCount);

        if (ret != _mb.ku8MBSuccess) {
            _debugPrintf("\nError reading from ModBus. Status = %02X\n", ret);
            return false;
        }

        for (int j = 0; j < mbCount; j++) {
            destBuf[i * MB_CHUNK_SIZE + j] = _mb.getResponseBuffer(j);
        }
    }
    _debugPrintf("\n");
    return true;
}
