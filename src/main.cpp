#include <Arduino.h>



// awesome base tools for debugging
#include <basetools.h>

// awesome i2c tools for debugging
#include <i2ctools.h>
i2ctools i2ctoolbox;


// tca9548 I2C Bus Muxer.
#include <tca9548.h>
tca9548 i2cbusmaster( 0x70 );




// super duper zifsocklib
#include <zifsocklib.h>
zifsocklib i2czifsocklibobj;


// super duper busport connector
#include <busportlib.h>
busportlib i2cbusport;




void halt() {
    while(1);
}



void setup() {
  // put your setup code here, to run once:

  uint8_t i2caddresses[6] = {
    0x20,
    0x22,
    0x23,
    0x24,
    0x26,
    0x27
  };

  i2czifsocklibobj.begin( i2caddresses );

  i2ctoolbox.begin();

  i2cbusmaster.begin();

  i2cbusport.begin();
  
  i2cbusmaster.enable_bus(2);

  for ( uint8_t loopcounter = 0 ; loopcounter < 16 ; loopcounter++ ) {
    i2czifsocklibobj.disable_pin( loopcounter );
    i2czifsocklibobj.pin_to_bus( loopcounter , BUSPORT_NC );
  }


}



// i2c bus commands
void command_i2cscan();
void command_i2cbus( String commandline );

// zif socket commands
void command_zifset( String commandline );
void command_zifena( String commandline );
void command_zifdis( String commandline );


// busport commands
void command_busport_get( String commandline );
void command_busport_set( String commandline );



// actual testing commands
void command_test_gate_1to1( uint8_t inputpin , uint8_t outputpin );

void command_sweep_gate_1to1( uint8_t inputpin , uint8_t outputpin );


uint8_t command_test_gate_2to1( uint8_t inputpin_0 , uint8_t inputpin_1 , uint8_t outputpin );


void test_7400();

void test_self();




// a utility function to make sure we only show the prompt once.
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








void loop() {
  // put your main code here, to run repeatedly:
  
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


  // enumerate the i2c bus.
  if ( commandline.startsWith( "i2cscan" ) ) { command_i2cscan(); }
  if ( commandline.startsWith( "i2cbus" ) ) { command_i2cbus( commandline ); }

  if ( commandline.startsWith( "zifset" ) ) { command_zifset( commandline ); }
  if ( commandline.startsWith( "zifena" ) ) { command_zifena( commandline ); }
  if ( commandline.startsWith( "zifdis" ) ) { command_zifdis( commandline ); }

  if ( commandline.startsWith( "busportget" ) ) { command_busport_get( commandline ); }
  if ( commandline.startsWith( "busportset" ) ) { command_busport_set( commandline ); }


  if ( commandline.startsWith( "testgate1to1" ) ) { 

      commandline = commandline.substring( 13 );
      uint8_t inputpin = commandline.toInt();
      commandline = commandline.substring( commandline.indexOf( " " ) );
      uint8_t outputpin = commandline.toInt();

      command_test_gate_1to1( inputpin , outputpin );
  }


  if ( commandline.startsWith( "sweepgate1to1" ) ) { 

        commandline = commandline.substring( 14 );
        uint8_t inputpin = commandline.toInt();
        commandline = commandline.substring( commandline.indexOf( " " ) );
        uint8_t outputpin = commandline.toInt();

        command_sweep_gate_1to1( inputpin , outputpin );
      }
  
  
  // test suitess
  
  if ( commandline.startsWith( "testself" ) ) { test_self(); }

  if ( commandline.startsWith( "test7400" ) ) { test_7400(); }


}








void command_i2cscan() {
  i2ctoolbox.scan();
}

void command_i2cbus( String commandline ) {
  commandline = commandline.substring( 7 );

  uint8_t busid = commandline.toInt();

  i2cbusmaster.enable_bus( busid );

  Serial.print( " i2C Bus set to " );
  Serial.println( busid );

}

