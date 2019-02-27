//Port E1 will be the !WE, Port E2 will be the !OE
#include "drive_memory.h"
#include <xc.h>
#include "serial.h"
#include "lcd.h"

#define _XTAL_FREQ 16000000

void write_op(char address, char data) {
  /*Initial conditions*/
  TRISC &= ~(0x01);
  TRISD = 0x00; // Data bits. All output
  TRISB &= ~(0x3f); // Lower 5 bits are the address, RB5 = !WE and RB6 = !OE
  PORTB = address;
  RB5 = 0; // Output enabled.
  //RB4 = 1; // Write disabled at first.
  RC0 = 1;

  /*Changing signals*/
  // RB4 = 0; // Write enabled after a delay.
  RC0 = 0;
  __delay_us(0.025); // Delay at least 25 ns before presenting data to the chip.
  PORTD = data;
  __delay_us(0.03); // Delay at least 30 ns before disabling write.
  // RB4 = 1; // Write disabled once again.
  RC0 = 1;
  /*At this point the chip has outputted the written data. */
}

char read_op(char address) {
  /*Initial conditions*/
  TRISC &= ~(0x01);
  TRISD = 0xFF; // Data bits. All input
  TRISB &= ~(0x3f); // Lower 5 bits are the address, RA5 = !WE and RA6 = !OE
  PORTB = address;
  RB5 = 1; // Output disabled.
  // RB4 = 1; // Write disabled.
  RC0 = 1;
  /*Changing signals*/
  RB5 = 0; // Output enabled.
  __delay_us(0.07); //Delay at least 70 ns.
  /*At this point data is outputted FROM the chip*/

  RB5 = 1; // Output enabled.

  char result = PORTD;
  return result;
}
