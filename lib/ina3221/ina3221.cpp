


#ifndef INA3221_BODY_GUARD
#define INA3221_BODY_GUARD

#include <ina3221.h>




ina3221::ina3221( uint8_t i2c_address ) {

    _i2c_address = i2c_address;

}



ina3221::ina3221() {
}


ina3221::~ina3221()
{
}






void ina3221::begin() {

    // make sure Wire.h has been set up?
    Wire.begin();

}



void ina3221::begin( uint8_t i2c_address )  {

    // make sure Wire.h has been set up?
    Wire.begin();

    _i2c_address = i2c_address;

}



uint16_t ina3221::_get_raw_register( uint8_t register_number ) {

    uint16_t register_content = 0x0000;

    Wire.beginTransmission( _i2c_address );
    Wire.write( register_number );
    Wire.endTransmission();
    
    Wire.requestFrom( (uint8_t)_i2c_address , (uint8_t)2 );
    register_content |= ( Wire.read() << 8 );
    register_content |= Wire.read();

    return register_content;


}


void ina3221::_set_raw_register( uint8_t register_number , uint16_t register_content ) {

    Wire.beginTransmission( _i2c_address );
    Wire.write( register_number );
    Wire.write( ( ( register_content >> 8 ) & 0xFF ) );
    Wire.write( ( register_content & 0xFF ) );
    Wire.endTransmission();

}




void ina3221::reset() {

    _set_raw_register( 0 , _get_raw_register( INA3221_REGISTER_CONFIG ) | 0b1000000000000000 );

}




void ina3221::set_channel_state( uint8_t channel_number , uint8_t channel_state ) {

    uint16_t config_register = _get_raw_register( INA3221_REGISTER_CONFIG );

    // clear the bit
    config_register &= ~( 0b0001000000000000 << channel_number );

    // set the bit if required
    if ( channel_state ) {
        config_register |= ( 0b0001000000000000 << channel_number );
    }

    _set_raw_register( 0 , config_register );

}


uint8_t ina3221::get_channel_state( uint8_t channel_number ) {

    return (uint8_t)( ( ( _get_raw_register( INA3221_REGISTER_CONFIG ) >> channel_number ) >> 12 ) & 0b1 );

}






void ina3221::set_averaging_mode( uint8_t averaging_mode ) {

    uint16_t config_register = _get_raw_register( INA3221_REGISTER_CONFIG );

    // clear the relevant bits
    config_register &= ~INA3221_CONFIG_BITMASK_AVG;

    // set the relevant bits
    config_register |= ( ( averaging_mode & 0b111 ) << 9 );

    // push them back to the chip
    _set_raw_register( INA3221_REGISTER_CONFIG , config_register );

}

uint8_t ina3221::get_averaging_mode() {

    return ( ( _get_raw_register( INA3221_REGISTER_CONFIG ) >> 9 ) & 0b111 );
}









void ina3221::set_vbus_conversion_time( uint8_t vbus_conversion_time ) {

    uint16_t config_register = _get_raw_register( INA3221_REGISTER_CONFIG );

    // clear the relevant bits
    config_register &= ~INA3221_CONFIG_BITMASK_AVG ;

    // set the relevant bits
    config_register |= ( ( vbus_conversion_time & 0b111 ) << 6 );

    // push them back to the chip
    _set_raw_register( INA3221_REGISTER_CONFIG , config_register );

}

uint8_t ina3221::get_vbus_conversion_time() {
    return ( ( _get_raw_register( INA3221_REGISTER_CONFIG ) >> 6 ) & 0b111 );
}






void ina3221::set_shunt_conversion_time( uint8_t shunt_conversion_time ) {
    
    uint16_t config_register = _get_raw_register( INA3221_REGISTER_CONFIG );

    // clear the relevant bits
    config_register &= ~INA3221_CONFIG_BITMASK_SHNT;

    // set the relevant bits
    config_register |= ( ( shunt_conversion_time & 0b111 ) << 3 );

    // push them back to the chip
    _set_raw_register( INA3221_REGISTER_CONFIG , config_register );

}

uint8_t ina3221::get_shunt_conversion_time() {
    return ( ( _get_raw_register( INA3221_REGISTER_CONFIG ) >> 3 ) & 0b111 );
}










void ina3221::set_operating_mode( uint8_t operating_mode ) {
    uint16_t config_register = _get_raw_register( INA3221_REGISTER_CONFIG );

    // clear the relevant bits
    config_register &= ~INA3221_CONFIG_BITMASK_MODE;

    // set the relevant bits
    config_register |= ( operating_mode & 0b111 );

    // push them back to the chip
    _set_raw_register( INA3221_REGISTER_CONFIG , config_register );
}





uint8_t ina3221::get_operating_mode() {
    return ( _get_raw_register( INA3221_REGISTER_CONFIG ) & 0b111 );
}







float ina3221::get_shunt_voltage( uint8_t channel_number ) {
    return (float)_get_raw_register( ( channel_number + channel_number ) + 1 ) / 1000;
}




float ina3221::get_bus_voltage( uint8_t channel_number ) {
    return ( _get_raw_register( ( channel_number + channel_number ) + 2 ) >> 3) * 0.008;
}









void ina3221::set_channel_critical_alert_limit( uint8_t channel_number , uint16_t critical_alert_value ) {
    _set_raw_register( ( ( channel_number + channel_number ) + 7 ) , ( critical_alert_value << 3 ) );
}




uint16_t ina3221::get_channel_critical_alert_limit( uint8_t channel_number ) {
    return ( _get_raw_register( ( channel_number + channel_number ) + 7 ) >> 3 );
}







void ina3221::set_channel_warning_alert_limit( uint8_t channel_number , uint16_t warning_alert_value ) {
    _set_raw_register( ( ( channel_number + channel_number ) + 8 ) , ( warning_alert_value << 3 ) );
}



uint16_t ina3221::get_channel_warning_alert_limit( uint8_t channel_number ) {
    return ( _get_raw_register( ( channel_number + channel_number ) + 8 ) >> 3 );
}




float ina3221::get_shunt_voltage_sum() {
    return _get_raw_register( INA3221_REGISTER_SHUNT_VOLTAGE_SUM ) >> 1;
}



void ina3221::set_shunt_voltage_sum_limit( uint16_t voltage_sum_limit ) {
    _set_raw_register( INA3221_REGISTER_SHUNT_VOLTAGE_SUM_LIMIT , voltage_sum_limit << 1 );
}




uint16_t ina3221::get_shunt_voltage_sum_limit() {
    return ( _get_raw_register( INA3221_REGISTER_SHUNT_VOLTAGE_SUM_LIMIT ) >> 1 );
}




void ina3221::set_power_valid_upper_limit( uint16_t power_valid_upper_limit ) {
    _set_raw_register( INA3221_REGISTER_PV_ULIMIT , power_valid_upper_limit << 3 );
}

uint16_t ina3221::get_power_valid_upper_limit() {
    return ( _get_raw_register( INA3221_REGISTER_PV_ULIMIT ) >> 3 );
}



void ina3221::set_power_valid_lower_limit( uint16_t power_valid_lower_limit ) {
    _set_raw_register( INA3221_REGISTER_PV_LLIMIT , power_valid_lower_limit << 3 );
}

uint16_t ina3221::get_power_valid_lower_limit() {
    return( _get_raw_register( INA3221_REGISTER_PV_LLIMIT ) >> 3 );
}







#endif



