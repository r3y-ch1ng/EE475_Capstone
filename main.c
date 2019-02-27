/*
* File:   main.c
* Author: reyching
*
* Created on January 18, 2019, 2:35 PM
*/

#include <xc.h>
#include <stdio.h>
#include <time.h>
#include <stddef.h>
#include <stdint.h>
#include "lcd.h"
#include "timer.h"
#include "serial.h"
#include "drive_memory.h"
#include "usart.h"
#include "adc_temp.h"
#include "adc.h"
#include "pwm.h"
#include "config.h"
#include "main.h"
#define _XTAL_FREQ 16000000

LCD lcd;

void main (void) {
  TRISCbits.TRISC2 = 0;
  TMR2IE = 1;
  TMR2IP = 1;
  RCONbits.IPEN = 1;
  initialize_PWM(0xFF);
  set_duty_cycle(0xA0, 0x03);
  int temperature;
  Timer0_Init();
  Timer0_StartTimer();
  initialize_TX();
  initialize_RX();
  TRISCbits.TRISC7 = 1; //C7 is an input RX.
  TRISCbits.TRISC6 = 0; //C6 is an output TX
  while(1) {
    TRISCbits.TRISC7 = 1; //C7 is an input RX.
    char input = ' ';
    while (input < 'a' || input > 'z') {
      input = UARTRecieveChar();
    }
    switch (input) {
      case 's':
      TRISCbits.TRISC7 = 0;
      temperature = (int) get_temp();
      int curr_time = (time_ms/60/1000);
      UARTSendString("Writing ");
      UARTSendString(int_to_char(temperature));
      UARTSendString(" to address ");
      UARTSendString(int_to_char(address));
      UARTNewLine();
      write_op(address, temperature);
      __delay_ms(100);
      UARTSendString("Writing ");
      UARTSendString(int_to_char(curr_time));
      UARTSendString(" to address ");
      UARTSendString(int_to_char(address));
      UARTNewLine();
      // if (temp < 100) set_duty_cycle(0xA0, 0x03);
      // else set_duty_cycle(0xFF, 0x03);

      write_op(address + 1, curr_time);
      address = (address + 2) % 16;
      UARTSendString("Done writing to memory.");
      UARTNewLine();
      __delay_ms(1000);
      break;
      case 'r':
      // stored_temp = read_op(address - 2);
      // int time_elapsed = read_op(address - 2 + 1);
      stored_temp = read_op(0);
      int time_elapsed = read_op(1);
      TRISCbits.TRISC7 = 0;
      UARTSendString("Temperature at address ");
      UARTSendString(int_to_char(address - 2));
      UARTSendString(": ");
      UARTSendString(int_to_char(stored_temp));
      UARTNewLine();
      UARTSendString("Time at address ");
      UARTSendString(int_to_char(address - 2 + 1));
      UARTSendString(": ");
      UARTSendString(int_to_char(time_elapsed));
      UARTNewLine();
      UARTSendString("Done reading from memory.");
      UARTNewLine();
      UARTNewLine();
      __delay_ms(1000);
      break;
      default:
      break;

    }
  }
}

void read_SRAM(int address) {
  int stored_temp = read_op(address);
  int time_elapsed = read_op(address + 1);
  TRISCbits.TRISC7 = 0;
  UARTSendString("Temperature at address ");
  UARTSendString(int_to_char(address));
  UARTSendString(": ");
  UARTSendString(int_to_char(stored_temp));
  UARTNewLine();
  UARTSendString("Time at address ");
  UARTSendString(int_to_char(address + 1));
  UARTSendString(": ");
  UARTSendString(int_to_char(time_elapsed));
  UARTNewLine();
  UARTSendString("Done reading from memory.");
  UARTNewLine();
  UARTNewLine();
  __delay_ms(1000);
}

// void main() {
//  INTCONbits.RBIE = 0;
//  INTCON2bits.RBPU = 1;
//
//  initialize_TX();
//  initialize_RX();
//  TRISCbits.TRISC7 = 1; //C7 is an input RX.
//  TRISCbits.TRISC6 = 0; //C6 is an output TX
//  TRISCbits.TRISC7 = 0; //C7 is an input RX.
//  while (1) {
//    write_op(3, 3);
//    __delay_ms(1000);
//    write_op(2, 2);
//    __delay_ms(1000);
//    write_op(1, 1);
//    UARTSendString(int_to_char(read_op(3)));
//    UARTNewLine();
//    __delay_ms(1000);
//    UARTSendString(int_to_char(read_op(2)));
//    UARTNewLine();
//    __delay_ms(1000);
//    UARTSendString(int_to_char(read_op(1)));
//    UARTNewLine();
//    __delay_ms(1000);
//
//  }
// }