void command_zifset( String commandline ) {

  i2cbusmaster.enable_bus(2);

  // strip off the command
  commandline = commandline.substring( 7 );

  // parse out the pin number
  uint8_t pinnumber = commandline.toInt();
  
  // strip the number off
  commandline = commandline.substring( commandline.indexOf(" ") + 1 );

  // get the bus number
  uint8_t busnumber = commandline.toInt();
  
  Serial.print( "Setting " );
  Serial.print( pinnumber );
  Serial.print( " to bus " );
  Serial.println( busnumber );

  i2czifsocklibobj.pin_to_bus( pinnumber , busnumber );

}

void command_zifena( String commandline ) {

  i2cbusmaster.enable_bus(2);

  // strip off the command
  commandline = commandline.substring( 7 );

  // parse out the pin number
  uint8_t pinnumber = commandline.toInt();

  Serial.print( "Pin enable: " );
  Serial.println( pinnumber );
  
  i2czifsocklibobj.enable_pin( pinnumber );

}

void command_zifdis( String commandline ) {

  i2cbusmaster.enable_bus(2);
  
  // strip off the command
  commandline = commandline.substring( 7 );

  // parse out the pin number
  uint8_t pinnumber = commandline.toInt();

  Serial.print( "Pin disable: " );
  Serial.println( pinnumber );
  
  i2czifsocklibobj.disable_pin( pinnumber );

}

