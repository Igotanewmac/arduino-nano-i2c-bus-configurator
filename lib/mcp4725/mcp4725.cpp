#ifndef MCP4725_BODY_GUARD
#define MCP4725_BODY_GUARD

#include <Arduino.h>

#include <Wire.h>


#include <mcp4725.h>

#include <basetools.h>








mcp4725::mcp4725( uint8_t i2caddress ) {
    _i2c_address = i2caddress;
}
mcp4725::mcp4725() {
}

mcp4725::~mcp4725() {

}



void mcp4725::_fillchipdata() {
    Wire.requestFrom( _i2c_address , (uint8_t)5 );
    _chipdata.statusflags = Wire.read();
    _chipdata.dacdata = (uint16_t)( ( Wire.read() << 8 ) | Wire.read() );
    _chipdata.eepromdata = (uint16_t)( ( Wire.read() << 8 ) | Wire.read() );
}




void mcp4725::begin() {
    Wire.begin();

    // This device supports the general call reset.
    Wire.beginTransmission( 0x00 );
    Wire.write( 0x06 );
    Wire.endTransmission();

    // In theory, this will reset and initialse the chip.

    // now fill our chip data array
    _fillchipdata();
}

void mcp4725::begin( uint8_t i2caddress ) {

    _i2c_address = i2caddress;

    Wire.begin();

    // This device supports the general call reset.
    Wire.beginTransmission( 0x00 );
    Wire.write( 0x06 );
    Wire.endTransmission();

    // In theory, this will reset and initialse the chip.

    // now fill our chip data array
    _fillchipdata();
}



void mcp4725::setvalue( uint16_t dacvalue ) {
    // brief Set the value of the DAC to a 12 bit value.
    // param dacvalue The 12 bit value for the DAC.

    // Write to the chip in fast mode

    _fillchipdata();

    // say hello to the chip
    Wire.beginTransmission( _i2c_address );

    // send over the power down mode flags and the first half of the dacvalue
    Wire.write( (uint8_t)( ( ( dacvalue & 0x0FFF ) >> 8 ) | ( ( _chipdata.statusflags & 0b110 ) << 3 ) ) );

    // send over the second half of the dac value
    Wire.write( (uint8_t)dacvalue );

    // say goodbyt to the chip.
    Wire.endTransmission();

    // fill the cache back up
    _fillchipdata();

}




uint16_t mcp4725::getvalue() {
    // brief Get the current DAC value.
    // return The 12 bit value of the DAC.

    _fillchipdata();

    return _chipdata.dacdata >> 4;


}





void mcp4725::setpowerdownmode( uint8_t powerdownmode ) {
    // brief Set the power down mode bits in the status register to control a resistor to gnd pn the output.
    // param powerdownmode 0b00 Normal Operation, 0b01 = 1k, 0b10 = 100k , 0b11 = 500k.

    _fillchipdata();

    Wire.beginTransmission( _i2c_address );

    Wire.write( (uint8_t)( (_chipdata.dacdata >> 12 ) | ( powerdownmode << 4 ) ) );
    Wire.write( (uint8_t)(_chipdata.dacdata >> 4 ) );

    Wire.endTransmission();


}




uint8_t mcp4725::getpowerdownmode() {
    // brief Gets the power down mode bits in the status register.
    // return 0b00 = Normal operation, 0b01 = 1k, 0b10 = 100k, 0b11 = 500k.

    _fillchipdata();

    return ( ( _chipdata.statusflags >> 1 ) & 0b11 );
}








/// @brief Sets the default value stored in the eeprom.
/// @param dacvalue The 12 bit value to store.
void mcp4725::seteepromvalue( uint16_t dacvalue ) {

    _fillchipdata();

    uint8_t commandbyte = ( ( _chipdata.statusflags & 0b00000110 ) | 0b01100000 );

    uint8_t dataword = ( dacvalue << 4 );

    Wire.beginTransmission( _i2c_address );
    Wire.write( commandbyte );
    Wire.write( (uint8_t)( dataword >> 8 ) );
    Wire.write( (uint8_t)dataword );
    Wire.write( commandbyte );
    Wire.write( (uint8_t)( dataword >> 8 ) );
    Wire.write( (uint8_t)dataword );
    Wire.endTransmission();

    // wait right here for the chip to finish its write.
    // which can take up to half a second, from bitter experience.
    _fillchipdata();
    while ( !( _chipdata.statusflags & 0b10000000 ) ) {
        _fillchipdata();
    }



}

/// @brief Gets the default value stored in the eeprom.
/// @return Tje 12 bit value to store.
uint16_t mcp4725::geteepromvalue() {

    _fillchipdata();
    
    return ( _chipdata.eepromdata & 0x0FFF );
}



















#endif