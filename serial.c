/*
 * File:   serial.c
 * Author: reyching
 *
 * Created on January 30, 2019, 10:40 AM
 */

#include <stdio.h>
#include <xc.h>
#include "serial.h"
#include "lcd.h"

#define _XTAL_FREQ 16000000

void interrupt_enable() {
  INTCONbits.GIE = 1;
  INTCONbits.PEIE = 1;
  INTCONbits.TMR0IE = 1;
  INTCON2bits.TMR0IP = 1;
  RCONbits.IPEN = 1;
}

void interrupt_disable() {
  INTCONbits.GIE = 0;
  INTCONbits.PEIE = 0;
  INTCONbits.TMR0IE = 0;
  INTCON2bits.TMR0IP = 0;
}

void initialize_TX() {
  TXSTAbits.TX9 = 1; //9-bit
  TXSTAbits.TXEN = 1;
  TXSTAbits.SYNC = 0;
  TXSTAbits.BRGH = 0; //Divisor is supposedly 16... No hi-speed
  TXSTAbits.TX9D = 1;
  SPBRG = 25;
}

void initialize_RX() {
  RCSTA = 0x00;
  RCSTAbits.SPEN = 1; //Enable the pin
  RCSTAbits.RX9 = 1; //9-bit
  RCSTAbits.CREN = 1; //Continuous reception
  RCSTAbits.ADDEN = 0; //Allow space for a possible parity bit
  RCSTAbits.RX9D = 1;
}

void UARTNewLine() {
  TRISCbits.TRISC7 = 0;
  UARTSendChar(0x0a);
  UARTSendChar(0x0d);
}

void UARTSendChar(char c) {
  TRISCbits.TRISC7 = 0;
  while (TXSTAbits.TRMT == 0);    // Wait for buffer to be empty
  TXREG = c;
}

char UARTRecieveChar() {
  while (RCIF == 0) { // wait for char
    if (RCSTAbits.OERR == 1) {
      RCSTAbits.OERR = 0; // clear overrun if it occurs
      RCSTAbits.CREN = 0;
      RCSTAbits.CREN = 1;
      __delay_ms(10);
    }
  }
  return RCREG1;
}

void UARTSendString(char* c) {
  TRISCbits.TRISC7 = 0;
  int i = 0;
  while (c[i] != '\0') {
    UARTSendChar(c[i]);
    i++;
  }
}
