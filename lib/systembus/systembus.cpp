#ifndef SYSTEMBUS_BODY_GUARD
#define SYSTEMBUS_BODY_GUARD


#include <systembus.h>



systembus::systembus(/* args */) {

}

systembus::~systembus() {

}


void systembus::begin( uint8_t* addresses ) {

    // turn on wire lib
    Wire.begin();

    mytca9548obj.begin( addresses[0] );

    mypcf8574obj.begin( addresses[1] );

}






void systembus::switchbus( uint8_t busid ) {
    // Switch to the given i2c bus
    mytca9548obj.enable_bus( busid );
}


void systembus::setled( uint8_t statebyte ) {
    switchbus( SYSTEMBUS_BUSID_SYSTEMBUS );
    mypcf8574obj.setbyte( statebyte );
}






#endif