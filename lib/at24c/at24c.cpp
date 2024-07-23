#ifndef AT24C_BODY_GUARD
#define AT24C_BODY_GUARD


#include <at24c.h>



at24c::at24c(/* args */) {
}

at24c::~at24c() {
}



void at24c::begin( uint8_t i2caddress ) {
    _i2caddress = i2caddress;
}




void at24c::_switchbus() {
    Wire.beginTransmission(0x20);
    Wire.write(0x00);
    Wire.endTransmission();
}





void at24c::setdata( uint16_t address , uint8_t data ) {
    Wire.beginTransmission( _i2caddress );
    Wire.write( ( address & 0xFF00 ) >> 8 );
    Wire.write( address & 0xFF );
    Wire.write( data );
    Wire.endTransmission();
    int8_t error = 1;
    while (error != 0 ) {
        Wire.beginTransmission(_i2caddress);
        error = Wire.endTransmission();
    }
}






uint8_t at24c::getdata( uint16_t address ) {
    Wire.beginTransmission( _i2caddress );
    Wire.write( ( address & 0xFF00 ) >> 8 );
    Wire.write( address & 0xFF );
    Wire.endTransmission();
    Wire.requestFrom( _i2caddress , (uint8_t)1 );
    return Wire.read();
}






void at24c::zifset( uint16_t & progptr , uint8_t inputpin , uint8_t outputpin ) {

    setdata( progptr++ , 0x01 ); // zifset command
    setdata( progptr++ , inputpin );
    setdata( progptr++ , outputpin );
    
}




void at24c::zifena( uint16_t & progptr , uint8_t inputpin ) {

    setdata( progptr++ , 0x02 ); // zifena command
    setdata( progptr++ , inputpin );

}



void at24c::zifdis( uint16_t & progptr , uint8_t inputpin ) {
    
    setdata( progptr++ , 0x03 );// zifdis command
    setdata( progptr++ , inputpin );

}

void at24c::zifallena( uint16_t & progptr ) {

    setdata( progptr++ , 0x04 );

}


void at24c::zifalldis( uint16_t & progptr ) {
    
    setdata( progptr++ , 0x05 );

}

void at24c::zifreset( uint16_t & progptr ) {
    
    setdata( progptr++ , 0x06 );

}


void at24c::gate1to1( uint16_t & progptr , uint8_t inputpin , uint8_t outputpin ) {
    
    setdata( progptr++ , 0x07 );
    setdata( progptr++ , inputpin );
    setdata( progptr++ , outputpin );
}

void at24c::gate2to1( uint16_t & progptr , uint8_t inputpina , uint8_t inputpinb , uint8_t outputpin ) {
    
    setdata( progptr++ , 0x08 );
    setdata( progptr++ , inputpina );
    setdata( progptr++ , inputpinb );
    setdata( progptr++ , outputpin );
    
}


void at24c::vsweepu( uint16_t & progptr , uint8_t inputpin , uint8_t outputpin , uint16_t stepsize ) {

    setdata( progptr++ , 0x09 );
    setdata( progptr++ , inputpin );
    setdata( progptr++ , outputpin );
    setdata( progptr++ , stepsize >> 8 );
    setdata( progptr++ , stepsize & 0xFF );
    

}

void at24c::vsweepd( uint16_t & progptr , uint8_t inputpin , uint8_t outputpin , uint16_t stepsize ) {

    setdata( progptr++ , 0x0A );
    setdata( progptr++ , inputpin );
    setdata( progptr++ , outputpin );
    setdata( progptr++ , stepsize >> 8 );
    setdata( progptr++ , stepsize & 0xFF );

}


void at24c::measure( uint16_t & progptr , uint8_t sensorid ) {

    setdata( progptr++ , 0x0B );
    setdata( progptr++ , sensorid );

}

void at24c::set( uint16_t & progptr , uint8_t sensorid , uint16_t datavalue ) {

    setdata( progptr++ , 0x0C );
    setdata( progptr++ , sensorid );
    setdata( progptr++ , datavalue >> 8 );
    setdata( progptr++ , datavalue & 0xFF );

}

void at24c::delay( uint16_t & progptr , uint16_t delaytime ) {
    setdata( progptr++ , 0x0D );
    setdata( progptr++ , delaytime >> 8 );
    setdata( progptr++ , delaytime & 0xFF );
}


void at24c::endprogram( uint16_t & progptr  ) {

    setdata( progptr++ , 0xFF );
    
}




#endif