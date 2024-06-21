#ifndef INA219_HEADER_GUARD
#define INA219_HEADER_GUARD

#include <Arduino.h>

#include <Wire.h>


// 0 : 019F : 0000000110011111
// 1 : FFFD : 1111111111111101
// 2 : 0002 : 0000000000000010
// 3 : 0000 : 0000000000000000
// 4 : FFFA : 1111111111111010
// 5 : 2000 : 0010000000000000




class ina219 {
    private:
        
        uint8_t _i2caddress;

    public:

        ina219(/* args */);
        ina219( uint8_t i2caddress );

        ~ina219();

        void begin();
        void begin( uint8_t i2caddress );

        void dumpregisters();

        void seti2caddress( uint8_t i2caddress );
        
        float getshuntvoltage();
        float getbusvoltage();
        float getpower();
        float getcurrent();



};





















#endif