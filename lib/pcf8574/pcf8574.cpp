#ifndef PCF8574_BODY_GUARD
#define PCF8574_BODY_GUARD

#include <Arduino.h>
#include <Wire.h>


#include <pcf8574.h>





pcf8574::pcf8574( uint8_t i2caddress ) {
    _i2caddress = i2caddress;
}

pcf8574::pcf8574() {
    _i2caddress = 0;
}

pcf8574::~pcf8574() {

}


void pcf8574::begin() {
    Wire.begin();
    setbyte( 0x00 );
}

void pcf8574::begin( uint8_t i2caddress ) {
    _i2caddress = i2caddress;
    Wire.begin();
    setbyte( 0x00 );
}




uint8_t pcf8574::getbyte() {
    Wire.requestFrom( _i2caddress , (uint8_t)1 );
    return Wire.read();
}



void pcf8574::setbyte( uint8_t databyte ) {
    _lastbytesent = databyte;
    Wire.beginTransmission( _i2caddress );
    Wire.write( databyte );
    Wire.endTransmission();
}


uint8_t pcf8574::getlastbyte() {
    return _lastbytesent;
}




#endif