void command_busport_get( String commandline ) {

  commandline = commandline.substring( 11 );

  uint8_t busid = commandline.toInt();


  Serial.print( "Bus id : " );
  Serial.println( busid );


  switch (busid)
  {
  case 0x00:
    /* code */
    break;

// bus objects

// 0 GND
// 1 VCC

// 2 MVC_LEDTOGND_0
// ina219 busobj_2_mvc_ledtognd_0;
case 0x02:

  i2cbusmaster.enable_bus( 1 );

  Serial.print( "Bus: " );
  Serial.println( i2cbusport.busobj_2_mvc_ledtognd_0.getbusvoltage() );
  Serial.print( "Shunt: " );
  Serial.println( i2cbusport.busobj_2_mvc_ledtognd_0.getshuntvoltage() );
  Serial.print( "Currrent: " );
  Serial.println( i2cbusport.busobj_2_mvc_ledtognd_0.getcurrent() );
  Serial.print( "Power: " );
  Serial.println( i2cbusport.busobj_2_mvc_ledtognd_0.getpower() );

  break;

// 3 MVC_LEDTOGND_1
// ina219 busobj_3_mvc_ledtognd_1;
case 0x03:

  i2cbusmaster.enable_bus( 0 );

  Serial.print( "Bus: " );
  Serial.println( i2cbusport.busobj_3_mvc_ledtognd_1.getbusvoltage() );
  Serial.print( "Shunt: " );
  Serial.println( i2cbusport.busobj_3_mvc_ledtognd_1.getshuntvoltage() );
  Serial.print( "Currrent: " );
  Serial.println( i2cbusport.busobj_3_mvc_ledtognd_1.getcurrent() );
  Serial.print( "Power: " );
  Serial.println( i2cbusport.busobj_3_mvc_ledtognd_1.getpower() );

  break;

// 4 MVC_GND
// ina219 busobj_4_mvc_gnd;
case 0x04:

  i2cbusmaster.enable_bus( 1 );

  Serial.print( "Bus: " );
  Serial.println( i2cbusport.busobj_4_mvc_gnd.getbusvoltage() );
  Serial.print( "Shunt: " );
  Serial.println( i2cbusport.busobj_4_mvc_gnd.getshuntvoltage() );
  Serial.print( "Currrent: " );
  Serial.println( i2cbusport.busobj_4_mvc_gnd.getcurrent() );
  Serial.print( "Power: " );
  Serial.println( i2cbusport.busobj_4_mvc_gnd.getpower() );

  break;

// 5 MV_GND_0
// ina3221 busobj_567_mv_gnd_x;
case 0x05:

  i2cbusmaster.enable_bus( 1 );

  Serial.print( "Bus: " );
  Serial.println( i2cbusport.busobj_567_mv_gnd_x.get_bus_voltage( 0 ) );
  Serial.print( "Shunt: " );
  Serial.println( i2cbusport.busobj_567_mv_gnd_x.get_shunt_voltage( 0 ) );
  
  break;
// 6 MV_GND_1
// ina3221 busobj_567_mv_gnd_x;
case 0x06:

  i2cbusmaster.enable_bus( 1 );

  Serial.print( "Bus: " );
  Serial.println( i2cbusport.busobj_567_mv_gnd_x.get_bus_voltage( 1 ) );
  Serial.print( "Shunt: " );
  Serial.println( i2cbusport.busobj_567_mv_gnd_x.get_shunt_voltage( 1 ) );
  
  break;
// 7 MV_GND_2
// ina3221 busobj_567_mv_gnd_x;
case 0x07:

  i2cbusmaster.enable_bus( 1 );

  Serial.print( "Bus: " );
  Serial.println( i2cbusport.busobj_567_mv_gnd_x.get_bus_voltage( 2 ) );
  Serial.print( "Shunt: " );
  Serial.println( i2cbusport.busobj_567_mv_gnd_x.get_shunt_voltage( 2 ) );
  
  break;





// 8 MV_VCC_0
// ina3221 busobj_89A_mv_vcc;
case 0x08:

  i2cbusmaster.enable_bus( 1 );

  Serial.print( "Bus: " );
  Serial.println( i2cbusport.busobj_89A_mv_vcc.get_bus_voltage( 0 ) );
  Serial.print( "Shunt: " );
  Serial.println( i2cbusport.busobj_89A_mv_vcc.get_shunt_voltage( 0 ) );
  
  break;
// 9 MV_VCC_1
// ina3221 busobj_89A_mv_vcc;
case 0x09:

  i2cbusmaster.enable_bus( 1 );

  Serial.print( "Bus: " );
  Serial.println( i2cbusport.busobj_89A_mv_vcc.get_bus_voltage( 0 ) );
  Serial.print( "Shunt: " );
  Serial.println( i2cbusport.busobj_89A_mv_vcc.get_shunt_voltage( 0 ) );
  
  break;
// A MV_VCC_2
// ina3221 busobj_89A_mv_vcc;
case 0x0A:

  i2cbusmaster.enable_bus( 1 );

  Serial.print( "Bus: " );
  Serial.println( i2cbusport.busobj_89A_mv_vcc.get_bus_voltage( 0 ) );
  Serial.print( "Shunt: " );
  Serial.println( i2cbusport.busobj_89A_mv_vcc.get_shunt_voltage( 0 ) );
  
  break;


// B MVC_VCC
// ina219 busobj_B_mvc_vcc;
case 0x0B:

  i2cbusmaster.enable_bus( 0 );

  Serial.print( "Bus: " );
  Serial.println( i2cbusport.busobj_B_mvc_vcc.getbusvoltage() );
  Serial.print( "Shunt: " );
  Serial.println( i2cbusport.busobj_B_mvc_vcc.getshuntvoltage() );
  Serial.print( "Currrent: " );
  Serial.println( i2cbusport.busobj_B_mvc_vcc.getcurrent() );
  Serial.print( "Power: " );
  Serial.println( i2cbusport.busobj_B_mvc_vcc.getpower() );

  break;


// C V_MVC_VCC_0
// ina219 busobj_C_v_mvc_vcc_0;
// mcp4725 busobj_C_v_mvc_vcc_0_dac;
case 0x0C:

  i2cbusmaster.enable_bus( 0 );

  Serial.print( "Bus: " );
  Serial.println( i2cbusport.busobj_C_v_mvc_vcc_0.getbusvoltage() );
  Serial.print( "Shunt: " );
  Serial.println( i2cbusport.busobj_C_v_mvc_vcc_0.getshuntvoltage() );
  Serial.print( "Currrent: " );
  Serial.println( i2cbusport.busobj_C_v_mvc_vcc_0.getcurrent() );
  Serial.print( "Power: " );
  Serial.println( i2cbusport.busobj_C_v_mvc_vcc_0.getpower() );

  Serial.print( "DAC: " );
  Serial.println ( i2cbusport.busobj_C_v_mvc_vcc_0_dac.getvalue() );

  break;


// D V_MVC_VCC_1
// ina219 busobj_D_v_mvc_vcc_1;
// mcp4725 busobj_D_v_mvc_vcc_1_dac;
case 0x0D:

  i2cbusmaster.enable_bus( 0 );

  Serial.print( "Bus: " );
  Serial.println( i2cbusport.busobj_D_v_mvc_vcc_1.getbusvoltage() );
  Serial.print( "Shunt: " );
  Serial.println( i2cbusport.busobj_D_v_mvc_vcc_1.getshuntvoltage() );
  Serial.print( "Currrent: " );
  Serial.println( i2cbusport.busobj_D_v_mvc_vcc_1.getcurrent() );
  Serial.print( "Power: " );
  Serial.println( i2cbusport.busobj_D_v_mvc_vcc_1.getpower() );

  Serial.print( "DAC: " );
  Serial.println ( i2cbusport.busobj_D_v_mvc_vcc_1_dac.getvalue() );
  
  break;


// E LEDTOGND
// F NC




  default:
    break;
  }








};

