

#ifndef I2CTOOLS_BODY_GUARD
#define I2CTOOLS_BODY_GUARD

#include <i2ctools.h>



i2ctools::i2ctools(/* args */) {
    
}

i2ctools::~i2ctools() {

}


/// brief Set up i2c and serial
/// param requestedserialspeed the serial speed to use.  default 115200
void i2ctools::begin( uint32_t requestedserialspeed ) {

    // turn on the serial port
    Serial.begin( requestedserialspeed );

    // turn on i2c
    Wire.begin();

    // all done

}




/// brief perform an i2c scan
void i2ctools::scan() {
    
    byte error, address; //variable for error and I2C address
    int nDevices = 0;

    Serial.println("Scanning...");

    nDevices = 0;
    for (address = 1; address < 127; address++ ) {

        // Serial.println( address );
        // The i2c_scanner uses the return value of
        // the Write.endTransmisstion to see if
        // a device did acknowledge to the address.
        Wire.beginTransmission(address);
        error = Wire.endTransmission();

        if (error == 0) {
            Serial.print("I2C device found at address 0x");
            if (address < 16)
            Serial.print("0");
            Serial.println(address, HEX);
            nDevices++;
        }
        
    }

    if (nDevices == 0) { Serial.println("No I2C devices found\n"); }
    else { Serial.println("done\n"); }

    return;

}








#endif