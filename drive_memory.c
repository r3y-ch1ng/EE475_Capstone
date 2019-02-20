//Port E1 will be the !WE, Port E2 will be the !OE
#include "drive_memory.h"
#include <xc.h>
#include "serial.h"
#include "lcd.h"

#define _XTAL_FREQ 16000000

//RC1 is !CE, RC2 is !OE, RC3 is !WE

//RC1 is !WE, RC2 is !OE

void write_op(char address, char data) {
  __delay_ms(10);
  TRISD = 0x00; //Rip out the LCD.
  TRISB &= ~(0x3f); //lots of outputs
  TRISCbits.TRISC1 = 0; //!WE
  //PORTCbits.RC1 = 0; //chip enabled
  PORTCbits.RC2 = 1; //output disabled
  PORTCbits.RC1 = 0; //write-enabled
  PORTB = address;
  PORTD = data;
  __delay_ms(10);
  //PORTCbits.RC1 = 1; //chip disabled at the end of read
  __delay_ms(10);
}

char read_op(char address) {
  __delay_ms(10);
  TRISD = 0xff; //input
  PORTB = address;
  PORTCbits.RC1 = 1; //write-disabled
  PORTCbits.RC2 = 0; //output-enabled
  //PORTB |= (address & 0x3f); //select address
  //PORTCbits.RC1 = 0; //!CE goes high at the end of read
  //PORTB = 0x00;
  char result = 0x00;
  char mask = 0x01;
  for (int i = 0; i < 8; i++) {
    result |= (PORTD & mask);
    mask = mask << 1;
  }
  __delay_ms(10);
  //PORTCbits.RC1 = 1; //!CE goes high at the end of read
  PORTCbits.RC2 = 1; //output-disabled
  TRISCbits.TRISC7 = 0;
  UARTSendString(int_to_char(result));
  UARTNewLine();
  return result;
}
