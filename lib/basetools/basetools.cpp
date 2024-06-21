

#ifndef BASETOOLS_BODY_GUARD
#define BASETOOLS_BODY_GUARD

#include <basetools.h>


void show4bithex( uint8_t hexvalue ) {
  switch (hexvalue) {
  case 0b0000: Serial.print("0"); break;
  case 0b0001: Serial.print("1"); break;
  case 0b0010: Serial.print("2"); break;
  case 0b0011: Serial.print("3"); break;
  case 0b0100: Serial.print("4"); break;
  case 0b0101: Serial.print("5"); break;
  case 0b0110: Serial.print("6"); break;
  case 0b0111: Serial.print("7"); break;
  case 0b1000: Serial.print("8"); break;
  case 0b1001: Serial.print("9"); break;
  case 0b1010: Serial.print("A"); break;
  case 0b1011: Serial.print("B"); break;
  case 0b1100: Serial.print("C"); break;
  case 0b1101: Serial.print("D"); break;
  case 0b1110: Serial.print("E"); break;
  case 0b1111: Serial.print("F"); break;
  }
}


void showhex( uint8_t hexvalue ) {
  show4bithex( hexvalue >> 4 );
  show4bithex( hexvalue & 0b1111 );
}

void showhex( uint16_t hexvalue ) {
  showhex( (uint8_t)( hexvalue >> 8 ) );
  showhex( (uint8_t)hexvalue );
}



void showbin( uint8_t binvalue ) {
  Serial.print( binvalue & 0b10000000 ? 1 : 0 );
  Serial.print( binvalue & 0b01000000 ? 1 : 0 );
  Serial.print( binvalue & 0b00100000 ? 1 : 0 );
  Serial.print( binvalue & 0b00010000 ? 1 : 0 );
  Serial.print( binvalue & 0b00001000 ? 1 : 0 );
  Serial.print( binvalue & 0b00000100 ? 1 : 0 );
  Serial.print( binvalue & 0b00000010 ? 1 : 0 );
  Serial.print( binvalue & 0b00000001 ? 1 : 0 );
}

void showbin( uint16_t binvalue ) {
  showbin( (uint8_t)( binvalue >> 8 ) );
  showbin( (uint8_t)binvalue );
}


#endif