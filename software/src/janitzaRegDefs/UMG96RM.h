#include "janitza.h"

registerDefinition_t regDef_UMG96RM[] = {
    // addr     multipl CTratio type            phase               name
    { 754,      1,      false,  Janitza::INT,   Janitza::P_NONE,    "SN"            },  // index 0 has to be serial number
    { 10,       1,      false,  Janitza::FLOAT, Janitza::P_NONE,    "CT_prim"       },  // index 1 has to be CT primary
    { 12,       1,      false,  Janitza::FLOAT, Janitza::P_NONE,    "CT_sec"        },  // index 2 has to be CT secondary


    { 19000,    1,      false,  Janitza::FLOAT, Janitza::P_L1,      "U_LN"          },  // V
    { 19002,    1,      false,  Janitza::FLOAT, Janitza::P_L2,      "U_LN"          },  // V
    { 19004,    1,      false,  Janitza::FLOAT, Janitza::P_L3,      "U_LN"          },  // V
    { 19006,    1,      false,  Janitza::FLOAT, Janitza::P_L1L2,    "U_LL"          },  // V
    { 19008,    1,      false,  Janitza::FLOAT, Janitza::P_L2L3,    "U_LL"          },  // V
    { 19010,    1,      false,  Janitza::FLOAT, Janitza::P_L3L1,    "U_LL"          },  // V
    { 19012,    1,      false,  Janitza::FLOAT, Janitza::P_L1,      "I"             },  // A
    { 19014,    1,      false,  Janitza::FLOAT, Janitza::P_L2,      "I"             },  // A
    { 19016,    1,      false,  Janitza::FLOAT, Janitza::P_L3,      "I"             },  // A
    { 19018,    1,      false,  Janitza::FLOAT, Janitza::P_ALL,     "I_N"           },  // A
    { 19020,    1,      false,  Janitza::FLOAT, Janitza::P_L1,      "P"             },  // W
    { 19022,    1,      false,  Janitza::FLOAT, Janitza::P_L2,      "P"             },  // W
    { 19024,    1,      false,  Janitza::FLOAT, Janitza::P_L3,      "P"             },  // W
    { 19026,    1,      false,  Janitza::FLOAT, Janitza::P_ALL,     "P"             },  // W
    { 19028,    1,      false,  Janitza::FLOAT, Janitza::P_L1,      "Q"             },  // VA
    { 19030,    1,      false,  Janitza::FLOAT, Janitza::P_L2,      "Q"             },  // VA
    { 19032,    1,      false,  Janitza::FLOAT, Janitza::P_L3,      "Q"             },  // VA
    { 19034,    1,      false,  Janitza::FLOAT, Janitza::P_ALL,     "Q"             },  // VA
    { 19036,    1,      false,  Janitza::FLOAT, Janitza::P_L1,      "S"             },  // var
    { 19038,    1,      false,  Janitza::FLOAT, Janitza::P_L2,      "S"             },  // var
    { 19040,    1,      false,  Janitza::FLOAT, Janitza::P_L3,      "S"             },  // var
    { 19042,    1,      false,  Janitza::FLOAT, Janitza::P_ALL,     "S"             },  // var
    { 19044,    1,      false,  Janitza::FLOAT, Janitza::P_L1,      "CosPhi"        },  // -
    { 19046,    1,      false,  Janitza::FLOAT, Janitza::P_L2,      "CosPhi"        },  // -
    { 19048,    1,      false,  Janitza::FLOAT, Janitza::P_L3,      "CosPhi"        },  // -
    { 19050,    1,      false,  Janitza::FLOAT, Janitza::P_NONE,    "F"             },  // Hz
    { 19052,    1,      false,  Janitza::FLOAT, Janitza::P_NONE,    "Rotation"      },  // -1: left, 0: none, 1: right
    { 19054,    1,      false,  Janitza::FLOAT, Janitza::P_L1,      "Wp"            },  // Wh
    { 19056,    1,      false,  Janitza::FLOAT, Janitza::P_L2,      "Wp"            },  // Wh
    { 19058,    1,      false,  Janitza::FLOAT, Janitza::P_L3,      "Wp"            },  // Wh
    { 19060,    1,      false,  Janitza::FLOAT, Janitza::P_ALL,     "Wp"            },  // Wh
    { 19062,    1,      false,  Janitza::FLOAT, Janitza::P_L1,      "Wp_Consumed"   },  // Wh
    { 19064,    1,      false,  Janitza::FLOAT, Janitza::P_L2,      "Wp_Consumed"   },  // Wh
    { 19066,    1,      false,  Janitza::FLOAT, Janitza::P_L3,      "Wp_Consumed"   },  // Wh
    { 19068,    1,      false,  Janitza::FLOAT, Janitza::P_ALL,     "Wp_Consumed"   },  // Wh
    { 19070,    1,      false,  Janitza::FLOAT, Janitza::P_L1,      "Wp_Delivered"  },  // Wh
    { 19072,    1,      false,  Janitza::FLOAT, Janitza::P_L2,      "Wp_Delivered"  },  // Wh
    { 19074,    1,      false,  Janitza::FLOAT, Janitza::P_L3,      "Wp_Delivered"  },  // Wh
    { 19076,    1,      false,  Janitza::FLOAT, Janitza::P_ALL,     "Wp_Delivered"  },  // Wh
    { 19078,    1,      false,  Janitza::FLOAT, Janitza::P_L1,      "Wq"            },  // VAh
    { 19080,    1,      false,  Janitza::FLOAT, Janitza::P_L2,      "Wq"            },  // VAh
    { 19082,    1,      false,  Janitza::FLOAT, Janitza::P_L3,      "Wq"            },  // VAh
    { 19084,    1,      false,  Janitza::FLOAT, Janitza::P_ALL,     "Wq"            },  // VAh
    { 19086,    1,      false,  Janitza::FLOAT, Janitza::P_L1,      "Ws"            },  // varh
    { 19088,    1,      false,  Janitza::FLOAT, Janitza::P_L2,      "Ws"            },  // varh
    { 19090,    1,      false,  Janitza::FLOAT, Janitza::P_L3,      "Ws"            },  // varh
    { 19092,    1,      false,  Janitza::FLOAT, Janitza::P_ALL,     "Ws"            },  // varh
    { 19094,    1,      false,  Janitza::FLOAT, Janitza::P_L1,      "Ws_ind"        },  // varh
    { 19096,    1,      false,  Janitza::FLOAT, Janitza::P_L2,      "Ws_ind"        },  // varh
    { 19098,    1,      false,  Janitza::FLOAT, Janitza::P_L3,      "Ws_ind"        },  // varh
    { 19100,    1,      false,  Janitza::FLOAT, Janitza::P_ALL,     "Ws_ind"        },  // varh
    { 19102,    1,      false,  Janitza::FLOAT, Janitza::P_L1,      "Ws_cap"        },  // varh
    { 19104,    1,      false,  Janitza::FLOAT, Janitza::P_L2,      "Ws_cap"        },  // varh
    { 19106,    1,      false,  Janitza::FLOAT, Janitza::P_L3,      "Ws_cap"        },  // varh
    { 19108,    1,      false,  Janitza::FLOAT, Janitza::P_ALL,     "Ws_cap"        },  // varh
    { 19110,    1,      false,  Janitza::FLOAT, Janitza::P_L1,      "THD_U"         },  // %
    { 19112,    1,      false,  Janitza::FLOAT, Janitza::P_L2,      "THD_U"         },  // %
    { 19114,    1,      false,  Janitza::FLOAT, Janitza::P_L3,      "THD_U"         },  // %
    { 19116,    1,      false,  Janitza::FLOAT, Janitza::P_L1,      "THD_I"         },  // %
    { 19118,    1,      false,  Janitza::FLOAT, Janitza::P_L2,      "THD_I"         },  // %
    { 19120,    1,      false,  Janitza::FLOAT, Janitza::P_L3,      "THD_I"         },  // %
    
    {  5896,    1,      false,  Janitza::INT,   Janitza::P_NONE,    "OperatingTime" },  // seconds

    // {  1000,    1,      false,  Janitza::FLOAT, Janitza::P_L1,      "U_LN_Harm1"    },   // TODO



};