void command_busport_set( String commandline ) {

    commandline = commandline.substring( 11 );

  uint8_t busid = commandline.toInt();

  commandline = commandline.substring( commandline.indexOf( " " ) );

  uint16_t valuetoset = commandline.toInt();

  Serial.print( "Bus id : " );
  Serial.println( busid );

  switch (busid)
  {
  case 0x0C:
    i2cbusmaster.enable_bus( 0 );
    i2cbusport.busobj_C_v_mvc_vcc_0_dac.setvalue( valuetoset );
    break;
  case 0x0D:
    i2cbusmaster.enable_bus( 0 );
    i2cbusport.busobj_D_v_mvc_vcc_1_dac.setvalue( valuetoset );
    break;
  
  default:
    break;
  }

};

void command_test_gate_1to1( uint8_t inputpin , uint8_t outputpin ) {


  Serial.print( "Single gate test 1 to 1.  Using input " );
  Serial.print( inputpin );
  Serial.print( " to output " );
  Serial.print( outputpin );
  Serial.println( "." );
  Serial.println();
  Serial.println( "Input\tOutput");

  // connect input pin to V_MVC_VCC_0
  i2cbusmaster.enable_bus(2);
  i2czifsocklibobj.pin_to_bus( inputpin , BUSPORT_V_MVC_VCC_0 );

  // connect output pin to MVC_LEDTOGND_0
  i2cbusmaster.enable_bus(2);
  i2czifsocklibobj.pin_to_bus( outputpin , BUSPORT_MVC_LEDTOGND_0 );

  // enable input pin
  i2cbusmaster.enable_bus(2);
  i2czifsocklibobj.enable_pin( inputpin );

  // enable output pin
  i2cbusmaster.enable_bus(2);
  i2czifsocklibobj.enable_pin( outputpin );

  // set V_MVC_VCC_0 to 0
  i2cbusmaster.enable_bus(0);
  i2cbusport.busobj_C_v_mvc_vcc_0_dac.setvalue( 0 );
  delay(1);
  Serial.print( "0\t" );

  // measure MVC_LEDTOGND_0
  i2cbusmaster.enable_bus(1);
  Serial.println( i2cbusport.busobj_2_mvc_ledtognd_0.getbusvoltage() > 2.5 ? "1" : "0" );

  // set V_MVC_VCC_0 to 4095
  i2cbusmaster.enable_bus(0);
  i2cbusport.busobj_C_v_mvc_vcc_0_dac.setvalue( 4095 );
  delay(1);
  Serial.print( "1\t" );

  // measure MVC_LEDTOGND_0
  i2cbusmaster.enable_bus(1);
  Serial.println( i2cbusport.busobj_2_mvc_ledtognd_0.getbusvoltage() > 2.5 ? "1" : "0" );

  // now turn things back off
  
  // turn off V_MVC_VCC_0
  i2cbusmaster.enable_bus(0);
  i2cbusport.busobj_C_v_mvc_vcc_0_dac.setvalue( 0 );

  // turn off the input port
  i2cbusmaster.enable_bus(2);
  i2czifsocklibobj.disable_pin( inputpin );
  i2czifsocklibobj.pin_to_bus( inputpin , BUSPORT_GND );

  // turn off the output port
  i2cbusmaster.enable_bus(2);
  i2czifsocklibobj.disable_pin( outputpin );
  i2czifsocklibobj.pin_to_bus( outputpin , BUSPORT_GND );

  // and now return
  Serial.println("Done!");
}

