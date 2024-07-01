#ifndef ZIFBUS_HEADER_GUARD
#define ZIFBUS_HEADER_GUARD


#include <Arduino.h>




class zifbus {
        
    private:
        uint8_t _i2c_addresses[6];

    public:

        zifbus();

        ~zifbus();



};
















#endif