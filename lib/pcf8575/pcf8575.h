#ifndef PCF8575_HEADER_GUARD
#define PCF8575_HEADER_GUARD

#include <Arduino.h>

class pcf8575
{
    private:



        /// @brief a cache of the current dataword.
        uint16_t _lastdataword;

    public:

        /// @brief The i2c address of the pcf8575.
        uint8_t _i2c_address;

        /// @brief The object constructor.
        /// @param i2caddress The i2c address of the pcf8575.
        pcf8575( uint8_t i2caddress = 0x80 );

        /// @brief The object deconstructor.
        ~pcf8575();

        /// @brief Initialise the chip.
        void begin( uint8_t i2caddress = 0x80 );

        /// @brief Gets the current pin data word from the chip.
        /// @return The 16 bit pin data word from the pins.
        uint16_t getword();

        /// @brief Sets a pin data word to the chip.
        /// @param dataword The 16 bit data word to set to the pins.
        void setword( uint16_t dataword );


        /// @brief Returns the last dataword sent, which should be the current state.
        /// @return The 16 bit dataword that was previously sent.
        uint16_t getlastword();

};


#endif