void command_sweep_gate_1to1( uint8_t inputpin , uint8_t outputpin ) {


  Serial.print( "Single gate sweep test 1 to 1.  Using input " );
  Serial.print( inputpin );
  Serial.print( " to output " );
  Serial.print( outputpin );
  Serial.println( "." );
  Serial.println();



  // first, set up the pins

  // set input pin to v_mvc_vcc
  i2cbusmaster.enable_bus(2);
  i2czifsocklibobj.pin_to_bus( inputpin , BUSPORT_V_MVC_VCC_0 );

  // set outputpin to mvc_ledtognd_0
  i2czifsocklibobj.pin_to_bus( outputpin , BUSPORT_MVC_LEDTOGND_0 );

  // set v_mvc_vcc to 0
  i2cbusmaster.enable_bus(0);
  i2cbusport.busobj_C_v_mvc_vcc_0_dac.setvalue(0);

  // enable input pin
  i2cbusmaster.enable_bus(2);
  i2czifsocklibobj.enable_pin( inputpin );

  // enable output pin
  i2czifsocklibobj.enable_pin( outputpin );



  Serial.println( "loop\tBus\tShunt\tCurr\tPow\tBus\tShunt\tCurr\tPow");




  // foreach 0 4096
  for ( uint16_t loopcounter = 0 ; loopcounter < 4096 ; loopcounter++ ) {
    
    // set v_mvc_vcc to loopcounter
    i2cbusmaster.enable_bus( 0 );
    i2cbusport.busobj_C_v_mvc_vcc_0_dac.setvalue( loopcounter );
    Serial.print( loopcounter ); Serial.print( "\t");
    delay(1);

    // measure v_mvc_vcc
    Serial.print( i2cbusport.busobj_C_v_mvc_vcc_0.getbusvoltage() ); Serial.print( "\t" );
    Serial.print( i2cbusport.busobj_C_v_mvc_vcc_0.getshuntvoltage() ); Serial.print( "\t" );
    Serial.print( i2cbusport.busobj_C_v_mvc_vcc_0.getcurrent() ); Serial.print( "\t" );
    Serial.print( i2cbusport.busobj_C_v_mvc_vcc_0.getpower() ); Serial.print( "\t" );

    // measure mvc_ledtognd_0
    i2cbusmaster.enable_bus(1);
    Serial.print( i2cbusport.busobj_2_mvc_ledtognd_0.getbusvoltage() ); Serial.print( "\t" );
    Serial.print( i2cbusport.busobj_2_mvc_ledtognd_0.getshuntvoltage() ); Serial.print( "\t" );
    Serial.print( i2cbusport.busobj_2_mvc_ledtognd_0.getcurrent() ); Serial.print( "\t" );
    Serial.print( i2cbusport.busobj_2_mvc_ledtognd_0.getpower() ); Serial.print( "\t" );

    Serial.println();

  }
  

  // test complete

  // set dac to zero
  i2cbusmaster.enable_bus( 0 );
  i2cbusport.busobj_C_v_mvc_vcc_0_dac.setvalue( 0 );

  // disable input pin
  i2cbusmaster.enable_bus( 2 );
  i2czifsocklibobj.disable_pin( inputpin );

  // set input pin to bus zero
  i2czifsocklibobj.pin_to_bus( inputpin , BUSPORT_NC );

  // disable output pin
  i2czifsocklibobj.disable_pin( outputpin );

  // set output pin to zero
  i2czifsocklibobj.pin_to_bus( outputpin , BUSPORT_NC );
  


}





