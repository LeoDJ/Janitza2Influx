#include <stdint.h>

typedef struct {
    uint16_t address;
    uint8_t multiplier;
    uint8_t applyCtRatio : 1;
    uint8_t type : 2;
    uint8_t phaseTag : 3;
    const char *influxStr;
} registerDefinition_t;

enum registerDataTypes {
    CHAR,
    INT,
    LONG,
};

enum phaseTag {
    P_ALL,
    P_L1,
    P_L2,
    P_L3,
    P_L1L2,
    P_L2L3,
    P_L3L1,
    P_TAG_NUM // length of enum
};

const char *phaseTagStr[] = {
    "All",
    "L1",
    "L2",
    "L3",
    "L1-L2",
    "L2-L3",
    "L3-L1"
};


















































#define REG_SERIAL  911
#define REG_CT_PRIM 600
#define REG_CT_SEC  601


#define EMPTY {0, 1, false, INT, P_ALL, NULL}
#define LONG_SPACER EMPTY

// TODO: fix addresses (currently not needed)
// array is aligned with line numbers for easier referencing
registerDefinition_t registerDefinition[] = {
    //addr  multipl CTratio type    phase   name
    { 200,  10,     false,  INT,    P_L1,   "U_LN"          },
    { 201,  10,     false,  INT,    P_L2,   "U_LN"          },
    { 202,  10,     false,  INT,    P_L3,   "U_LN"          },
    { 203,  10,     false,  INT,    P_L1L2, "U_LL"          }, // L1-L2
    { 204,  10,     false,  INT,    P_L2L3, "U_LL"          },
    { 205,  10,     false,  INT,    P_L3L1, "U_LL"          },
    { 206,  1,      true,   INT,    P_L1,   "I"             },
    { 207,  1,      true,   INT,    P_L2,   "I"             },
    { 208,  1,      true,   INT,    P_L3,   "I"             },
    { 209,  10,     true,   INT,    P_L1,   "P"             }, // real power
    { 210,  10,     true,   INT,    P_L2,   "P"             }, 
    { 211,  10,     true,   INT,    P_L3,   "P"             }, 
    { 212,  10,     true,   INT,    P_L1,   "Q"             }, // reactive power
    { 213,  10,     true,   INT,    P_L2,   "Q"             },
    { 214,  10,     true,   INT,    P_L3,   "Q"             },
    { 215,  10,     true,   INT,    P_L1,   "S"             }, // apparent power
    { 216,  10,     true,   INT,    P_L2,   "S"             },
    { 217,  10,     true,   INT,    P_L3,   "S"             },
    { 218,  100,    false,  INT,    P_L1,   "CosPhi"        },
    { 219,  100,    false,  INT,    P_L2,   "CosPhi"        },
    { 220,  100,    false,  INT,    P_L3,   "CosPhi"        },
    {   1,  10,     false,  INT,    P_L1,   "U_LN_Harm1"    },
    {   1,  10,     false,  INT,    P_L1,   "U_LN_Harm3"    },
    {   1,  10,     false,  INT,    P_L1,   "U_LN_Harm5"    },
    {   1,  10,     false,  INT,    P_L1,   "U_LN_Harm7"    },
    {   1,  10,     false,  INT,    P_L1,   "U_LN_Harm9"    },
    {   1,  10,     false,  INT,    P_L1,   "U_LN_Harm11"   },
    {   1,  10,     false,  INT,    P_L1,   "U_LN_Harm13"   },
    {   1,  10,     false,  INT,    P_L1,   "U_LN_Harm15"   },
    {   1,  10,     false,  INT,    P_L2,   "U_LN_Harm1"    },
    {   1,  10,     false,  INT,    P_L2,   "U_LN_Harm3"    },
    {   1,  10,     false,  INT,    P_L2,   "U_LN_Harm5"    },
    {   1,  10,     false,  INT,    P_L2,   "U_LN_Harm7"    },
    {   1,  10,     false,  INT,    P_L2,   "U_LN_Harm9"    },
    {   1,  10,     false,  INT,    P_L2,   "U_LN_Harm11"   },
    {   1,  10,     false,  INT,    P_L2,   "U_LN_Harm13"   },
    {   1,  10,     false,  INT,    P_L2,   "U_LN_Harm15"   },
    {   1,  10,     false,  INT,    P_L3,   "U_LN_Harm1"    },
    {   1,  10,     false,  INT,    P_L3,   "U_LN_Harm3"    },
    {   1,  10,     false,  INT,    P_L3,   "U_LN_Harm5"    },
    {   1,  10,     false,  INT,    P_L3,   "U_LN_Harm7"    },
    {   1,  10,     false,  INT,    P_L3,   "U_LN_Harm9"    },
    {   1,  10,     false,  INT,    P_L3,   "U_LN_Harm11"   },
    {   1,  10,     false,  INT,    P_L3,   "U_LN_Harm13"   },
    {   1,  10,     false,  INT,    P_L3,   "U_LN_Harm15"   },
    {   1,  1,      true,   INT,    P_L1,   "I_Harm1"       },
    {   1,  1,      true,   INT,    P_L1,   "I_Harm3"       },
    {   1,  1,      true,   INT,    P_L1,   "I_Harm5"       },
    {   1,  1,      true,   INT,    P_L1,   "I_Harm7"       },
    {   1,  1,      true,   INT,    P_L1,   "I_Harm9"       },
    {   1,  1,      true,   INT,    P_L1,   "I_Harm11"      },
    {   1,  1,      true,   INT,    P_L1,   "I_Harm13"      },
    {   1,  1,      true,   INT,    P_L1,   "I_Harm15"      },
    {   1,  1,      true,   INT,    P_L2,   "I_Harm1"       },
    {   1,  1,      true,   INT,    P_L2,   "I_Harm3"       },
    {   1,  1,      true,   INT,    P_L2,   "I_Harm5"       },
    {   1,  1,      true,   INT,    P_L2,   "I_Harm7"       },
    {   1,  1,      true,   INT,    P_L2,   "I_Harm9"       },
    {   1,  1,      true,   INT,    P_L2,   "I_Harm11"      },
    {   1,  1,      true,   INT,    P_L2,   "I_Harm13"      },
    {   1,  1,      true,   INT,    P_L2,   "I_Harm15"      },
    {   1,  1,      true,   INT,    P_L3,   "I_Harm1"       },
    {   1,  1,      true,   INT,    P_L3,   "I_Harm3"       },
    {   1,  1,      true,   INT,    P_L3,   "I_Harm5"       },
    {   1,  1,      true,   INT,    P_L3,   "I_Harm7"       },
    {   1,  1,      true,   INT,    P_L3,   "I_Harm9"       },
    {   1,  1,      true,   INT,    P_L3,   "I_Harm11"      },
    {   1,  1,      true,   INT,    P_L3,   "I_Harm13"      },
    {   1,  1,      true,   INT,    P_L3,   "I_Harm15"      },
    {   1,  10,     false,  INT,    P_L1,   "THD_U"         },
    {   1,  10,     false,  INT,    P_L2,   "THD_U"         },
    {   1,  10,     false,  INT,    P_L3,   "THD_U"         },
    {   1,  10,     false,  INT,    P_L1,   "THD_I"         },
    {   1,  10,     false,  INT,    P_L2,   "THD_I"         },
    {   1,  10,     false,  INT,    P_L3,   "THD_I"         },
    {   1,  100,    false,  INT,    P_L1,   "F"             },
    {   1,  100,    false,  INT,    P_ALL,  "CosPhi"        },
    {   1,  1,      false,  INT,    P_ALL,  "Rotation"      },
    {   1,  1,      false,  INT,    P_ALL,  "I_N"           },
    {   1,  1,      true,   INT,    P_ALL,  "P"             },
    {   1,  1,      true,   INT,    P_ALL,  "Q"             },
    {   1,  1,      true,   INT,    P_ALL,  "S"             },
    EMPTY, // {   0,  1,      true,   INT,    P_L1,   "I_Mean"        },
    EMPTY, // {   0,  1,      true,   INT,    P_L2,   "I_Mean"        },
    EMPTY, // {   0,  1,      true,   INT,    P_L3,   "I_Mean"        },
    EMPTY, // {   0,  10,     true,   INT,    P_L1,   "P_Mean"        }, // real power
    EMPTY, // {   0,  10,     true,   INT,    P_L2,   "P_Mean"        }, 
    EMPTY, // {   0,  10,     true,   INT,    P_L3,   "P_Mean"        }, 
    EMPTY, // {   0,  10,     true,   INT,    P_L1,   "Q_Mean"        }, // reactive power
    EMPTY, // {   0,  10,     true,   INT,    P_L2,   "Q_Mean"        },
    EMPTY, // {   0,  10,     true,   INT,    P_L3,   "Q_Mean"        },
    EMPTY, // {   0,  10,     true,   INT,    P_L1,   "S_Mean"        }, // apparent power
    EMPTY, // {   0,  10,     true,   INT,    P_L2,   "S_Mean"        },
    EMPTY, // {   0,  10,     true,   INT,    P_L3,   "S_Mean"        },
    EMPTY,
    EMPTY,
    EMPTY,
    EMPTY,
    EMPTY,
    EMPTY,
    EMPTY,
    EMPTY,
    EMPTY,
    EMPTY,
    EMPTY,
    EMPTY,
    EMPTY,
    EMPTY,
    EMPTY,
    EMPTY,
    EMPTY,
    EMPTY,
    EMPTY,
    EMPTY,
    EMPTY,
    EMPTY,
    EMPTY,
    EMPTY,
    EMPTY,
    EMPTY,
    EMPTY,
    EMPTY,
    EMPTY,
    EMPTY,
    EMPTY,
    EMPTY,
    EMPTY,
    EMPTY,
    EMPTY,
    EMPTY,
    EMPTY,
    EMPTY,
    EMPTY,
    EMPTY,
    EMPTY,
    EMPTY,
    EMPTY,
    EMPTY,
    EMPTY,
    EMPTY,
    EMPTY,
    EMPTY,
    EMPTY,
    EMPTY,
    EMPTY,
    EMPTY,
    EMPTY,
    EMPTY,
    EMPTY,
    EMPTY,
    EMPTY,
    EMPTY,
    EMPTY,
    EMPTY,
    EMPTY,
    EMPTY,
    EMPTY,
    EMPTY,
    EMPTY,
    EMPTY,
    EMPTY,
    EMPTY,
    EMPTY,
    EMPTY,
    EMPTY,
    EMPTY,
    EMPTY,
    EMPTY,
    EMPTY,
    EMPTY,
    EMPTY,
    EMPTY,
    EMPTY,
    EMPTY,
    EMPTY,
    EMPTY,
    EMPTY,
    EMPTY,
    EMPTY,
    EMPTY,
    EMPTY,
    EMPTY,
    EMPTY,
    EMPTY,
    EMPTY,
    EMPTY,
    EMPTY,
    EMPTY,
    EMPTY,
    EMPTY,
    EMPTY,
    EMPTY,
    EMPTY,
    EMPTY,
    {   1,  60,    false,  LONG,  P_ALL,  "OperatingTime"   },
    LONG_SPACER,
    EMPTY,
    LONG_SPACER,
    EMPTY,
    LONG_SPACER,
    EMPTY,
    LONG_SPACER,
    EMPTY,
    LONG_SPACER,
    EMPTY,
    LONG_SPACER,
    EMPTY,
    LONG_SPACER,
    {   1,  1,      false,  INT,   P_ALL,  "Temperature"    },
    {   1,  100,    false,  INT,   P_ALL,  "U_Operating"    },
    EMPTY,
    LONG_SPACER,
    EMPTY,
    EMPTY,
    EMPTY,
    EMPTY,
    {   1,  1,      true,   LONG,  P_ALL,  "Wp"             }, // Wh
    LONG_SPACER,
    {   1,  1,      true,   LONG,  P_ALL,  "Wq"             },
    LONG_SPACER,
    EMPTY,
    EMPTY,
    {   1,  1,      true,   LONG,  P_ALL,  "Wp_Import"      }, // Wh
    LONG_SPACER,
    {   1,  1,      true,   LONG,  P_ALL,  "Wp_Supply"      }, // Wh
    LONG_SPACER,
};
#define REG_DEF_NUM   (sizeof(registerDefinition) / sizeof(registerDefinition[0]))
#define REG_DEF_START_ADDR  200