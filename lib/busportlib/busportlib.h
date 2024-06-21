#ifndef BUSPORTLIB_HEADER_GUARD
#define BUSPORTLIB_HEADER_GUARD

#include <Arduino.h>

#include <Wire.h>

#include <ina219.h>

#include <ina3221.h>

#include <mcp4725.h>

#include <tca9548.h>

extern tca9548 i2cbusmaster;


// bus objects

// 0 NC
#define BUSPORT_NC 0x00

// 1 VCC
#define BUSPORT_VCC 0x01

// 2 MVC_LEDTOGND_0
#define BUSPORT_MVC_LEDTOGND_0 0x02

// 3 MVC_LEDTOGND_1
#define BUSPORT_MVC_LEDTOGND_1 0x03

// 4 MVC_GND
#define BUSPORT_MVC_GND 0x04

// 5 MV_GND_0
#define BUSPORT_MV_GND_0 0x05

// 6 MV_GND_1
#define BUSPORT_MV_GND_1 0x06

// 7 MV_GND_2
#define BUSPORT_MV_GND_2 0x07

// 8 MV_VCC_0
#define BUSPORT_MV_VCC_0 0x08

// 9 MV_VCC_1
#define BUSPORT_MV_VCC_1 0x09

// A MV_VCC_2
#define BUSPORT_MV_VCC_2 0x0A

// B MVC_VCC
#define BUSPORT_MVC_VCC 0x0B

// C V_MVC_VCC_0
#define BUSPORT_V_MVC_VCC_0 0x0C

// D V_MVC_VCC_1
#define BUSPORT_V_MVC_VCC_1 0x0D

// E LEDTOGND
#define BUSPORT_LEDTOGND 0x0E

// F NC
#define BUSPORT_GND 0x0F

// 0 GND
// 1 VCC
// 2 MVC_LEDTOGND_0
// 3 MVC_LEDTOGND_1
// 4 MVC_GND
// 5 MV_GND_0
// 6 MV_GND_1
// 7 MV_GND_2
// 8 MV_VCC_0
// 9 MV_VCC_1
// A MV_VCC_2
// B MVC_VCC
// C V_MVC_VCC_0
// D V_MVC_VCC_1
// E LEDTOGND
// F NC


class busportlib {

    private:


    public:

        // bus objects

        // 0 GND
        // 1 VCC
        
        // 2 MVC_LEDTOGND_0
        ina219 busobj_2_mvc_ledtognd_0;

        // 3 MVC_LEDTOGND_1
        ina219 busobj_3_mvc_ledtognd_1;

        // 4 MVC_GND
        ina219 busobj_4_mvc_gnd;

        // 5 MV_GND_0
        // 6 MV_GND_1
        // 7 MV_GND_2
        ina3221 busobj_567_mv_gnd_x;

        // 8 MV_VCC_0
        // 9 MV_VCC_1
        // A MV_VCC_2
        ina3221 busobj_89A_mv_vcc;

        // B MVC_VCC
        ina219 busobj_B_mvc_vcc;

        // C V_MVC_VCC_0
        ina219 busobj_C_v_mvc_vcc_0;
        mcp4725 busobj_C_v_mvc_vcc_0_dac;

        // D V_MVC_VCC_1
        ina219 busobj_D_v_mvc_vcc_1;
        mcp4725 busobj_D_v_mvc_vcc_1_dac;
        
        // E LEDTOGND
        // F NC


        busportlib(/* args */);

        ~busportlib();

        void begin();

        void switchto( uint8_t busid );



};




























#endif