#include "janitza.h"

registerDefinition_t regDef_UMG96RM[] = {
    // addr     multipl CTratio type            phase               name
    { 754,      1,      false,  Janitza::INT,   Janitza::P_NONE,     "SN"            },  // index 0 has to be serial number
    { 10,       1,      false,  Janitza::FLOAT, Janitza::P_NONE,     "CT_prim"       },  // index 1 has to be CT primary
    { 12,       1,      false,  Janitza::FLOAT, Janitza::P_NONE,     "CT_sec"        },  // index 2 has to be CT secondary


    { 19000,    1,      false,  Janitza::FLOAT, Janitza::P_L1,      "U_LN"          },
    { 19002,    1,      false,  Janitza::FLOAT, Janitza::P_L2,      "U_LN"          },
    { 19004,    1,      false,  Janitza::FLOAT, Janitza::P_L3,      "U_LN"          },
    { 19006,    1,      false,  Janitza::FLOAT, Janitza::P_L1L2,    "U_LL"          },
    { 19008,    1,      false,  Janitza::FLOAT, Janitza::P_L2L3,    "U_LL"          },
    { 19010,    1,      false,  Janitza::FLOAT, Janitza::P_L3L1,    "U_LL"          },
    { 19012,    1,      false,  Janitza::FLOAT, Janitza::P_L1,      "I"             },
    { 19014,    1,      false,  Janitza::FLOAT, Janitza::P_L2,      "I"             },
    { 19016,    1,      false,  Janitza::FLOAT, Janitza::P_L3,      "I"             },
    { 19018,    1,      false,  Janitza::FLOAT, Janitza::P_ALL,     "I_N"           },
    { 19020,    1,      false,  Janitza::FLOAT, Janitza::P_L1,      "P"             },
    { 19022,    1,      false,  Janitza::FLOAT, Janitza::P_L2,      "P"             },
    { 19024,    1,      false,  Janitza::FLOAT, Janitza::P_L3,      "P"             },
    { 19026,    1,      false,  Janitza::FLOAT, Janitza::P_ALL,     "P"             },
    { 19028,    1,      false,  Janitza::FLOAT, Janitza::P_L1,      "Q"             },
    { 19030,    1,      false,  Janitza::FLOAT, Janitza::P_L2,      "Q"             },
    { 19032,    1,      false,  Janitza::FLOAT, Janitza::P_L3,      "Q"             },
    { 19034,    1,      false,  Janitza::FLOAT, Janitza::P_ALL,     "Q"             },
    { 19036,    1,      false,  Janitza::FLOAT, Janitza::P_L1,      "S"             },
    { 19038,    1,      false,  Janitza::FLOAT, Janitza::P_L2,      "S"             },
    { 19040,    1,      false,  Janitza::FLOAT, Janitza::P_L3,      "S"             },
    { 19042,    1,      false,  Janitza::FLOAT, Janitza::P_ALL,     "S"             },
    { 19044,    1,      false,  Janitza::FLOAT, Janitza::P_L1,      "CosPhi"        },
    { 19046,    1,      false,  Janitza::FLOAT, Janitza::P_L2,      "CosPhi"        },
    { 19048,    1,      false,  Janitza::FLOAT, Janitza::P_L3,      "CosPhi"        },
    { 19050,    1,      false,  Janitza::FLOAT, Janitza::P_NONE,    "F"             },
    { 19052,    1,      false,  Janitza::FLOAT, Janitza::P_NONE,    "Rotation"      },
    { 19054,    1,      false,  Janitza::FLOAT, Janitza::P_L1,      "Wp"            },
    { 19056,    1,      false,  Janitza::FLOAT, Janitza::P_L2,      "Wp"            },
    { 19058,    1,      false,  Janitza::FLOAT, Janitza::P_L3,      "Wp"            },
    { 19060,    1,      false,  Janitza::FLOAT, Janitza::P_ALL,     "Wp"            },
    { 19062,    1,      false,  Janitza::FLOAT, Janitza::P_L1,      "Wp_Consumed"   },
    { 19064,    1,      false,  Janitza::FLOAT, Janitza::P_L2,      "Wp_Consumed"   },
    { 19066,    1,      false,  Janitza::FLOAT, Janitza::P_L3,      "Wp_Consumed"   },
    { 19068,    1,      false,  Janitza::FLOAT, Janitza::P_ALL,     "Wp_Consumed"   },
    { 19070,    1,      false,  Janitza::FLOAT, Janitza::P_L1,      "Wp_Delivered"  },
    { 19072,    1,      false,  Janitza::FLOAT, Janitza::P_L2,      "Wp_Delivered"  },
    { 19074,    1,      false,  Janitza::FLOAT, Janitza::P_L3,      "Wp_Delivered"  },
    { 19076,    1,      false,  Janitza::FLOAT, Janitza::P_ALL,     "Wp_Delivered"  },
    { 19078,    1,      false,  Janitza::FLOAT, Janitza::P_L1,      "Wq"            },
    { 19080,    1,      false,  Janitza::FLOAT, Janitza::P_L2,      "Wq"            },
    { 19082,    1,      false,  Janitza::FLOAT, Janitza::P_L3,      "Wq"            },
    { 19084,    1,      false,  Janitza::FLOAT, Janitza::P_ALL,     "Wq"            },
    { 19086,    1,      false,  Janitza::FLOAT, Janitza::P_L1,      "Ws"            },
    { 19088,    1,      false,  Janitza::FLOAT, Janitza::P_L2,      "Ws"            },
    { 19090,    1,      false,  Janitza::FLOAT, Janitza::P_L3,      "Ws"            },
    { 19092,    1,      false,  Janitza::FLOAT, Janitza::P_ALL,     "Ws"            },
    { 19094,    1,      false,  Janitza::FLOAT, Janitza::P_L1,      "Ws_ind"        },
    { 19096,    1,      false,  Janitza::FLOAT, Janitza::P_L2,      "Ws_ind"        },
    { 19098,    1,      false,  Janitza::FLOAT, Janitza::P_L3,      "Ws_ind"        },
    { 19100,    1,      false,  Janitza::FLOAT, Janitza::P_ALL,     "Ws_ind"        },
    { 19102,    1,      false,  Janitza::FLOAT, Janitza::P_L1,      "Ws_cap"        },
    { 19104,    1,      false,  Janitza::FLOAT, Janitza::P_L2,      "Ws_cap"        },
    { 19106,    1,      false,  Janitza::FLOAT, Janitza::P_L3,      "Ws_cap"        },
    { 19108,    1,      false,  Janitza::FLOAT, Janitza::P_ALL,     "Ws_cap"        },
    { 19110,    1,      false,  Janitza::FLOAT, Janitza::P_L1,      "THD_U"         },
    { 19112,    1,      false,  Janitza::FLOAT, Janitza::P_L2,      "THD_U"         },
    { 19114,    1,      false,  Janitza::FLOAT, Janitza::P_L3,      "THD_U"         },
    { 19116,    1,      false,  Janitza::FLOAT, Janitza::P_L1,      "THD_I"         },
    { 19118,    1,      false,  Janitza::FLOAT, Janitza::P_L2,      "THD_I"         },
    { 19120,    1,      false,  Janitza::FLOAT, Janitza::P_L3,      "THD_I"         },
    
    {  5896,    1,      false,  Janitza::INT,   Janitza::P_NONE,    "OperatingTime" },  // seconds

    // {  1000,    1,      false,  Janitza::FLOAT, Janitza::P_L1,      "U_LN_Harm1"    },   // TODO



};