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
#define SYSTEMBUS_BUSID_SENSORBUS_0 3
#define SYSTEMBUS_BUSID_SENSORBUS_1 2

class systembus {

    private:
        /* data */

        // tca object
        tca9548 mytca9548obj;

        // pcf object
        pcf8574 mypcf8574obj;
        

        
    public:
        
        
        systembus();
        ~systembus();
        
        /// @brief Initialise all the things.
        void begin( uint8_t* i2caddresses );

        /// @brief Switch to the given i2c bus
        void switchbus( uint8_t busid );

        /// @brief 
        /// @param lednum 
        /// @param state 
        void setled( uint8_t statebyte );


};













#endif