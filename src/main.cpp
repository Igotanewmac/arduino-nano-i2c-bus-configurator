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




#include <at24c.h>
at24c mychipstore;



// command declarations

void command_i2cscan();
void command_i2cbus( String & commandline );

void command_zifset( String & commandline );
void command_zifena( String & commandline );
void command_zifdis( String & commandline );
void command_zifallena();
void command_zifalldis();
void command_zifreset();


/// @brief Perform a self test with the 0-15 board.
void command_selftest_a();
/// @brief Perform a self test with the 0-1 board.
void command_selftest_b();

/// @brief Perform a functionality check of a gate.
/// @param inputpin The input pin to check.
/// @param outputpin The output pin to check.
/// @return The results of the test.
uint8_t command_test_gate1to1(  uint8_t inputpin , uint8_t outputpin );

void command_test_gate1to1_parser(  String & commandline );


/// @brief Perform a functionality check of the gate.
/// @param inputpina The first input pin to check.
/// @param inputpinb The second input pin to check.
/// @param outputpin The output pin to record.
/// @return The results of the test.
uint8_t command_test_gate2to1(  uint8_t inputpina , uint8_t inputpinb , uint8_t outputpin );

void command_test_gate2to1_parser(  String & commandline );



/// @brief Perform a voltage sweep test and output CSV values over the serial port.
/// @param inputpin The input pin to check.
/// @param outputpin The output pin to check.
void command_test_vsweepu( uint8_t inputpin , uint8_t outputpin , uint16_t stepsize = 128 );

void command_test_vsweepu_parser( String & commandline );

/// @brief Perform a voltage sweep test and output CSV values over the serial port.
/// @param inputpin The input pin to check.
/// @param outputpin The output pin to check.
void command_test_vsweepd( uint8_t inputpin , uint8_t outputpin , uint16_t stepsize = 128 );

void command_test_vsweepd_parser( String & commandline );






void command_test_measure_parser( String & commandline );

void command_test_measure( uint8_t sensorid );


void command_test_dacset_parser( String & commandline );

void command_test_dacset( uint8_t sensorid , uint16_t sensorvalue );



void storeprogram();

void showprogram( String & commandline );

void runprogram( String & commandline );




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


  mychipstore.begin( 0x50 );

  storeprogram();

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


/// @brief Do a tab.
void dotab() {
  Serial.print("\t");
}






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
  if ( commandline.startsWith( "selftestb" ) ) { command_selftest_b(); }

  if ( commandline.startsWith( "vsweepu" ) ) { command_test_vsweepu_parser( commandline ); }
  if ( commandline.startsWith( "vsweepd" ) ) { command_test_vsweepd_parser( commandline ); }


  if ( commandline.startsWith( "gate1to1" ) ) { command_test_gate1to1_parser( commandline ); }
  if ( commandline.startsWith( "gate2to1" ) ) { command_test_gate2to1_parser( commandline ); }
  
  
  
  if ( commandline.startsWith( "measure" ) ) { command_test_measure_parser( commandline ); }
  
  if ( commandline.startsWith( "setdac" ) ) { command_test_dacset_parser( commandline ); }


  if ( commandline.startsWith( "storeprogram" ) ) { storeprogram(); }
  if ( commandline.startsWith( "showprogram" ) ) { showprogram( commandline ); }
  if ( commandline.startsWith( "runprogram" ) ) { runprogram( commandline ); }
  

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
  Serial.println("All zif pins disabled.");
  myzifbusobj.disableall();
}



void command_zifreset() {
  Serial.println("Resetting zif bus.");
  myzifbusobj.reset();
}

















/// @brief Perform a self test with the 0-15 board.
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
    myzifbusobj.pin2bus( 15 - loopcounter , ZIFBUS_MLEDTOGND_0 );
    myzifbusobj.enable( 15 - loopcounter );


    delay(10);

    
    mysensorbusobj.switchto(MVCC_0);
    Serial.print( mysensorbusobj.ourina219obj[MVCC_0].getbusvoltage() );
    Serial.print("\t");
    mysensorbusobj.switchto(LEDTOGND_0);
    Serial.println( mysensorbusobj.ourina219obj[LEDTOGND_0].getpower() );

    // reset
    myzifbusobj.reset();

  }
  
}



