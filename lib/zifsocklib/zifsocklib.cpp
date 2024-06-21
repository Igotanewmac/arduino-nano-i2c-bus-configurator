#ifndef ZIFSOCKLIB_BODY_GUARD
#define ZIFSOCKLIB_BODY_GUARD

#include <Arduino.h>

#include <Wire.h>

#include <zifsocklib.h>

#include <basetools.h>


zifsocklib::zifsocklib() {
}


zifsocklib::~zifsocklib() {
}




void zifsocklib::begin( uint8_t* i2caddressarray ) {
    Wire.begin();
    bus_select[0].begin( i2caddressarray[0] );
    bus_select[1].begin( i2caddressarray[1] );
    bus_select[2].begin( i2caddressarray[2] );
    bus_select[3].begin( i2caddressarray[3] );
    bus_enable[0].begin( i2caddressarray[4] );
    bus_enable[1].begin( i2caddressarray[5] );

    bus_enable[0].setbyte(0xFF);
    bus_enable[1].setbyte(0xFF);
    

}






void zifsocklib::pin_to_bus( uint8_t zifsockpinid , uint8_t busid , uint8_t zifsockpinstate ) {


    uint8_t selectorindex;
    uint8_t byteindex;

    // Serial.print( "Setting pin " );
    // Serial.print( zifsockpinid );
    // Serial.print( " to bus " );
    // Serial.println( busid );

    switch (zifsockpinid) {
        case 0x0: { selectorindex = 3; byteindex = 3; break; }
        case 0x1: { selectorindex = 3; byteindex = 2; break; }
        case 0x2: { selectorindex = 2; byteindex = 3; break; }
        case 0x3: { selectorindex = 2; byteindex = 2; break; }
        case 0x4: { selectorindex = 1; byteindex = 3; break; }
        case 0x5: { selectorindex = 1; byteindex = 2; break; }
        case 0x6: { selectorindex = 0; byteindex = 3; break; }
        case 0x7: { selectorindex = 0; byteindex = 2; break; }
        case 0x8: { selectorindex = 0; byteindex = 0; break; }
        case 0x9: { selectorindex = 0; byteindex = 1; break; }
        case 0xA: { selectorindex = 1; byteindex = 0; break; }
        case 0xB: { selectorindex = 1; byteindex = 1; break; }
        case 0xC: { selectorindex = 2; byteindex = 0; break; }
        case 0xD: { selectorindex = 2; byteindex = 1; break; }
        case 0xE: { selectorindex = 3; byteindex = 0; break; }
        case 0xF: { selectorindex = 3; byteindex = 1; break; }
    }

    // Serial.print("Selectorindex = " );
    // Serial.println( selectorindex );
    // Serial.print( "byteindex = " );
    // Serial.println( byteindex );

    uint16_t tempvar = bus_select[ selectorindex ].getlastword();
    // Serial.print( "Rcvd: " );
    // showbin( tempvar );
    // Serial.println();

    // clear the bits
    uint16_t tempbitmask = ~( (uint16_t)( 0xF << ( byteindex * 4 ) ) );
    tempvar &= tempbitmask;

    // Serial.print( "mask: " );
    // showbin( tempbitmask );
    // Serial.println();


    // set the bits
    tempvar |= (uint16_t)( ( busid & 0b1111 ) << ( byteindex * 4 ) );

    bus_select[ selectorindex ].setword( tempvar );
    // Serial.print( "Sent: " );
    // showbin( tempvar );
    // Serial.println();



}









