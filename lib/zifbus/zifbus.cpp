#ifndef ZIFBUS_BODY_GUARD
#define ZIFBUS_BODY_GUARD



#include <zifbus.h>





zifbus::zifbus(/* args */) {
}

zifbus::~zifbus() {
}


void zifbus::begin( uint8_t* i2caddresses ) {
    
    Wire.begin();

    mysystembusobj.switchbus( SYSTEMBUS_BUSID_ZIFBUS );

    mypcf8575obj[0].begin( i2caddresses[0] );
    mypcf8575obj[1].begin( i2caddresses[1] );
    mypcf8575obj[2].begin( i2caddresses[2] );
    mypcf8575obj[3].begin( i2caddresses[3] );

    mypcf8574obj[0].begin( i2caddresses[4] );
    mypcf8574obj[1].begin( i2caddresses[5] );

    disableall();

}




void zifbus::reset() {
    for ( uint8_t loopcounter = 0 ; loopcounter < 16 ; loopcounter++ ) {
        disable(loopcounter);
        pin2bus( loopcounter , 0 );
    }
}




void zifbus::pin2bus( uint8_t pinid , uint8_t busid ) {
    
    mysystembusobj.switchbus( SYSTEMBUS_BUSID_ZIFBUS );

    uint8_t selectorindex;
    uint8_t byteindex;

    // Serial.print( "Setting pin " );
    // Serial.print( pinid );
    // Serial.print( " to bus " );
    // Serial.println( busid );

    switch (pinid) {
        case 0: { selectorindex = 3; byteindex = 3; break; }
        case 1: { selectorindex = 3; byteindex = 2; break; }
        case 2: { selectorindex = 2; byteindex = 3; break; }
        case 3: { selectorindex = 2; byteindex = 2; break; }
        case 4: { selectorindex = 1; byteindex = 3; break; }
        case 5: { selectorindex = 1; byteindex = 2; break; }
        case 6: { selectorindex = 0; byteindex = 2; break; }
        case 7: { selectorindex = 0; byteindex = 3; break; }
        case 8: { selectorindex = 0; byteindex = 1; break; }
        case 9: { selectorindex = 0; byteindex = 0; break; }
        case 10: { selectorindex = 1; byteindex = 1; break; }
        case 11: { selectorindex = 1; byteindex = 0; break; }
        case 12: { selectorindex = 2; byteindex = 1; break; }
        case 13: { selectorindex = 2; byteindex = 0; break; }
        case 14: { selectorindex = 3; byteindex = 1; break; }
        case 15: { selectorindex = 3; byteindex = 0; break; }
    }

    // Serial.print("Selectorindex = " );
    // Serial.println( selectorindex );
    // Serial.print( "byteindex = " );
    // Serial.println( byteindex );

    uint16_t tempvar = mypcf8575obj[ selectorindex ].getlastword();
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

    mypcf8575obj[ selectorindex ].setword( tempvar );
    // Serial.print( "Sent: " );
    // showbin( tempvar );
    // Serial.println();
}

void zifbus::enable( uint8_t pinid ) {


    mysystembusobj.switchbus( SYSTEMBUS_BUSID_ZIFBUS );

    uint8_t selectorindex;
    uint8_t byteindex;

    switch (pinid) {
        case 0x0: { selectorindex = 1; byteindex = 0; break; }
        case 0x1: { selectorindex = 1; byteindex = 1; break; }
        case 0x2: { selectorindex = 1; byteindex = 2; break; }
        case 0x3: { selectorindex = 1; byteindex = 3; break; }
        case 0x4: { selectorindex = 1; byteindex = 4; break; }
        case 0x5: { selectorindex = 1; byteindex = 5; break; }
        case 0x6: { selectorindex = 1; byteindex = 7; break; }
        case 0x7: { selectorindex = 1; byteindex = 6; break; }
        case 0x8: { selectorindex = 0; byteindex = 7; break; }
        case 0x9: { selectorindex = 0; byteindex = 6; break; }
        case 0xA: { selectorindex = 0; byteindex = 5; break; }
        case 0xB: { selectorindex = 0; byteindex = 4; break; }
        case 0xC: { selectorindex = 0; byteindex = 3; break; }
        case 0xD: { selectorindex = 0; byteindex = 2; break; }
        case 0xE: { selectorindex = 0; byteindex = 1; break; }
        case 0xF: { selectorindex = 0; byteindex = 0; break; }
    }
    
    // Serial.print("Selectorindex = " );
    // Serial.println( selectorindex );
    // Serial.print( "byteindex = " );
    // Serial.println( byteindex );

    uint8_t tempvar = mypcf8574obj[ selectorindex ].getlastbyte();
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
    mypcf8574obj[ selectorindex ].setbyte( tempvar );
    // Serial.print( "Sent: " );
    // showbin( tempvar );
    // Serial.println();

}









void zifbus::disable( uint8_t pinid ) {

    mysystembusobj.switchbus( SYSTEMBUS_BUSID_ZIFBUS );
    
    uint8_t selectorindex;
    uint8_t byteindex;

    switch (pinid) {
        case 0x0: { selectorindex = 1; byteindex = 0; break; }
        case 0x1: { selectorindex = 1; byteindex = 1; break; }
        case 0x2: { selectorindex = 1; byteindex = 2; break; }
        case 0x3: { selectorindex = 1; byteindex = 3; break; }
        case 0x4: { selectorindex = 1; byteindex = 4; break; }
        case 0x5: { selectorindex = 1; byteindex = 5; break; }
        case 0x6: { selectorindex = 1; byteindex = 7; break; }
        case 0x7: { selectorindex = 1; byteindex = 6; break; }
        case 0x8: { selectorindex = 0; byteindex = 7; break; }
        case 0x9: { selectorindex = 0; byteindex = 6; break; }
        case 0xA: { selectorindex = 0; byteindex = 5; break; }
        case 0xB: { selectorindex = 0; byteindex = 4; break; }
        case 0xC: { selectorindex = 0; byteindex = 3; break; }
        case 0xD: { selectorindex = 0; byteindex = 2; break; }
        case 0xE: { selectorindex = 0; byteindex = 1; break; }
        case 0xF: { selectorindex = 0; byteindex = 0; break; }
    }
    
    // Serial.print("Selectorindex = " );
    // Serial.println( selectorindex );
    // Serial.print( "byteindex = " );
    // Serial.println( byteindex );

    uint8_t tempvar = mypcf8574obj[ selectorindex ].getlastbyte();
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
    mypcf8574obj[ selectorindex ].setbyte( tempvar );
    // Serial.print( "Sent: " );
    // showbin( tempvar );
    // Serial.println();
}

void zifbus::enableall() {
    mysystembusobj.switchbus( SYSTEMBUS_BUSID_ZIFBUS );
    enable(0);
    enable(1);
    enable(2);
    enable(3);
    enable(4);
    enable(5);
    enable(6);
    enable(7);
    enable(8);
    enable(9);
    enable(10);
    enable(11);
    enable(12);
    enable(13);
    enable(14);
    enable(15);
}

void zifbus::disableall() {
    mysystembusobj.switchbus( SYSTEMBUS_BUSID_ZIFBUS );
    disable(0);
    disable(1);
    disable(2);
    disable(3);
    disable(4);
    disable(5);
    disable(6);
    disable(7);
    disable(8);
    disable(9);
    disable(10);
    disable(11);
    disable(12);
    disable(13);
    disable(14);
    disable(15);
}











#endif