/// @brief Perform a self test with the 0-1 board.
//void command_selftest_b();
void command_selftest_b() {

  for ( uint8_t loopcounter = 0 ; loopcounter < 16 ; loopcounter += 2 ) {

      myzifbusobj.pin2bus( loopcounter , ZIFBUS_MVCC_0 );
      myzifbusobj.enable( loopcounter );

      myzifbusobj.pin2bus( loopcounter + 1 , ZIFBUS_MLEDTOGND_0 );
      myzifbusobj.enable( loopcounter + 1 );

      Serial.print( loopcounter ); dotab();

      mysensorbusobj.switchto(MVCC_0);
      Serial.print( mysensorbusobj.ourina219obj[MVCC_0].getbusvoltage() ); dotab();

      mysensorbusobj.switchto(LEDTOGND_0);
      Serial.print( mysensorbusobj.ourina219obj[LEDTOGND_0].getpower() );

      Serial.println();

      myzifbusobj.reset();


  }

}












void command_test_gate1to1_parser( String & commandline ) {

  // remove command
  commandline = commandline.substring( 9 );
  commandline += " ";

  // get the input pin
  uint8_t inputpin = commandline.toInt();
  commandline = commandline.substring( commandline.indexOf(" ") + 1 );

  // get the output pin
  uint8_t outputpin = commandline.toInt();
  commandline = commandline.substring( commandline.indexOf(" ") + 1 );

  Serial.print("Result: ");
  showbin( command_test_gate1to1( inputpin , outputpin ) );
  Serial.println();

}

/// @brief Perform a functionality check of a gate.
/// @param inputpin The input pin to check.
/// @param outputpin The output pin to check.
/// @return The results of the test.
uint8_t command_test_gate1to1(  uint8_t inputpin , uint8_t outputpin ) {

  uint8_t result = 0;

  mysensorbusobj.switchto(VMVCC_0);
  mysensorbusobj.ourmcp4725obj[VMVCC_DAC_0].setvalue(0);

  myzifbusobj.pin2bus( inputpin , ZIFBUS_VMVCC_0 );
  myzifbusobj.enable( inputpin );

  myzifbusobj.pin2bus( outputpin , ZIFBUS_MLEDTOGND_0 );
  myzifbusobj.enable( outputpin );

  // first test
  //input is at zero
  // output is at....
  mysensorbusobj.switchto(LEDTOGND_0);

  result |= ( mysensorbusobj.ourina219obj[LEDTOGND_0].getbusvoltage() > 1.5 ? 0b1 : 0b0 );

  // now second test
  // input is high
  mysensorbusobj.switchto(VMVCC_0);
  mysensorbusobj.ourmcp4725obj[VMVCC_DAC_0].setvalue( 4095 );
  delay(5);

  mysensorbusobj.switchto(LEDTOGND_0);
  result |= ( mysensorbusobj.ourina219obj[LEDTOGND_0].getbusvoltage() > 1.5 ? 0b10 : 0b00 );

  mysensorbusobj.switchto(VMVCC_0);
  mysensorbusobj.ourmcp4725obj[VMVCC_DAC_0].setvalue( 4095 );
  myzifbusobj.reset();

  return result;
}










void command_test_gate2to1_parser( String & commandline ) {

  // remove command
  commandline = commandline.substring( 9 );
  commandline += " ";

  // get the input pin
  uint8_t inputpina = commandline.toInt();
  commandline = commandline.substring( commandline.indexOf(" ") + 1 );
  
  // get the input pin
  uint8_t inputpinb = commandline.toInt();
  commandline = commandline.substring( commandline.indexOf(" ") + 1 );

  // get the output pin
  uint8_t outputpin = commandline.toInt();

  Serial.println("Gate 2 to 1 test.");
  Serial.print("input a: ");
  Serial.println( inputpina );
  Serial.print("input b: ");
  Serial.println( inputpinb );
  Serial.print("output: ");
  Serial.println( outputpin );

  Serial.print("Result: ");
  showbin( command_test_gate2to1( inputpina , inputpinb , outputpin ) );
  Serial.println();

}

