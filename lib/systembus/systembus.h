#ifndef SYSTEMBUS_HEADER_GUARD
#define SYSTEMBUS_HEADER_GUARD

// arduino library for arduino functions
#include <Arduino.h>

// Arduino Wire library for i2c
#include <Wire.h>

// TCA9548 Library for bus switcher
#include <tca9548.h>


// pcf library for indicators
#include <pcf8574.h>



// system bus library

// responsible for :-
// TCA bus switcher
// Clock
// Storage
// PCF LED Bank





#define SYSTEMBUS_BUSID_SYSTEMBUS 0
#define SYSTEMBUS_BUSID_ZIFBUS 1
#define SYSTEMBUS_BUSID_SENSORBUS_0 2
#define SYSTEMBUS_BUSID_SENSORBUS_1 3

#define I2C_ADDRESS_TCA9548 0x70


class systembus {

    private:
        /* data */

        // tca object
        tca9548 mytca9548obj;

        // pcf object
        pcf8574 mypcf8574obj;
        

        uint8_t _i2caddress_tca9548 = I2C_ADDRESS_TCA9548;


    public:
        systembus(/* args */);
        ~systembus();
        
        /// @brief Initialise all the things.
        void begin();

        // address setters
        void seti2caddress_tca9548( uint8_t i2caddress = I2C_ADDRESS_TCA9548 );

        /// @brief Switch to the given i2c bus
        void switchbus( uint8_t busid );

        void seti2caddress_pcf8574( uint8_t i2caddress );

};













#endif