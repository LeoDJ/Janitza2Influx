#include <stdint.h>

typedef struct {
    uint16_t address;
    uint8_t multiplier;
    uint8_t applyCtRatio : 1;
    uint8_t type : 2;
    const char *influxStr;
} registerDefinition_t;

enum registerDataTypes {
    CHAR,
    INT,
    LONG,
};

registerDefinition_t registerDefinition[] = {
    //addr  multipl CTratio type    name
    { 200,  10,     false,  INT,    "ULN1"      },
    { 201,  10,     false,  INT,    "ULN2"      },
    { 202,  10,     false,  INT,    "ULN3"      },
    { 203,  10,     false,  INT,    "ULL12"     }, // L1-L2
    { 204,  10,     false,  INT,    "ULL23"     },
    { 205,  10,     false,  INT,    "ULL31"     },
    { 206,  1,      true,   INT,    "IL1"       },
    { 207,  1,      true,   INT,    "IL2"       },
    { 208,  1,      true,   INT,    "IL3"       },
    { 209,  10,     true,   INT,    "PL1"       }, // real power
    { 210,  10,     true,   INT,    "PL2"       }, 
    { 211,  10,     true,   INT,    "PL3"       }, 
    { 212,  10,     true,   INT,    "QL1"       }, // reactive power
    { 213,  10,     true,   INT,    "QL2"       },
    { 214,  10,     true,   INT,    "QL3"       },
    { 215,  10,     true,   INT,    "SL1"       }, // apparent power
    { 216,  10,     true,   INT,    "SL2"       },
    { 217,  10,     true,   INT,    "SL3"       },
    { 218,  100,    false,  INT,    "CosPhiL1"  },
    { 219,  100,    false,  INT,    "CosPhiL2"  },
    { 220,  100,    false,  INT,    "CosPhiL3"  },
    { 221,  10,     false,  INT,    "ULN1H1"    },
    { 221,  10,     false,  INT,    "ULN1H3"    },
    { 221,  10,     false,  INT,    "ULN1H5"    },
    { 221,  10,     false,  INT,    "ULN1H7"    },
    { 221,  10,     false,  INT,    "ULN1H9"    },
    { 221,  10,     false,  INT,    "ULN1H11"   },
    { 221,  10,     false,  INT,    "ULN1H13"   },
    { 221,  10,     false,  INT,    "ULN1H15"   },
    // ...
};
#define REG_DEF_NUM   (sizeof(registerDefinition) / sizeof(registerDefinition[0]))
#define REG_DEF_START_ADDR  200

#define REG_SERIAL  911