/// @brief Perform a functionality check of the gate.
/// @param inputpina The first input pin to check.
/// @param inputpinb The second input pin to check.
/// @param outputpin The output pin to record.
/// @return The results of the test.
uint8_t command_test_gate2to1(  uint8_t inputpina , uint8_t inputpinb , uint8_t outputpin ) {

  // reset everything
  mysensorbusobj.switchto(VMVCC_0);
  mysensorbusobj.ourmcp4725obj[VMVCC_DAC_0].setvalue( 0 );
  mysensorbusobj.switchto(VMVCC_1);
  mysensorbusobj.ourmcp4725obj[VMVCC_DAC_1].setvalue( 0 );
  
  // set up the pins
  myzifbusobj.pin2bus( inputpina , ZIFBUS_VMVCC_0 );
  myzifbusobj.enable( inputpina );

  myzifbusobj.pin2bus( inputpinb , ZIFBUS_VMVCC_1 );
  myzifbusobj.enable( inputpinb );

  myzifbusobj.pin2bus( outputpin , ZIFBUS_MLEDTOGND_0 );
  myzifbusobj.enable( outputpin );

  uint8_t result = 0;

  for ( uint8_t loopcounter = 0 ; loopcounter < 4 ; loopcounter++ ) {

    mysensorbusobj.switchto(VMVCC_0);
    mysensorbusobj.ourmcp4725obj[VMVCC_DAC_0].setvalue( loopcounter & 0b1 ? 4095 : 0 );
    
    mysensorbusobj.switchto(VMVCC_1);
    mysensorbusobj.ourmcp4725obj[VMVCC_DAC_1].setvalue( loopcounter & 0b10 ? 4095 : 0 );

    // settle delay
    delay(5);

    // record result
    mysensorbusobj.switchto(LEDTOGND_0);
    result |= ( mysensorbusobj.ourina219obj[LEDTOGND_0].getbusvoltage() > 1.5 ? 1 : 0 ) << loopcounter;

  }

  // reset everything
  mysensorbusobj.switchto(VMVCC_0);
  mysensorbusobj.ourmcp4725obj[VMVCC_DAC_0].setvalue( 0 );
  mysensorbusobj.switchto(VMVCC_1);
  mysensorbusobj.ourmcp4725obj[VMVCC_DAC_1].setvalue( 0 );
  
  myzifbusobj.reset();

  return result;
}













void command_test_vsweepu_parser( String & commandline ) {

  // remove command
  commandline = commandline.substring( 8 );
  commandline += " ";

  // get the input pin
  uint8_t inputpin = commandline.toInt();
  commandline = commandline.substring( commandline.indexOf(" ") + 1 );

  // get the output pin
  uint8_t outputpin = commandline.toInt();
  commandline = commandline.substring( commandline.indexOf(" ") + 1 );

  // get the stepsize if any
  uint16_t stepsize = 0;
  if ( commandline.length() ) {
    stepsize = commandline.toInt();
  }

  if ( stepsize == 0 ) { stepsize = 128; }

  command_test_vsweepu( inputpin , outputpin , stepsize );

};


