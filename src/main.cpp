#include <Arduino.h>


#include <i2ctools.h>
i2ctools myi2ctoolsobj;

#include <basetools.h>

#include <systembus.h>
systembus mysystembusobj;



#include <zifbus.h>
zifbus myzifbusobj;









void setup() {

  uint8_t i2caddressarray[6] = {0};

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











