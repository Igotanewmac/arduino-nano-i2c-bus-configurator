

#ifndef INA3221_HEADER_GUARD
#define INA3221_HEADER_GUARD

#include <Arduino.h>

#include <Wire.h>







  // set averaging mode
  // 000 = 1 (default)
  // 001 = 4
  // 010 = 16
  // 011 = 64
  // 100 = 128
  // 101 = 256
  // 110 = 512
  // 111 = 1024

  // bus voltage conversion time
  // 000 = 140 us
  // 001 = 204 us
  // 010 = 332 us
  // 011 = 588 us
  // 100 = 1.1 ms (default)
  // 101 = 2.116 ms
  // 110 = 4.156 ms
  // 111 = 8.244 ms




typedef enum : uint8_t {

    INA3221_REGISTER_CONFIG = 0x00 ,
    INA3221_REGISTER_SHUNT_VOLTAGE_SUM = 0x0D ,
    INA3221_REGISTER_SHUNT_VOLTAGE_SUM_LIMIT = 0x0E ,
    INA3221_REGISTER_PV_ULIMIT = 0x10 ,
    INA3221_REGISTER_PV_LLIMIT = 0x11 ,
    INA3221_REGISTER_MASK_EN = 0x0F

} ina3221_register_number;



typedef enum : uint16_t {

    INA3221_CONFIG_BITMASK_RST  = 0b1000000000000000 ,
    INA3221_CONFIG_BITMASK_CHAN = 0b0111000000000000 ,
    INA3221_CONFIG_BITMASK_AVG  = 0b0000111000000000 ,
    INA3221_CONFIG_BITMASK_BUS  = 0b0000000111000000 ,
    INA3221_CONFIG_BITMASK_SHNT = 0b0000000000111000 ,
    INA3221_CONFIG_BITMASK_MODE = 0b0000000000000111

} ina3221_config_bitmask;



typedef enum : uint16_t {

    INA3221_ALERT_FLAGS_BITMASK_SCC  = 0b0111000000000000 , 
    INA3221_ALERT_FLAGS_BITMASK_WEN  = 0b0000100000000000 , 
    INA3221_ALERT_FLAGS_BITMASK_CEN  = 0b0000010000000000 , 
    INA3221_ALERT_FLAGS_BITMASK_CF   = 0b0000001110000000 , 
    INA3221_ALERT_FLAGS_BITMASK_SF   = 0b0000000001000000 , 
    INA3221_ALERT_FLAGS_BITMASK_WF   = 0b0000000000111000 , 
    INA3221_ALERT_FLAGS_BITMASK_PVF  = 0b0000000000000100 , 
    INA3221_ALERT_FLAGS_BITMASK_TCV  = 0b0000000000000010 , 
    INA3221_ALERT_FLAGS_BITMASK_CVRF = 0b0000000000000001

} ina3221_alert_flags_bitmask;


















class ina3221
{

    private:
        /* data */
        uint8_t _i2c_address;


    public:
        

        // setup routines


        /// @brief Create the object
        /// @param new_i2c_address the i2c address of the device
        ina3221( uint8_t i2c_address );
        ina3221();
        ~ina3221();

        /// @brief Initialise the i2c connection
        void begin();
        void begin( uint8_t i2c_address );


        // utility routines



        /// @brief Get the 16 bit value in a register
        /// @param register_number The number of the register to return
        /// @return The register contents
        uint16_t _get_raw_register( uint8_t register_number );

        /// @brief Set the 16 bit value in a register
        /// @param register_number The number of the register to set
        /// @param register_content The content to set the register to
        void _set_raw_register( uint8_t register_number , uint16_t register_content );


        // configuration routines


        /// @brief Triggers a soft reset of the chip.
        void reset();

        /// @brief Set the channel enable state.
        /// @param channel_number The number of the channel to set. 0 - 2.
        /// @param channel_state The state pf the channel. 0 is off, 1 is on.
        void set_channel_state( uint8_t channel_number , uint8_t channel_state );