/// @brief Perform a voltage sweep test and output CSV values over the serial port.
/// @param inputpin The input pin to check.
/// @param outputpin The output pin to check.
void command_test_vsweepu( uint8_t inputpin , uint8_t outputpin , uint16_t stepsize ) {

  Serial.println("vsweep test");
  Serial.print("Input: ");
  Serial.println( inputpin );
  Serial.print("Output: ");
  Serial.println( outputpin );
  Serial.print("Stepsize: " );
  Serial.println( stepsize );

  myzifbusobj.pin2bus( inputpin , ZIFBUS_VMVCC_0 );
  myzifbusobj.enable( inputpin );

  myzifbusobj.pin2bus( outputpin , ZIFBUS_MLEDTOGND_0 );
  myzifbusobj.enable( outputpin );

  for ( uint16_t loopcounter = 0 ; loopcounter < 4096 ; loopcounter += stepsize ) {

    Serial.print(loopcounter);
    dotab();

    // set the dac first
    mysensorbusobj.switchto(VMVCC_0);
    mysensorbusobj.ourmcp4725obj[VMVCC_DAC_0].setvalue( loopcounter );

    // delay to let things settle
    delay(5);

    // input
    mysensorbusobj.switchto(VMVCC_0);
    Serial.print( mysensorbusobj.ourina219obj[VMVCC_0].getbusvoltage() ); dotab();
    Serial.print( mysensorbusobj.ourina219obj[VMVCC_0].getcurrent() ); dotab();
    Serial.print( mysensorbusobj.ourina219obj[VMVCC_0].getpower() ); dotab();

    // output
    mysensorbusobj.switchto(LEDTOGND_0);
    Serial.print( mysensorbusobj.ourina219obj[LEDTOGND_0].getbusvoltage() ); dotab();
    Serial.print( mysensorbusobj.ourina219obj[LEDTOGND_0].getcurrent() ); dotab();
    Serial.print( mysensorbusobj.ourina219obj[LEDTOGND_0].getpower() );

    Serial.println();
    
  }

  Serial.print(4096); dotab();
  mysensorbusobj.switchto(VMVCC_0);
  mysensorbusobj.ourmcp4725obj[VMVCC_DAC_0].setvalue( 4095 );
  delay(5);
  mysensorbusobj.switchto(VMVCC_0);
  Serial.print( mysensorbusobj.ourina219obj[VMVCC_0].getbusvoltage() ); dotab();
  Serial.print( mysensorbusobj.ourina219obj[VMVCC_0].getcurrent() ); dotab();
  Serial.print( mysensorbusobj.ourina219obj[VMVCC_0].getpower() ); dotab();
  mysensorbusobj.switchto(LEDTOGND_0);
  Serial.print( mysensorbusobj.ourina219obj[LEDTOGND_0].getbusvoltage() ); dotab();
  Serial.print( mysensorbusobj.ourina219obj[LEDTOGND_0].getcurrent() ); dotab();
  Serial.print( mysensorbusobj.ourina219obj[LEDTOGND_0].getpower() );
  Serial.println();

  // turn off the vmvcc!
  mysensorbusobj.switchto(VMVCC_0);
  mysensorbusobj.ourmcp4725obj[VMVCC_DAC_0].setvalue(0);

  myzifbusobj.reset();

}





void command_test_vsweepd_parser( String & commandline ) {

  // remove command
  commandline = commandline.substring( 8 );
  commandline += " ";

  // get the input pin
  uint8_t inputpin = commandline.toInt();
  commandline = commandline.substring( commandline.indexOf(" ") + 1 );

  // get the output pin
  uint8_t outputpin = commandline.toInt();
  commandline = commandline.substring( commandline.indexOf(" ") + 1 );

  // get the stepsize if any
  uint16_t stepsize = 0;
  if ( commandline.length() ) {
    stepsize = commandline.toInt();
  }

  if ( stepsize == 0 ) { stepsize = 128; }

  command_test_vsweepd( inputpin , outputpin , stepsize );

};