uint8_t command_test_gate_2to1( uint8_t inputpin_0 , uint8_t inputpin_1 , uint8_t outputpin ) {

  // configure pins

  // input 1
  i2cbusport.switchto( BUSPORT_V_MVC_VCC_0 );
  i2cbusport.busobj_C_v_mvc_vcc_0_dac.setvalue( 0 );
  i2cbusmaster.enable_bus( 2 );
  i2czifsocklibobj.pin_to_bus( inputpin_0 , BUSPORT_V_MVC_VCC_0 );
  i2czifsocklibobj.enable_pin( inputpin_0 );
  

  // input 2
  i2cbusport.switchto( BUSPORT_V_MVC_VCC_1 );
  i2cbusport.busobj_D_v_mvc_vcc_1_dac.setvalue( 0 );
  i2cbusmaster.enable_bus( 2 );
  i2czifsocklibobj.pin_to_bus( inputpin_1 , BUSPORT_V_MVC_VCC_1 );
  i2czifsocklibobj.enable_pin( inputpin_1 );

  // output
  i2cbusmaster.enable_bus( 2 );
  i2czifsocklibobj.pin_to_bus( outputpin , BUSPORT_MVC_LEDTOGND_0 );
  i2czifsocklibobj.enable_pin( outputpin );

  // create somewhere to store the results
  uint8_t result = 0;


  for ( uint8_t loopcounter = 0 ; loopcounter < 4 ; loopcounter++ ) {

      // set input 1
      i2cbusport.switchto( BUSPORT_V_MVC_VCC_0 );
      i2cbusport.busobj_C_v_mvc_vcc_0_dac.setvalue( loopcounter & 0b1 ? 4095 : 0 );

      // set input 2
      i2cbusport.switchto( BUSPORT_V_MVC_VCC_1 );
      i2cbusport.busobj_D_v_mvc_vcc_1_dac.setvalue( ( loopcounter & 0b10 ) >> 1 ? 4095 : 0 );

      // check output
      i2cbusport.switchto( BUSPORT_MVC_LEDTOGND_0 );
      result |= ( i2cbusport.busobj_2_mvc_ledtognd_0.getbusvoltage() > 2.5 ? 1 : 0 ) << loopcounter;

  }

  return result;

}





void test_self() {

  Serial.print( "Self Test!" );

  command_test_gate_1to1( 0 , 15 );
  command_test_gate_1to1( 1 , 14 );
  command_test_gate_1to1( 2 , 13 );
  command_test_gate_1to1( 3 , 12 );
  command_test_gate_1to1( 4 , 11 );
  command_test_gate_1to1( 5 , 10 );
  command_test_gate_1to1( 6 , 9 );
  command_test_gate_1to1( 7 , 8 );
  

}




