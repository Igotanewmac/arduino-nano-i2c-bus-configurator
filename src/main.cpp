#include <Arduino.h>


#include <i2ctools.h>
i2ctools myi2ctoolsobj;

#include <basetools.h>

#include <systembus.h>
systembus mysystembusobj;



#include <zifbus.h>
zifbus myzifbusobj;



#include <sensorbus.h>
sensorbus mysensorbusobj;





void setup() {

  uint8_t i2caddressarray[8] = {0};

  // initialise systembus first, to default system bus
  
  i2caddressarray[0] = 0x70;
  i2caddressarray[1] = 0x20;


  mysystembusobj.begin( i2caddressarray );
  
  myi2ctoolsobj.begin( 115200 );

  mysystembusobj.setled( 0 );
  
  
  i2caddressarray[0] = 0x21;
  i2caddressarray[1] = 0x22;
  i2caddressarray[2] = 0x23;
  i2caddressarray[3] = 0x24;
  i2caddressarray[4] = 0x26;
  i2caddressarray[5] = 0x27;

  myzifbusobj.begin( i2caddressarray );

  myzifbusobj.disableall();

  i2caddressarray[0] = 0x40;
  i2caddressarray[1] = 0x41;
  i2caddressarray[2] = 0x44;
  i2caddressarray[3] = 0x45;
  i2caddressarray[4] = 0x44;
  i2caddressarray[5] = 0x45;
  
  i2caddressarray[6] = 0x60;
  i2caddressarray[7] = 0x61;
  
  mysensorbusobj.begin( i2caddressarray );



}




/// @brief Halt, but do NOT catch fire!
void halt() {
  Serial.println("halted!");
  while(1);
}



/// @brief a utility function to make sure we only show the prompt once.
uint8_t serialpromptstate = 0;
void doprompt( uint8_t requestedstate ) {
  if ( requestedstate ) {
    if ( !serialpromptstate ) {
      serialpromptstate = 1;
      Serial.print( "Ready: " );
    }
  }
  else {
    serialpromptstate = 0;
  }
}





// command declarations

void command_i2cscan();
void command_i2cbus( String & commandline );

void command_zifset( String & commandline );
void command_zifena( String & commandline );
void command_zifdis( String & commandline );
void command_zifallena();
void command_zifalldis();
void command_zifreset();



void command_selftest_a();








void loop() {

  // show prompt if needed
  doprompt(1);
  
  // await serial input.
  if (!Serial.available()) {
    return;
  }

  // reset prompt flag
  doprompt(0);

  // get first line
  String commandline = Serial.readStringUntil( 0x0A );

  // discard the rest
  while ( Serial.available() ) { Serial.read(); }

  // repeat it
  Serial.print(">> " );
  Serial.println( commandline );

  if ( commandline.startsWith( "i2cscan" ) ) { command_i2cscan(); }
  if ( commandline.startsWith( "i2cbus" ) ) { command_i2cbus( commandline ); }

  if ( commandline.startsWith( "zifset" ) ) { command_zifset( commandline ); }
  if ( commandline.startsWith( "zifena" ) ) { command_zifena( commandline ); }
  if ( commandline.startsWith( "zifdis" ) ) { command_zifdis( commandline ); }
  if ( commandline.startsWith( "zifallena" ) ) { command_zifallena(); }
  if ( commandline.startsWith( "zifalldis" ) ) { command_zifalldis(); }
  if ( commandline.startsWith( "zifreset" ) ) { command_zifreset(); }


  if ( commandline.startsWith( "selftesta" ) ) { command_selftest_a(); }


}







void command_i2cscan() {
  myi2ctoolsobj.scan();
};




void command_i2cbus( String & commandline ) {

  // clean off command
  commandline = commandline.substring( 7 );

  if ( commandline.length() < 2 ) {
    Serial.println("ERROR: i2c bus number not recognised!");
    return;
  }

  uint8_t i2cbusid = 0xFF;
  i2cbusid = commandline.toInt();

  if ( i2cbusid > 7 ) {
    Serial.println("ERROR: i2c bus number is wrong.");
    return;
  }

  Serial.print("Switching to bus ");
  Serial.println( i2cbusid );
  mysystembusobj.switchbus( i2cbusid );

};





void command_zifset( String & commandline ) {

  // clean off command
  commandline = commandline.substring( 7 );

  if ( commandline.length() < 2 ) {
    Serial.println("ERROR: zif pin id not recognised!");
    return;
  }

  uint8_t zifpinid = commandline.toInt();

  if ( zifpinid > 15 ) {
    Serial.println("ERROR: zif pin id is wrong!");
    return;
  }

  commandline = commandline.substring( commandline.indexOf(" ") + 1 );

  uint8_t zifbusid = commandline.toInt();

  if ( zifpinid > 15 ) {
    Serial.println("ERROR: zif bus id is wrong!");
    return;
  }

  Serial.print("Setting pin ");
  Serial.print(zifpinid);
  Serial.print(" to bus ");
  Serial.println(zifbusid);
  myzifbusobj.pin2bus( zifpinid , zifbusid );


};



void command_zifena( String & commandline ) {

  commandline = commandline.substring( 7 );

  if ( commandline.length() < 2 ) {
    Serial.println("ERROR: zif pin id not recognised!");
    return;
  }

  uint8_t zifpinid = commandline.toInt();

  if ( zifpinid > 15 ) {
    Serial.println("ERROR: zif pin id is wrong!");
    return;
  }

  Serial.print("Enabling pin ");
  Serial.println(zifpinid);
  myzifbusobj.enable( zifpinid );

};



void command_zifdis( String & commandline ) {

  commandline = commandline.substring( 7 );

  if ( commandline.length() < 2 ) {
    Serial.println("ERROR: zif pin id not recognised!");
    return;
  }

  uint8_t zifpinid = commandline.toInt();

  if ( zifpinid > 15 ) {
    Serial.println("ERROR: zif pin id is wrong!");
    return;
  }

  Serial.print("Disabling pin ");
  Serial.println(zifpinid);
  myzifbusobj.disable( zifpinid );

};




void command_zifallena() {
  Serial.println("All zif pins enabled.");
  myzifbusobj.enableall();
}


void command_zifalldis() {
  Serial.print("All zif pins disabled.");
  myzifbusobj.disableall();
}



void command_zifreset() {
  Serial.println("Resetting zif bus.");
  myzifbusobj.reset();
}





void command_selftest_a() {


  // for each pin pair
  for (uint8_t loopcounter = 0; loopcounter < 8; loopcounter++) {
    
    Serial.print("Test: ");
    Serial.print( loopcounter );
    Serial.print("\t");

    // set input pin
    myzifbusobj.pin2bus( loopcounter , ZIFBUS_MVCC_0 );
    myzifbusobj.enable( loopcounter );

    // set output pin
    myzifbusobj.pin2bus( 15 - loopcounter , ZIFBUS_LEDTOGND_0 );
    myzifbusobj.enable( 15 - loopcounter );


    delay(10);

    
    mysensorbusobj.switchto(MVCC_0);
    Serial.print( mysensorbusobj.ourina219obj[MVCC_0].getbusvoltage() );
    Serial.print("\t");
    mysensorbusobj.switchto(LEDTOGND_0);
    Serial.println( mysensorbusobj.ourina219obj[LEDTOGND_0].getbusvoltage() );

    // reset
    myzifbusobj.reset();

  }
  



}











