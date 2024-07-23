#ifndef SENSORBUS_HEADER_GUARD
#define SENSORBUS_HEADER_GUARD

#include <Arduino.h>

#include <Wire.h>

#include <ina219.h>

#include <mcp4725.h>

#include <systembus.h>
extern systembus mysystembusobj;


#define LEDTOGND_0 0
#define LEDTOGND_1 1
#define MVCC_0 2
#define MVCC_1 3
#define VMVCC_0 4
#define VMVCC_1 5
#define VMVCC_DAC_0 0
#define VMVCC_DAC_1 1

#define SYSTEMBUS_LEDTOGND_0 SYSTEMBUS_BUSID_SENSORBUS_0
#define SYSTEMBUS_LEDTOGND_1 SYSTEMBUS_BUSID_SENSORBUS_0
#define SYSTEMBUS_MVCC_0 SYSTEMBUS_BUSID_SENSORBUS_0
#define SYSTEMBUS_MVCC_1 SYSTEMBUS_BUSID_SENSORBUS_0
#define SYSTEMBUS_VMVCC_0 SYSTEMBUS_BUSID_SENSORBUS_1
#define SYSTEMBUS_VMVCC_1 SYSTEMBUS_BUSID_SENSORBUS_1
#define SYSTEMBUS_VMVCC_DAC_0 SYSTEMBUS_BUSID_SENSORBUS_1
#define SYSTEMBUS_VMVCC_DAC_1 SYSTEMBUS_BUSID_SENSORBUS_1






class sensorbus {

    private:
        /* data */

    public:

        ina219 ourina219obj[6];

        mcp4725 ourmcp4725obj[2];

        sensorbus(/* args */);
        ~sensorbus();

        /// @brief Initialise all the things.
        void begin( uint8_t* i2caddresses );



        /// @brief Switch to the right sensorbus for this sensor.
        /// @param sensorid The sensor to switch to.
        void switchto( uint8_t sensorid );

        

};








#endif