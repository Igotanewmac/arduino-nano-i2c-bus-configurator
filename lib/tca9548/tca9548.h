#ifndef TCA9548_HEADER_GUARD
#define TCA9548_HEADER_GUARD

class tca9548
{

    private:

        /// @brief The i2c address of the device
        uint8_t _i2c_address;

    public:

        /// @brief The object constructor
        /// @param i2caddress The i2c address of the tca9548
        tca9548( uint8_t i2caddress );
        tca9548( );

        /// @brief The object deconstructor
        ~tca9548();

        /// @brief Set the devices i2c address.
        /// @param i2caddress The i2c address of the device.
        void seti2caddress( uint8_t i2caddress );

        /// @brief Set up and initialise the TCA9548
        void begin();

        /// @brief Set up and initialise the TCA9548
        /// @param i2caddress The i2caddress of the device
        void begin( uint8_t i2caddress );

        /// @brief Get the current bus state register.
        /// @return The eight bits of bus state.
        uint8_t get_bus_states();

        /// @brief Disconnect all busses.
        void disable_bus();

        /// @brief Enable a single i2c bus, disconnecting others as required.
        /// @param i2c_bus_number The bus number to connect to.
        void enable_bus( uint8_t i2c_bus_number );

        /// @brief Mux several i2c busses together.
        /// @param i2c_bus_select_bitmask The bitmask of the busses to mux.
        void mux_buses( uint8_t i2c_bus_select_bitmask );

};

#endif