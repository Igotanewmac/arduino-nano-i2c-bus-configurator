#ifndef PCF8575_BODY_GUARD
#define PCF8575_BODY_GUARD

#include <Arduino.h>
#include <Wire.h>

#include <pcf8575.h>


pcf8575::pcf8575( uint8_t i2caddress) {
    _i2c_address = i2caddress;
}


pcf8575::~pcf8575() {

}



void pcf8575::begin( uint8_t i2caddress) {
    if ( i2caddress != 0x80 ) {
        _i2c_address = i2caddress;
    }
    setword( 0x0000 );
}



uint16_t pcf8575::getword() {
    Wire.requestFrom( _i2c_address , (uint8_t)2 );
    return ( Wire.read() | ( Wire.read() << 8 ) );
}


void pcf8575::setword( uint16_t dataword ) {
    _lastdataword = dataword;
    Wire.beginTransmission( _i2c_address );
    Wire.write( (uint8_t)dataword );
    Wire.write( (uint8_t)( dataword >> 8 ) );
    Wire.endTransmission();
}


uint16_t pcf8575::getlastword() {
    return _lastdataword;
}



#endif