#include <Arduino.h>


#include <i2ctools.h>

i2ctools myi2ctoolsobj;

#include <systembus.h>

systembus mysystembusobj;



#include <zifbus.h>










void setup() {

  uint8_t i2caddressarray[6] = {0};

  // initialise systembus first, to default system bus
  
  i2caddressarray[0] = 0x70;
  i2caddressarray[1] = 0x20;


  mysystembusobj.begin( i2caddressarray );
  
  myi2ctoolsobj.begin( 115200 );

  mysystembusobj.setled( 0 );
  
  

}





void loop() {

  mysystembusobj.switchbus( SYSTEMBUS_BUSID_SYSTEMBUS );

  myi2ctoolsobj.scan();

  delay( 1000 );

}