        /// @brief Get the channel enable state.
        /// @param channel_number The number of the channel to check. 0-2.
        /// @return The state of the channel.  0 is off, 1 is on.
        uint8_t get_channel_state( uint8_t channel_number );

        /// @brief Set the averaging mode.
        /// @param averaging_mode The bitmask to set the averaging mode. 0b000 - 0b111.
        void set_averaging_mode( uint8_t averaging_mode );

        /// @brief Gets the averaging mode.
        /// @return The bitmask for the current averaging mode. 0b000 - 0b111
        uint8_t get_averaging_mode();

        /// @brief Set the bus voltage conversion time.
        /// @param vbus_conversion_time The bitmask of the conversion time to use. 0b000 - 0b111.
        void set_vbus_conversion_time( uint8_t vbus_conversion_time );

        /// @brief Gets the bus voltage conversion time.
        /// @return The bitmask of the bus voltage conversion time.  0b000 - 0b111.
        uint8_t get_vbus_conversion_time();

        /// @brief Set the shunt voltage conversion time.
        /// @param shunt_conversion_time The bitmask for the shunt voltage conversion time. 0b000 - 0b111.
        void set_shunt_conversion_time( uint8_t shunt_conversion_time );

        /// @brief Get the shunt voltage conversion time.
        /// @return The bitmask for the shuny voltage conversion time. 0b000 - 0b111.
        uint8_t get_shunt_conversion_time();

        /// @brief Set the operating mode bitmask.
        /// @param operating_mode The operating mode bitmask. 0b000 - 0b111
        void set_operating_mode( uint8_t operating_mode );

        /// @brief Get the operating mode bitmask
        /// @return The operating mode bitmask
        uint8_t get_operating_mode();





        // measurement


        /// @brief Get the current shunt voltage value.
        /// @param channel_number The channel to return the value for.  0 - 2.
        /// @return The value of the channels shunt register.
        float get_shunt_voltage( uint8_t channel_number );


        /// @brief Get the current bus voltage value.
        /// @param channel_number The channel to return the value for.  0 - 2.
        /// @return The value of the channels bus register.
        float get_bus_voltage( uint8_t channel_number );





        /// @brief Set a channels critical alert limit value.
        /// @param channel_number The channel to set.
        /// @param critical_alert_value The value to set for the critical alert limit
        void set_channel_critical_alert_limit( uint8_t channel_number , uint16_t critical_alert_value );

        /// @brief Get a channels critical alert limit value.
        /// @param channel_number The channel to get.
        /// @return The critical alert value for this channel.
        uint16_t get_channel_critical_alert_limit( uint8_t channel_number );



        /// @brief Set the channels warning alert limit value.
        /// @param channel_number The channel to set.
        /// @param warning_alert_value The warning alert value for this channel
        void set_channel_warning_alert_limit( uint8_t channel_number , uint16_t warning_alert_value );


        /// @brief Get a channels warning alert limit value.
        /// @param channel_number The channel to get.
        /// @return The warning alert value for this channel.
        uint16_t get_channel_warning_alert_limit( uint8_t channel_number );


        
        /// @brief Gets the sum of the selected shunt voltages.
        /// @return The float value of the sum.
        float get_shunt_voltage_sum();

        

        /// @brief Sets the shunt voltage sum limit value.
        /// @param voltage_sum_limit The limit value.
        void set_shunt_voltage_sum_limit( uint16_t voltage_sum_limit );

        /// @brief Gets the shunt voltage sum limit value.
        /// @return The limit value.
        uint16_t get_shunt_voltage_sum_limit();



        /// @brief Set the power validity upper limit.
        /// @param power_valid_upper_limit The limite value to set.
        void set_power_valid_upper_limit( uint16_t power_valid_upper_limit );

        /// @brief Get the power validity upper limit.
        /// @return The limit value.
        uint16_t get_power_valid_upper_limit();

        
        
        /// @brief Set the power validity lower limit.
        /// @param power_valid_upper_limit The limit value.
        void set_power_valid_lower_limit( uint16_t power_valid_lower_limit );

        /// @brief Get the power validity lower limit.
        /// @return The limit value.
        uint16_t get_power_valid_lower_limit();











};









#endif