/// @brief Perform a voltage sweep test and output CSV values over the serial port.
/// @param inputpin The input pin to check.
/// @param outputpin The output pin to check.
void command_test_vsweepd( uint8_t inputpin , uint8_t outputpin , uint16_t stepsize ) {

  Serial.println("vsweep test");
  Serial.print("Input: ");
  Serial.println( inputpin );
  Serial.print("Output: ");
  Serial.println( outputpin );
  Serial.print("Stepsize: " );
  Serial.println( stepsize );

  myzifbusobj.pin2bus( inputpin , ZIFBUS_VMVCC_0 );
  myzifbusobj.enable( inputpin );

  myzifbusobj.pin2bus( outputpin , ZIFBUS_MLEDTOGND_0 );
  myzifbusobj.enable( outputpin );

  Serial.print(4096); dotab();
  mysensorbusobj.switchto(VMVCC_0);
  mysensorbusobj.ourmcp4725obj[VMVCC_DAC_0].setvalue( 4095 );
  delay(5);
  mysensorbusobj.switchto(VMVCC_0);
  Serial.print( mysensorbusobj.ourina219obj[VMVCC_0].getbusvoltage() ); dotab();
  Serial.print( mysensorbusobj.ourina219obj[VMVCC_0].getcurrent() ); dotab();
  Serial.print( mysensorbusobj.ourina219obj[VMVCC_0].getpower() ); dotab();
  mysensorbusobj.switchto(LEDTOGND_0);
  Serial.print( mysensorbusobj.ourina219obj[LEDTOGND_0].getbusvoltage() ); dotab();
  Serial.print( mysensorbusobj.ourina219obj[LEDTOGND_0].getcurrent() ); dotab();
  Serial.print( mysensorbusobj.ourina219obj[LEDTOGND_0].getpower() );
  Serial.println();

  for ( uint16_t loopcounter = 4096 - stepsize ; loopcounter > 0 ; loopcounter -= stepsize ) {

    Serial.print(loopcounter);
    dotab();

    // set the dac first
    mysensorbusobj.switchto(VMVCC_0);
    mysensorbusobj.ourmcp4725obj[VMVCC_DAC_0].setvalue( loopcounter );

    // delay to let things settle
    delay(5);

    // input
    mysensorbusobj.switchto(VMVCC_0);
    Serial.print( mysensorbusobj.ourina219obj[VMVCC_0].getbusvoltage() ); dotab();
    Serial.print( mysensorbusobj.ourina219obj[VMVCC_0].getcurrent() ); dotab();
    Serial.print( mysensorbusobj.ourina219obj[VMVCC_0].getpower() ); dotab();

    // output
    mysensorbusobj.switchto(LEDTOGND_0);
    Serial.print( mysensorbusobj.ourina219obj[LEDTOGND_0].getbusvoltage() ); dotab();
    Serial.print( mysensorbusobj.ourina219obj[LEDTOGND_0].getcurrent() ); dotab();
    Serial.print( mysensorbusobj.ourina219obj[LEDTOGND_0].getpower() );

    Serial.println();
    
  }

  Serial.print(0); dotab();
  mysensorbusobj.switchto(VMVCC_0);
  mysensorbusobj.ourmcp4725obj[VMVCC_DAC_0].setvalue( 0 );
  delay(5);
  mysensorbusobj.switchto(VMVCC_0);
  Serial.print( mysensorbusobj.ourina219obj[VMVCC_0].getbusvoltage() ); dotab();
  Serial.print( mysensorbusobj.ourina219obj[VMVCC_0].getcurrent() ); dotab();
  Serial.print( mysensorbusobj.ourina219obj[VMVCC_0].getpower() ); dotab();
  mysensorbusobj.switchto(LEDTOGND_0);
  Serial.print( mysensorbusobj.ourina219obj[LEDTOGND_0].getbusvoltage() ); dotab();
  Serial.print( mysensorbusobj.ourina219obj[LEDTOGND_0].getcurrent() ); dotab();
  Serial.print( mysensorbusobj.ourina219obj[LEDTOGND_0].getpower() );
  Serial.println();


  // turn off the vmvcc!
  mysensorbusobj.switchto(VMVCC_0);
  mysensorbusobj.ourmcp4725obj[VMVCC_DAC_0].setvalue(0);

  myzifbusobj.reset();

}







void command_test_measure_parser( String & commandline ) {

  // remove command
  commandline = commandline.substring( 8 );
  commandline += " ";

  // get the input pin
  uint8_t sensorid = commandline.toInt();
  commandline = commandline.substring( commandline.indexOf(" ") + 1 );

  command_test_measure( sensorid );
}


void command_test_measure( uint8_t sensorid ) {

  Serial.print( "Measure: ");
  Serial.print( sensorid ); dotab();

  mysensorbusobj.switchto(sensorid);
  Serial.print( mysensorbusobj.ourina219obj[sensorid].getbusvoltage() ); dotab();
  Serial.print( mysensorbusobj.ourina219obj[sensorid].getshuntvoltage() ); dotab();
  Serial.print( mysensorbusobj.ourina219obj[sensorid].getcurrent() ); dotab();
  Serial.print( mysensorbusobj.ourina219obj[sensorid].getpower() );
  Serial.println();
  
}












void command_test_dacset_parser( String & commandline ) {

  // remove command
  commandline = commandline.substring( 7 );
  commandline += " ";

  // get the dac index
  uint8_t sensorid = commandline.toInt();
  commandline = commandline.substring( commandline.indexOf(" ") + 1 );

  // get the value
  uint16_t sensorvalue = commandline.toInt();
  commandline = commandline.substring( commandline.indexOf(" ") + 1 );

  Serial.print("Setting DAC ");
  Serial.print( sensorid );
  Serial.print(" to ");
  Serial.print( sensorvalue );
  Serial.println();

  command_test_dacset( sensorid , sensorvalue );

}

