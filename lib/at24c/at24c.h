#ifndef AT24C_HEADER_GUARD
#define AT24C_HEADER_GUARD


#include <Arduino.h>

#include <Wire.h>







class at24c {
    private:
        
        uint8_t _i2caddress;

    public:
        at24c(/* args */);
        ~at24c();

        void begin( uint8_t i2caddress );

        void _switchbus();

        void setdata( uint16_t address , uint8_t data );
        
        uint8_t getdata( uint16_t address );

        

        void zifset( uint16_t & progptr , uint8_t inputpin , uint8_t outputpin );
        void zifena( uint16_t & progptr , uint8_t inputpin );
        void zifdis( uint16_t & progptr , uint8_t inputpin );
        void zifallena( uint16_t & progptr );
        void zifalldis( uint16_t & progptr );
        void zifreset( uint16_t & progptr );
        

        void gate1to1( uint16_t & progptr , uint8_t inputpin , uint8_t outputpin );
        void gate2to1( uint16_t & progptr , uint8_t inputpina , uint8_t inputpinb , uint8_t outputpin );
        

        void vsweepu( uint16_t & progptr , uint8_t inputpin , uint8_t outputpin , uint16_t stepsize );
        void vsweepd( uint16_t & progptr , uint8_t inputpin , uint8_t outputpin , uint16_t stepsize );
        

        void measure( uint16_t & progptr , uint8_t sensorid );
        void set( uint16_t & progptr , uint8_t snsorid , uint16_t datavalue );        

        void delay( uint16_t & progptr , uint16_t delaytime );

        void endprogram( uint16_t & progptr );



};



















#endif