void test_7400() {

  Serial.println("Testing 7400 Chip!");

  uint8_t dut_vcc = 15;
  uint8_t dut_gnd = 6;

  uint8_t dut_gate[4][3] = {  
                              { 0 , 1 , 2 } ,
                              { 3 , 4 , 5 } ,
                              { 11 , 10 , 9 } ,
                              { 14 , 13 , 12 }
                           };


  // set up the chip

  // switch to busport
  i2cbusmaster.enable_bus(2);

  // vcc
  i2czifsocklibobj.pin_to_bus( dut_vcc , BUSPORT_MVC_VCC );
  i2czifsocklibobj.enable_pin( dut_vcc );

  // gnd
  i2czifsocklibobj.pin_to_bus( dut_gnd , BUSPORT_MVC_LEDTOGND_1 );
  i2czifsocklibobj.enable_pin( dut_gnd );

  // set pins to input high so output low
  for ( uint8_t loopcounter = 0 ; loopcounter < 4 ; loopcounter++ ) {
    i2czifsocklibobj.pin_to_bus( dut_gate[loopcounter][0] , BUSPORT_VCC );
    i2czifsocklibobj.enable_pin( dut_gate[loopcounter][0] );
    i2czifsocklibobj.pin_to_bus( dut_gate[loopcounter][1] , BUSPORT_VCC );
    i2czifsocklibobj.enable_pin( dut_gate[loopcounter][1] );
    i2czifsocklibobj.pin_to_bus( dut_gate[loopcounter][2] , BUSPORT_GND );
    i2czifsocklibobj.enable_pin( dut_gate[loopcounter][2] );
  }
  
  // now the chip is powered on, and all gates are in the off state.


  for ( uint8_t loopcounter = 0 ; loopcounter < 4 ; loopcounter++ ) {
    Serial.print( "Gate " );
    Serial.print( loopcounter );
    Serial.print( " test: " );
    uint8_t result = command_test_gate_2to1( dut_gate[loopcounter][0] , dut_gate[loopcounter][1] , dut_gate[loopcounter][2] );
    showbin( result );
    Serial.print( " : " );
    Serial.print( result == 0b00000111 ? "Pass." : "Fail!" );
    Serial.println();
  }







    // set up the chip

  // switch to busport
  i2cbusmaster.enable_bus(2);

  // vcc
  i2czifsocklibobj.pin_to_bus( dut_vcc , BUSPORT_MVC_VCC );
  i2czifsocklibobj.enable_pin( dut_vcc );

  // gnd
  i2czifsocklibobj.pin_to_bus( dut_gnd , BUSPORT_MVC_LEDTOGND_1 );
  i2czifsocklibobj.enable_pin( dut_gnd );

  // set pins to input high so output low
  for ( uint8_t loopcounter = 0 ; loopcounter < 4 ; loopcounter++ ) {
    i2czifsocklibobj.pin_to_bus( dut_gate[loopcounter][0] , BUSPORT_VCC );
    i2czifsocklibobj.enable_pin( dut_gate[loopcounter][0] );
    i2czifsocklibobj.pin_to_bus( dut_gate[loopcounter][1] , BUSPORT_VCC );
    i2czifsocklibobj.enable_pin( dut_gate[loopcounter][1] );
    i2czifsocklibobj.pin_to_bus( dut_gate[loopcounter][2] , BUSPORT_GND );
    i2czifsocklibobj.enable_pin( dut_gate[loopcounter][2] );
  }


  // turn on gate 0
  i2czifsocklibobj.pin_to_bus( dut_gate[0][0] , BUSPORT_GND );
  i2czifsocklibobj.pin_to_bus( dut_gate[0][1] , BUSPORT_GND );
  i2czifsocklibobj.pin_to_bus( dut_gate[0][2] , BUSPORT_MVC_LEDTOGND_0 );
  
  // print VCC reference
  i2cbusport.switchto( BUSPORT_MVC_VCC );
  Serial.print( "VCC: " );
  Serial.println( i2cbusport.busobj_B_mvc_vcc.getbusvoltage() );
  
  // print gate output
  i2cbusport.switchto( BUSPORT_MVC_LEDTOGND_0 );
  Serial.print( "Gate: " );
  Serial.println( i2cbusport.busobj_2_mvc_ledtognd_0.getbusvoltage() );


  // disconnect!
  i2cbusmaster.enable_bus(2);
  for ( uint8_t loopcounter = 0 ; loopcounter < 16 ; loopcounter++ ) {
    i2czifsocklibobj.pin_to_bus( loopcounter , BUSPORT_NC );
    i2czifsocklibobj.disable_pin( loopcounter );
  }



}