void command_test_dacset( uint8_t sensorid , uint16_t sensorvalue ) {

  mysensorbusobj.switchto(VMVCC_0);
  mysensorbusobj.ourmcp4725obj[sensorid].setvalue( sensorvalue );

}















void storeprogram() {

  Serial.println( "Storing Program" );

  mysystembusobj.switchbus(SYSTEMBUS_BUSID_SYSTEMBUS);

  uint16_t progptr = 0;


  Serial.print("Selftest: A");
  Serial.println( progptr );

  mychipstore.zifreset( progptr );
  for ( uint8_t loopcounter = 0 ; loopcounter < 8 ; loopcounter++ ) {
  mychipstore.zifset( progptr , loopcounter , 11 );
  mychipstore.zifena( progptr , loopcounter );
  mychipstore.zifset( progptr , 15 - loopcounter , 9 );
  mychipstore.zifena( progptr , 15 - loopcounter );
  mychipstore.delay( progptr , 1 );
  mychipstore.measure( progptr , 2 );
  mychipstore.measure( progptr , 0 );
  mychipstore.zifreset( progptr );
  }
  mychipstore.endprogram( progptr );

  Serial.print("Selftest B: ");
  Serial.println( progptr );

  mychipstore.zifreset( progptr );
  for ( uint8_t loopcounter = 0 ; loopcounter < 16 ; loopcounter += 2 ) {
  mychipstore.zifset( progptr , loopcounter , 11 );
  mychipstore.zifena( progptr , loopcounter );
  mychipstore.zifset( progptr , loopcounter + 1 , 9 );
  mychipstore.zifena( progptr , loopcounter + 1 );
  mychipstore.delay( progptr , 1 );
  mychipstore.measure( progptr , 2 );
  mychipstore.measure( progptr , 0 );
  mychipstore.zifreset( progptr );
  }
  mychipstore.endprogram( progptr );




  // 7400

  Serial.print( "7400 reset : " );
  Serial.println( progptr );

  /*
    7400 quad NAND
           ----
    1A  0 -|  |-  15  VCC
    1B  1 -|  |-  14  4B
    1Y  2 -|  |-  13  4A
    2A  3 -|  |-  12  4Y
    2B  4 -|  |-  11  3B
    2Y  5 -|  |-  10  3A
    GND 6 -|  |-  9   3Y
           ----

  */

  // set gates to off
  mychipstore.zifreset( progptr );

  // gate config
  #define P1A 0
  #define P1B 1
  #define P1Y 2
  #define P2A 3
  #define P2B 4
  #define P2Y 5
  #define P3A 10
  #define P3B 11
  #define P3Y 9
  #define P4A 13
  #define P4B 14
  #define P4Y 12
  #define PVCC 15
  #define PGND 6

  // mychipstore.zifset( progptr , P1A , ZIFBUS_VCC );
  // mychipstore.zifset( progptr , P1B , ZIFBUS_VCC );
  // mychipstore.zifset( progptr , P1Y , ZIFBUS_GND );
  // mychipstore.zifset( progptr , P2A , ZIFBUS_VCC );
  // mychipstore.zifset( progptr , P2B , ZIFBUS_VCC );
  // mychipstore.zifset( progptr , P2Y , ZIFBUS_GND );
  // mychipstore.zifset( progptr , P3A , ZIFBUS_VCC );
  // mychipstore.zifset( progptr , P3B , ZIFBUS_VCC );
  // mychipstore.zifset( progptr , P3Y , ZIFBUS_GND );
  // mychipstore.zifset( progptr , P4A , ZIFBUS_VCC );
  // mychipstore.zifset( progptr , P4B , ZIFBUS_VCC );
  // mychipstore.zifset( progptr , P4Y , ZIFBUS_GND );
  
  // mychipstore.zifset( progptr , PVCC , ZIFBUS_VCC );
  // mychipstore.zifset( progptr , PGND , ZIFBUS_GND );

  // mychipstore.zifallena( progptr );
  // mychipstore.delay( progptr , 5 );

  mychipstore.gate2to1( progptr , P1A , P1B , P1Y );
  // mychipstore.zifset( progptr , P1A , ZIFBUS_VCC );
  // mychipstore.zifset( progptr , P1B , ZIFBUS_VCC );
  // mychipstore.zifset( progptr , P1Y , ZIFBUS_GND );
  
  mychipstore.gate2to1( progptr , P2A , P2B , P2Y );
  // mychipstore.zifset( progptr , P2A , ZIFBUS_VCC );
  // mychipstore.zifset( progptr , P2B , ZIFBUS_VCC );
  // mychipstore.zifset( progptr , P2Y , ZIFBUS_GND );
  
  mychipstore.gate2to1( progptr , P3A , P3B , P3Y );
  // mychipstore.zifset( progptr , P3A , ZIFBUS_VCC );
  // mychipstore.zifset( progptr , P3B , ZIFBUS_VCC );
  // mychipstore.zifset( progptr , P3Y , ZIFBUS_GND );
  
  mychipstore.gate2to1( progptr , P4A , P4B , P4Y );
  // mychipstore.zifset( progptr , P4A , ZIFBUS_VCC );
  // mychipstore.zifset( progptr , P4B , ZIFBUS_VCC );
  // mychipstore.zifset( progptr , P4Y , ZIFBUS_GND );
  




  mychipstore.zifreset( progptr );








  mychipstore.endprogram( progptr );

}



