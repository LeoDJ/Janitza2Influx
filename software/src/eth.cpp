#include "eth.h"

#ifdef ARDUINO_ARCH_STM32
uint8_t macAddrArr[6];
uint8_t *macAddr = macAddrArr;
char macStr[13];
// EthernetClient net;

static inline uint64_t mix(uint64_t h) {
    h ^= h >> 23;
    h *= 0x2127599bf4325c37ULL;
    h ^= h >> 47;
    //
    return h;
}

// hash function copied from https://stackoverflow.com/a/47895889
uint64_t fastHash64(const void * buf, size_t len, uint64_t seed) {
    const uint64_t m = 0x880355f21e6d1965ULL;
    const uint64_t * pos = (const uint64_t*)buf;
    const uint64_t * end = pos + (len / 8);
    const unsigned char * pos2;
    uint64_t h = seed ^ (len * m);
    uint64_t v;

    while(pos != end)
    {
        v  = *pos++;
        h ^= mix(v);
        h *= m;
    }

    pos2 = (const unsigned char*)pos;
    v = 0;

    switch(len & 7)
    {
        case 7: v ^= (uint64_t)pos2[6] << 48;
        case 6: v ^= (uint64_t)pos2[5] << 40;
        case 5: v ^= (uint64_t)pos2[4] << 32;
        case 4: v ^= (uint64_t)pos2[3] << 24;
        case 3: v ^= (uint64_t)pos2[2] << 16;
        case 2: v ^= (uint64_t)pos2[1] << 8;
        case 1: v ^= (uint64_t)pos2[0];
                h ^= mix(v);
                h *= m;
    }

    return mix(h);
}

void generateMAC(uint8_t *macArray) {

    // uint32_t uid[3] = {HAL_GetUIDw0(), HAL_GetUIDw1(), HAL_GetUIDw2()};
    uint64_t hash = fastHash64((uint8_t *)UID_BASE, 12, 0x421337f00beefULL);
    memcpy(macArray, &hash, 6);
    macArray[0] = 0x42; // set first byte to predefined value, where private bit is set

    snprintf(macStr, 13, "%02X%02X%02X%02X%02X%02X", macArray[0], macArray[1], macArray[2], macArray[3], macArray[4], macArray[5]);
}

void initEthernet() {
    SPI.setMOSI(SPI2_MOSI);
    SPI.setMISO(SPI2_MISO);
    SPI.setSCLK(SPI2_SCK);

    Ethernet.init(SPI2_CS);

    generateMAC(macAddr);
}
#endif
#ifdef ARDUINO_ARCH_CH32V
void initEthernet() {
    Ethernet.setLedPins(PB0, PB1, true);
}  
uint8_t *macAddr = nullptr; // Ethernet uses internal MAC, if parameter is nullptr
#endif


bool connectEthernet() {
    DBG.println("Initialize Ethernet");

    bool success= true;

    // DHCP
    if (Ethernet.begin(macAddr, 10000) == 0) {
        DBG.println("Failed to configure Ethernet using DHCP");
        success = false;
    }
    // Static IP
    // Ethernet.begin(macAddr, IPAddress(192,168,13,245));

    if (Ethernet.hardwareStatus() == EthernetNoHardware) {
        DBG.println("Ethernet module was not found.  Sorry, can't run without hardware. :(");
        // delay(5000);
        // HAL_NVIC_SystemReset();
    } else if (Ethernet.linkStatus() == LinkOFF) {
        DBG.println("Ethernet cable is not connected.");
        success= false;
    }
    
    if(success) {
        // print your local IP address:
        DBG.print("My IP address: ");
        DBG.println(Ethernet.localIP());
    }
    return success;
}