void zifsocklib::disable_pin( uint8_t zifsockpinid ) {
uint8_t selectorindex;
    uint8_t byteindex;

    switch (zifsockpinid) {
        case 0x0: { selectorindex = 1; byteindex = 7; break; }
        case 0x1: { selectorindex = 1; byteindex = 6; break; }
        case 0x2: { selectorindex = 1; byteindex = 5; break; }
        case 0x3: { selectorindex = 1; byteindex = 4; break; }
        case 0x4: { selectorindex = 1; byteindex = 3; break; }
        case 0x5: { selectorindex = 1; byteindex = 2; break; }
        case 0x6: { selectorindex = 1; byteindex = 1; break; }
        case 0x7: { selectorindex = 1; byteindex = 0; break; }
        case 0x8: { selectorindex = 0; byteindex = 0; break; }
        case 0x9: { selectorindex = 0; byteindex = 1; break; }
        case 0xA: { selectorindex = 0; byteindex = 2; break; }
        case 0xB: { selectorindex = 0; byteindex = 3; break; }
        case 0xC: { selectorindex = 0; byteindex = 4; break; }
        case 0xD: { selectorindex = 0; byteindex = 5; break; }
        case 0xE: { selectorindex = 0; byteindex = 6; break; }
        case 0xF: { selectorindex = 0; byteindex = 7; break; }
    }
    
    // Serial.print("Selectorindex = " );
    // Serial.println( selectorindex );
    // Serial.print( "byteindex = " );
    // Serial.println( byteindex );

    uint8_t tempvar = bus_enable[ selectorindex ].getlastbyte();
    // Serial.print( "Rcvd: " );
    // showbin( tempvar );
    // Serial.println();

    // clear the right bit
    uint8_t tempbitmask = (uint8_t)( 1 << byteindex );
    tempvar &= ~tempbitmask;

    // Serial.print( "mask: " );
    // showbin( tempbitmask );
    // Serial.println();

    // now set the right bit
    tempvar |= ( 0b1 << byteindex );

    // send it
    bus_enable[ selectorindex ].setbyte( tempvar );
    // Serial.print( "Sent: " );
    // showbin( tempvar );
    // Serial.println();

}







void zifsocklib::enable_pin( uint8_t zifsockpinid ) {

uint8_t selectorindex;
    uint8_t byteindex;

    switch (zifsockpinid) {
        case 0x0: { selectorindex = 1; byteindex = 7; break; }
        case 0x1: { selectorindex = 1; byteindex = 6; break; }
        case 0x2: { selectorindex = 1; byteindex = 5; break; }
        case 0x3: { selectorindex = 1; byteindex = 4; break; }
        case 0x4: { selectorindex = 1; byteindex = 3; break; }
        case 0x5: { selectorindex = 1; byteindex = 2; break; }
        case 0x6: { selectorindex = 1; byteindex = 1; break; }
        case 0x7: { selectorindex = 1; byteindex = 0; break; }
        case 0x8: { selectorindex = 0; byteindex = 0; break; }
        case 0x9: { selectorindex = 0; byteindex = 1; break; }
        case 0xA: { selectorindex = 0; byteindex = 2; break; }
        case 0xB: { selectorindex = 0; byteindex = 3; break; }
        case 0xC: { selectorindex = 0; byteindex = 4; break; }
        case 0xD: { selectorindex = 0; byteindex = 5; break; }
        case 0xE: { selectorindex = 0; byteindex = 6; break; }
        case 0xF: { selectorindex = 0; byteindex = 7; break; }
    }
    
    // Serial.print("Selectorindex = " );
    // Serial.println( selectorindex );
    // Serial.print( "byteindex = " );
    // Serial.println( byteindex );

    uint8_t tempvar = bus_enable[ selectorindex ].getlastbyte();
    // Serial.print( "Rcvd: " );
    // showbin( tempvar );
    // Serial.println();

    // clear the right bit
    uint8_t tempbitmask = (uint8_t)( 1 << byteindex );
    tempvar &= ~tempbitmask;

    // Serial.print( "mask: " );
    // showbin( tempbitmask );
    // Serial.println();

    // now set the right bit
    //tempvar |= ( 0b1 << byteindex );

    // send it
    bus_enable[ selectorindex ].setbyte( tempvar );
    // Serial.print( "Sent: " );
    // showbin( tempvar );
    // Serial.println();

}








#endif