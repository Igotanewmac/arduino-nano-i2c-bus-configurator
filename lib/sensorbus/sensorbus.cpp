#ifndef SENSORBUS_BODY_GUARD
#define SENSORBUS_BODY_GUARD

#include <sensorbus.h>








sensorbus::sensorbus(/* args */) {
}

sensorbus::~sensorbus() {
}










void sensorbus::begin( uint8_t* i2caddresses ) {

    // turn on the wire lib
    Wire.begin();

    // switch to first bus
    mysystembusobj.switchbus( SYSTEMBUS_BUSID_SENSORBUS_0 );

    // start up our four multimeters
    ourina219obj[0].begin( i2caddresses[0] );
    ourina219obj[1].begin( i2caddresses[1] );
    ourina219obj[2].begin( i2caddresses[2] );
    ourina219obj[3].begin( i2caddresses[3] );

    // switch to second bus
    mysystembusobj.switchbus( SYSTEMBUS_BUSID_SENSORBUS_1 );

    // start up our two multimeters
    ourina219obj[4].begin( i2caddresses[4] );
    ourina219obj[5].begin( i2caddresses[5] );
    
    // start up our two dacs
    ourmcp4725obj[0].begin( i2caddresses[6] );
    ourmcp4725obj[1].begin( i2caddresses[7] );
    
    // reset dacs to a sane value of off.
    ourmcp4725obj[0].setvalue(0);
    ourmcp4725obj[1].setvalue(0);
    ourmcp4725obj[0].seteepromvalue(0);
    ourmcp4725obj[1].seteepromvalue(0);
    
    // all done!

}



void sensorbus::switchto( uint8_t sensorid ) {

    switch (sensorid)
    {
    case LEDTOGND_0: mysystembusobj.switchbus( SYSTEMBUS_LEDTOGND_0 ); break;
    case LEDTOGND_1: mysystembusobj.switchbus( SYSTEMBUS_LEDTOGND_1 ); break;
    case MVCC_0: mysystembusobj.switchbus( SYSTEMBUS_MVCC_0 ); break;
    case MVCC_1: mysystembusobj.switchbus( SYSTEMBUS_MVCC_1 ); break;
    case VMVCC_0: mysystembusobj.switchbus( SYSTEMBUS_VMVCC_0 ); break;
    case VMVCC_1: mysystembusobj.switchbus( SYSTEMBUS_VMVCC_1 ); break;
    }

}






#endif