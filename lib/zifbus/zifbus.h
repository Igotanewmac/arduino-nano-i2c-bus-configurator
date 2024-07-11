#ifndef ZIFBUS_HEADER_GUARD
#define ZIFBUS_HEADER_GUARD


#include <Arduino.h>

#include <Wire.h>

#include <systembus.h>
extern systembus mysystembusobj;

#include <basetools.h>

#include <pcf8574.h>

#include <pcf8575.h>



class zifbus {
        
    private:
        
        pcf8574 mypcf8574obj[2];

        pcf8575 mypcf8575obj[4];


    public:

        zifbus();

        ~zifbus();

        void begin( uint8_t* i2caddress );

        void pin2bus( uint8_t pinid , uint8_t busid );

        void enable( uint8_t pinid );

        void disable( uint8_t pinid );

        void enableall();

        void disableall();


};
















#endif