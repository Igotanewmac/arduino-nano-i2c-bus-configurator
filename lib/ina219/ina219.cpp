#ifndef INA219_BODY_GUARD
#define INA219_BODY_GUARD

#include <ina219.h>

#include <basetools.h>


ina219::ina219(/* args */) {
}


ina219::ina219( uint8_t i2caddress ) {
    _i2caddress = i2caddress;
}


ina219::~ina219() {
}



// 0 : 019F : 0000000110011111
// 1 : FFFD : 1111111111111101
// 2 : 0002 : 0000000000000010
// 3 : 0000 : 0000000000000000
// 4 : FFFA : 1111111111111010
// 5 : 2000 : 0010000000000000



void ina219::begin() {
    Wire.begin();
    Wire.beginTransmission( _i2caddress );
    Wire.write( 0x00 );
    Wire.write( 0x01 );
    Wire.write( 0x9F );
    Wire.endTransmission();
    Wire.beginTransmission( _i2caddress );
    Wire.write( 0x05 );
    Wire.write( 0x20 );
    Wire.write( 0x00 );
    Wire.endTransmission();
}



void ina219::begin( uint8_t i2caddress ) {
    _i2caddress = i2caddress;
    Wire.begin();
    Wire.beginTransmission( _i2caddress );
    Wire.write( 0x00 );
    Wire.write( 0x01 );
    Wire.write( 0x9F );
    Wire.endTransmission();
    Wire.beginTransmission( _i2caddress );
    Wire.write( 0x05 );
    Wire.write( 0x20 );
    Wire.write( 0x00 );
    Wire.endTransmission();
}










float ina219::getshuntvoltage() {
    int16_t tempvar = 0x0000;
    Wire.beginTransmission( _i2caddress );
    Wire.write( 0x01 );
    Wire.endTransmission();
    Wire.requestFrom( _i2caddress , (uint8_t)2 );
    tempvar |= ( Wire.read() ) << 8;
    tempvar |= Wire.read();
    return tempvar * 0.01 ;
}

float ina219::getbusvoltage() {
    int16_t tempvar = 0x0000;
    Wire.beginTransmission( _i2caddress );
    Wire.write( 0x02 );
    Wire.endTransmission();
    Wire.requestFrom( _i2caddress , (uint8_t)2 );
    tempvar |= ( Wire.read() ) << 8;
    tempvar |= Wire.read();
    return ( ( ( tempvar >> 3 ) * 0.4 ) * 0.01 );
}

float ina219::getpower() {
    int16_t tempvar = 0x0000;
    Wire.beginTransmission( _i2caddress );
    Wire.write( 0x03 );
    Wire.endTransmission();
    Wire.requestFrom( _i2caddress , (uint8_t)2 );
    tempvar |= ( Wire.read() ) << 8;
    tempvar |= Wire.read();
    return (float)tempvar;
}

float ina219::getcurrent() {
    int16_t tempvar = 0x0000;
    Wire.beginTransmission( _i2caddress );
    Wire.write( 0x04 );
    Wire.endTransmission();
    Wire.requestFrom( _i2caddress , (uint8_t)2 );
    tempvar |= ( Wire.read() ) << 8;
    tempvar |= Wire.read();
    return (float)tempvar / 20;
}


void ina219::dumpregisters() {

    uint16_t tempvar;

    for ( uint8_t regnum = 0 ; regnum < 6 ; regnum++ ) {
    
    tempvar = 0;

    Wire.beginTransmission( _i2caddress );
    Wire.write( regnum );
    Wire.endTransmission();
    Wire.requestFrom( _i2caddress , (uint8_t)2 );
    tempvar |= ( Wire.read() << 8 );
    tempvar |= Wire.read();

    Serial.print( regnum );
    Serial.print( " : " );
    showhex( tempvar );
    Serial.print( " : " );
    showbin( tempvar );
    Serial.println();

    }   
    
}












#endif