#ifndef MCP4725_HEADER_GUARD
#define MCP4725_HEADER_GUARD

#include <Arduino.h>



struct mcp4725_values {
    uint8_t statusflags;
    uint16_t dacdata;
    uint16_t eepromdata;
};




class mcp4725
{
    private:

        uint8_t _i2c_address;
        
        mcp4725_values _chipdata;

        void _fillchipdata();

    public:

        /// @brief The object initialiser.
        /// @param i2caddress The i2c address of the device.
        mcp4725( uint8_t i2caddress );
        mcp4725(  );

        /// @brief The deconstructor for the device.
        ~mcp4725();

        /// @brief Initialise the device.
        void begin();
        void begin( uint8_t i2caddress );

        /// @brief Set the value of the DAC to a 12 bit value.
        /// @param dacvalue The 12 bit value for the DAC.
        void setvalue( uint16_t dacvalue );

        /// @brief Get the current DAC value.
        /// @return The 12 bit value of the DAC.
        uint16_t getvalue();

        /// @brief Set the power down mode bits in the status register to control a resistor to gnd pn the output.
        /// @param powerdownmode 0b00 Normal Operation, 0b01 = 1k, 0b10 = 100k , 0b11 = 500k.
        void setpowerdownmode( uint8_t powerdownmode );

        /// @brief Gets the power down mode bits in the status register.
        /// @return 0b00 = Normal operation, 0b01 = 1k, 0b10 = 100k, 0b11 = 500k.
        uint8_t getpowerdownmode();



        /// @brief Sets the default value stored in the eeprom.
        /// @param dacvalue The 12 bit value to store.
        void seteepromvalue( uint16_t dacvalue );

        /// @brief Gets the default value stored in the eeprom.
        /// @return The 12 bit value to store.
        uint16_t geteepromvalue();


        /// @brief Set the default eeprom power down mode bits
        /// @param powerdownword The mode bits to set. 0b00 = Normal operation, 0b01 = 1k, 0b10 - 100k, 0b11 = 500k.
        void seteeprompowerdownmode( uint8_t powerdownword );

        /// @brief Get the default eeprom power down mode bits.
        /// @return 0b00 = Normal operation, 0b01 = 1k , 0b10 = 100k , 0b11 = 500k
        uint8_t geteeprompowerdownmode();








};















#endif