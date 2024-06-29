#ifndef SYSTEMBUS_BODY_GUARD
#define SYSTEMBUS_BODY_GUARD


#include <systembus.h>



systembus::systembus(/* args */) {

}

systembus::~systembus() {

}


void systembus::seti2caddress_tca9548( uint8_t i2caddress ) {
    _i2caddress_tca9548 = i2caddress;
}




void systembus::begin() {

    // turn on wire lib
    Wire.begin();

    // initialise the TCA
    mytca9548obj.seti2caddress( _i2caddress_tca9548 );

    // begin for it
    mytca9548obj.begin();

    // switch to systembus at startup
    mytca9548obj.enable_bus( SYSTEMBUS_BUSID_SYSTEMBUS );

}






void systembus::switchbus( uint8_t busid ) {
    // Switch to the given i2c bus
    mytca9548obj.enable_bus( busid );
}


#endif