void showprogram( String & commandline ) {

  mysystembusobj.switchbus(SYSTEMBUS_BUSID_SYSTEMBUS);

  commandline = commandline.substring( 11 );
  commandline += " ";

  uint16_t progptr = commandline.toInt();


  Serial.println( "Program" );

  uint8_t databyte = 0;
  while ( databyte != 0xFF ) {
    databyte = mychipstore.getdata( progptr++ );
    showhex( databyte );
  }
  Serial.println();

}




















void runprogram( String & commandline ) {

  mysystembusobj.switchbus(SYSTEMBUS_BUSID_SYSTEMBUS);

  commandline = commandline.substring( 11 );
  commandline += " ";

  uint16_t programstartaddress = commandline.toInt();

  Serial.print( "Program " );
  Serial.println( programstartaddress );

  uint16_t progptr = programstartaddress;
  uint8_t databyte = 0;

  uint8_t u8bitarg1 = 0;
  uint8_t u8bitarg2 = 0;
  uint8_t u8bitarg3 = 0;
  
  uint16_t u16bitarg1 = 0;
  

  while ( databyte != 0xFF ) {
    mysystembusobj.switchbus(SYSTEMBUS_BUSID_SYSTEMBUS);
    databyte = mychipstore.getdata( progptr++ );
    Serial.print( progptr - 1 );;
    Serial.print(" - ");
    showhex( databyte );
    Serial.print(" - ");

    switch (databyte) {

    case 0x00:
      // no-op
      break;
    
    // zifset
    case 0x01:
      u8bitarg1 =  mychipstore.getdata( progptr++ );
      u8bitarg2 =  mychipstore.getdata( progptr++ );
      Serial.print( "Zifset " );
      Serial.print( u8bitarg1 );
      Serial.print( " to " );
      Serial.print( u8bitarg2 );
      Serial.println();
      myzifbusobj.pin2bus( u8bitarg1 , u8bitarg2 );
      break;
    

    // zifena
    case 0x02:
      u8bitarg1 = mychipstore.getdata( progptr++ );
      Serial.print( "Zifena " );
      Serial.println( u8bitarg1 );
      myzifbusobj.enable( u8bitarg1 );
      break;


    // zifdis
    case 0x03:
      u8bitarg1 = mychipstore.getdata( progptr++ );
      Serial.print( "Zifdis " );
      Serial.println( u8bitarg1 );
      myzifbusobj.disable( u8bitarg1 );
      break;

    // zifallena
    case 0x04:
      Serial.print( "Zifallena " );
      Serial.println();
      myzifbusobj.enableall();
      break;
    
    // zifalldis
    case 0x05:
      Serial.print( "Zifalldis " );
      Serial.println();
      myzifbusobj.disableall();
      break;

    // zifreset
    case 0x06:
        Serial.print( "Zifrst " );
        Serial.println();
        myzifbusobj.reset();
      break;

    // gate1to1
    case 0x07:
      u8bitarg1 = mychipstore.getdata( progptr++ );
      u8bitarg2 = mychipstore.getdata( progptr++ );
      Serial.print( "Gate1to1 input: ");
      Serial.print( u8bitarg1 );
      Serial.print( "  -> output: " );
      Serial.print( u8bitarg2 );
      Serial.println();
      showbin( command_test_gate1to1( u8bitarg1 , u8bitarg2 ) );
      Serial.println();
      break;

    // gate2to1
    case 0x08:
      u8bitarg1 = mychipstore.getdata( progptr++ );
      u8bitarg2 = mychipstore.getdata( progptr++ );
      u8bitarg3 = mychipstore.getdata( progptr++ );
      Serial.print( "Gate2to1 inputa: ");
      Serial.print( u8bitarg1 );
      Serial.print( " inputb: " );
      Serial.print( u8bitarg2 );
      Serial.print( " output: " );
      Serial.print( u8bitarg3 );
      Serial.println();
      showbin( command_test_gate2to1( u8bitarg1 , u8bitarg2 , u8bitarg3 ) );
      Serial.println();
      break;

    // vsweepu
    case 0x09:
      u8bitarg1 = mychipstore.getdata( progptr++ );
      u8bitarg2 = mychipstore.getdata( progptr++ );
      u16bitarg1 = mychipstore.getdata( progptr++ ) << 8;
      u16bitarg1 |= mychipstore.getdata( progptr++ );
      Serial.print( "vsweepu input: " );
      Serial.print( u8bitarg1 );
      Serial.print( " output: " );
      Serial.print( u8bitarg2 );
      Serial.print( " stepsize: " );
      Serial.print( u16bitarg1 );
      Serial.println();
      command_test_vsweepu( u8bitarg1 , u8bitarg2 , u16bitarg1 );
      break;
    
    // vsweepd
    case 0x0A:
      u8bitarg1 = mychipstore.getdata( progptr++ );
      u8bitarg2 = mychipstore.getdata( progptr++ );
      u16bitarg1 = mychipstore.getdata( progptr++ ) << 8;
      u16bitarg1 |= mychipstore.getdata( progptr++ );
      Serial.print( "vsweepu input: " );
      Serial.print( u8bitarg1 );
      Serial.print( " output: " );
      Serial.print( u8bitarg2 );
      Serial.print( " stepsize: " );
      Serial.print( u16bitarg1 );
      Serial.println();
      command_test_vsweepd( u8bitarg1 , u8bitarg2 , u16bitarg1 );
      break;

    // measure
    case 0x0B:
      u8bitarg1 = mychipstore.getdata( progptr++ );
      Serial.print( "Measure: " );
      Serial.print( u8bitarg1 ); dotab();
      mysensorbusobj.switchto( u8bitarg1 );
      Serial.print( mysensorbusobj.ourina219obj[u8bitarg1].getbusvoltage() ); dotab();
      Serial.print( mysensorbusobj.ourina219obj[u8bitarg1].getshuntvoltage() ); dotab();
      Serial.print( mysensorbusobj.ourina219obj[u8bitarg1].getcurrent() ); dotab();
      Serial.print( mysensorbusobj.ourina219obj[u8bitarg1].getpower() );
      Serial.println();
      break;
    
    // set
    case 0x0C:
      u8bitarg1 = mychipstore.getdata( progptr++ );
      u16bitarg1 = mychipstore.getdata( progptr++ ) << 8 ;
      u16bitarg1 |= mychipstore.getdata( progptr++ );
      Serial.print( "set: " );
      Serial.print( u8bitarg1 );
      Serial.print( " to: " );
      Serial.print( u16bitarg1 );
      mysensorbusobj.switchto(VMVCC_0);
      mysensorbusobj.ourmcp4725obj[u8bitarg1].setvalue( u16bitarg1 );
      break;

    // delay
    case 0x0D:
      u16bitarg1 = mychipstore.getdata( progptr++ ) << 8;
      u16bitarg1 |= mychipstore.getdata( progptr++ );
      Serial.print( "Delay: " );
      Serial.print( u16bitarg1 );
      Serial.println();
      delay( u16bitarg1 );
      break;

    default:
      Serial.println();
      break;




    }




  }
  Serial.println();

}

