#ifndef BUSPORTLIB_BODY_GUARD
#define BUSPORTLIB_BODY_GUARD


#include <busportlib.h>






busportlib::busportlib(/* args */) {
}

busportlib::~busportlib() {
}




void busportlib::begin() {
    // set up the bus lines...

    // 0 GND
    // 1 VCC
    // 2 MVC_LEDTOGND_0
    i2cbusmaster.enable_bus( 0x01 );
    busobj_2_mvc_ledtognd_0.begin( 0x45 );

    // 3 MVC_LEDTOGND_1
    i2cbusmaster.enable_bus( 0x00 );
    busobj_3_mvc_ledtognd_1.begin( 0x45 );

    // 4 MVC_GND
    i2cbusmaster.enable_bus( 0x01 );
    busobj_4_mvc_gnd.begin( 0x44 );

    // 5 MV_GND_0
    // 6 MV_GND_1
    // 7 MV_GND_2
    i2cbusmaster.enable_bus( 0x01 );
    busobj_567_mv_gnd_x.begin( 0x41 );

    // 8 MV_VCC_0
    // 9 MV_VCC_1
    // A MV_VCC_2
    i2cbusmaster.enable_bus( 0x01 );
    busobj_89A_mv_vcc.begin( 0x40 );

    // B MVC_VCC
    i2cbusmaster.enable_bus( 0 );
    busobj_B_mvc_vcc.begin( 0x44 );
    
    // C V_MVC_VCC_0
    i2cbusmaster.enable_bus( 0 );
    busobj_C_v_mvc_vcc_0.begin( 0x41 );
    busobj_C_v_mvc_vcc_0_dac.begin( 0x61 );

    // D V_MVC_VCC_1
    i2cbusmaster.enable_bus( 0 );
    busobj_D_v_mvc_vcc_1.begin( 0x40 );
    busobj_D_v_mvc_vcc_1_dac.begin( 0x60 );

    // E LEDTOGND
    // F NC

};



void busportlib::switchto( uint8_t busid ) {

    switch (busid) {
        // 00
        case BUSPORT_NC:
            i2cbusmaster.disable_bus();
            break;
        // 01
        case BUSPORT_VCC:
            i2cbusmaster.disable_bus();
            break;
        // 02
        case BUSPORT_MVC_LEDTOGND_0:
            i2cbusmaster.enable_bus( 1 );
            break;
        // 03
        case BUSPORT_MVC_LEDTOGND_1:
            i2cbusmaster.enable_bus( 0 );
            break;
        // 04
        case BUSPORT_MVC_GND:
            i2cbusmaster.enable_bus( 1 );
            break;
        // 05
        case BUSPORT_MV_GND_0:
            i2cbusmaster.enable_bus( 1 );
            break;
        // 06
        case BUSPORT_MV_GND_1:
            i2cbusmaster.enable_bus( 1 );
            break;
        // 07
        case BUSPORT_MV_GND_2:
            i2cbusmaster.enable_bus( 1 );
            break;
        // 08
        case BUSPORT_MV_VCC_0:
            i2cbusmaster.enable_bus( 1 );
            break;
        // 09
        case BUSPORT_MV_VCC_1:
            i2cbusmaster.enable_bus( 1 );
            break;
        // 10
        case BUSPORT_MV_VCC_2:
            i2cbusmaster.enable_bus( 1 );
            break;
        // 11
        case BUSPORT_MVC_VCC:
            i2cbusmaster.enable_bus( 0 );
            break;
        // 12
        case BUSPORT_V_MVC_VCC_0:
            i2cbusmaster.enable_bus( 0 );
            break;
        // 13
        case BUSPORT_V_MVC_VCC_1:
            i2cbusmaster.enable_bus( 0 );
            break;
        // 14
        case BUSPORT_LEDTOGND:
            i2cbusmaster.disable_bus();
            break;
        // 15
        case BUSPORT_GND:
            i2cbusmaster.disable_bus();
            break;
    }

}

#endif