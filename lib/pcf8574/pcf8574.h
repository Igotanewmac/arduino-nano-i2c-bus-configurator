#ifndef PCF8574_HEADER_GUARD
#define PCF8574_HEADER_GUARD

#include <Arduino.h>



class pcf8574
{
    private:
        
        /// @brief The i2c address of the chip.
        uint8_t _i2caddress;

        /// @brief The last byte sent, the current state.
        uint8_t _lastbytesent;

    public:

        /// @brief The object constructor
        /// @param i2caddress The i2c address of the chip
        pcf8574( uint8_t i2caddress );
        pcf8574();

        /// @brief The object deconstructor.
        ~pcf8574();

        /// @brief Initialise the object.
        void begin();
        void begin( uint8_t i2caddress );

        /// @brief Read the pin states from the chip.
        /// @return The pin states, MSB first.
        uint8_t getbyte();

        /// @brief Set the IO status of the pin
        /// @param databyte The pin states, MSB first.
        void setbyte( uint8_t databyte );

        /// @brief Returns the last byte sent to the chip, which is its current state.
        /// @return the pin states, MSB first.
        uint8_t getlastbyte();



};












#endif