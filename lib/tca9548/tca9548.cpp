#ifndef TCA9548_BODY_GUARD
#define TCA9548_BODY_GUARD

#include <Arduino.h>

#include <Wire.h>

#include <tca9548.h>

tca9548::tca9548( uint8_t i2caddress ) {

    // save the i2c address for later
    _i2c_address = i2caddress;

}

tca9548::~tca9548() {

    // don't do anything as this object is expected
    // to last the lifectime of the program

}

void tca9548::begin() {

    // turn on i2c
    Wire.begin();

}

uint8_t tca9548::get_bus_states() {

    // request 1 byte from the tca
    Wire.requestFrom( _i2c_address , (uint8_t)1 );

    // return the byte received
    return Wire.read();

}

void tca9548::disable_bus() {

    // say hello to the chip
    Wire.beginTransmission( _i2c_address );

    // write a zero to disable all 8 bits
    Wire.write( 0 );

    // say goodbye to the chip
    Wire.endTransmission();

}

void tca9548::enable_bus( uint8_t i2c_bus_number ) {

    // say hello to the chip
    Wire.beginTransmission( _i2c_address );

    // send a set bit shifted to the right bus column
    Wire.write( 1 << i2c_bus_number );

    // say goodbye to the chip
    Wire.endTransmission();

}

void tca9548::mux_buses( uint8_t i2c_bus_select_bitmask ) {

    // say hello to the chip
    Wire.beginTransmission( _i2c_address );

    // send the raw bitmask and let the user worry about it being right
    Wire.write( i2c_bus_select_bitmask );

    // say goodbye to the chip
    Wire.endTransmission();

}

#endif