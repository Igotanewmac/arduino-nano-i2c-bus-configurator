#ifndef ZIFSOCKLIB_HEADER_GUARD
#define ZIFSOCKLIB_HEADER_GUARD


#include <Arduino.h>

#include <pcf8574.h>

#include <pcf8575.h>






class zifsocklib {

    private:

        pcf8574 bus_enable[2];
        pcf8575 bus_select[4];

    public:

        // the constructor
        zifsocklib();

        // the destructor
        ~zifsocklib();

        // initialise the object
        void begin( uint8_t* i2caddressarray );



        void pin_to_bus( uint8_t zifsockpinid , uint8_t busid , uint8_t zifsockpinstate = 0x00 );

        void enable_pin( uint8_t zifsockpinid );

        void disable_pin( uint8_t zifsockpinid );

};















#endif