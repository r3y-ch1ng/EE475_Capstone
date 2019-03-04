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
  interrupt_enable();
  TRISCbits.TRISC2 = 0;
  TMR2IE = 1;
  TMR2IP = 1;
  Timer0_Init();
  Timer0_StartTimer();
  initialize_TX();
  initialize_RX();
  TRISCbits.TRISC7 = 1; //C7 is an input RX.
  TRISCbits.TRISC6 = 0; //C6 is an output TX
  while(1) {
    TRISCbits.TRISC7 = 1; //C7 is an input RX.
    char input = '`';
    while (input < 'a' || input > 'z') {
     input = UARTRecieveChar();
    }
    switch (input) {
      case 's':
      write_SRAM();
      break;
      case 'r':
      TRISCbits.TRISC7 = 0;
      input = ' ';
      while (input < '0' || input > '9') {
        input = UARTRecieveChar();
      }
      int input_address = input & 0xF;
      read_SRAM(input_address);
      break;
      case 'x':
      interrupt_enable();
      interrupt_disable();
      TMR2IE = 1;
      TMR2IP = 1;
      initialize_PWM(0xFF);
      set_duty_cycle(0x00, 0x00);
      while (1) {
        int temp = (int) get_temp();
        TRISCbits.TRISC7 = 0;
        UARTSendString(int_to_char(temp));
        UARTNewLine();
        if (temp < 30) set_duty_cycle(0x00, 0x00);
        else if (temp < 50) set_duty_cycle(0x0F, 0x03);
        else if (temp < 70) set_duty_cycle(0x13, 0x03);
        else if (temp < 100) set_duty_cycle(0x20, 0x03);
        else set_duty_cycle(0x3F, 0x03);
        __delay_ms(200);
      }
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
  UARTSendString(" degrees C ");
  UARTNewLine();
  UARTSendString("Time at address ");
  UARTSendString(int_to_char(address + 1));
  UARTSendString(": ");
  UARTSendString(int_to_char(time_elapsed));
  UARTSendString(" minutes");
  UARTNewLine();
  UARTSendString("Done reading from memory.");
  UARTNewLine();
  UARTNewLine();
}

void write_SRAM(){
  TRISCbits.TRISC7 = 0;
  int temperature = (int) get_temp();
  Timer0_StopTimer();
  int curr_time = (int) (ReadTime()/60/100/60);
  Timer0_StartTimer();
  UARTSendString("Writing ");
  UARTSendString(int_to_char(temperature));
  UARTSendString(" to address ");
  UARTSendString(int_to_char(address));
  UARTNewLine();
  write_op(address, temperature);
  UARTSendString("Writing ");
  UARTSendString(int_to_char(curr_time));
  UARTSendString(" to address ");
  UARTSendString(int_to_char(address + 1));
  UARTNewLine();
  write_op(address + 1, curr_time);
  address = (address + 2) % 16;
  UARTSendString("Done writing to memory.");
  UARTNewLine();
  UARTNewLine();
}

// void main() {
//   interrupt_enable();
//   interrupt_disable();
//   TRISCbits.TRISC2 = 0;
//   TMR2IE = 1;
//   TMR2IP = 1;
//   int temperature;
//   Timer0_Init();
//   Timer0_StartTimer();
//   initialize_TX();
//   initialize_RX();
//   TRISCbits.TRISC7 = 1; //C7 is an input RX.
//   TRISCbits.TRISC6 = 0; //C6 is an output TX
//   initialize_PWM(0xFF);
//   set_duty_cycle(0x00, 0x00);
//   while (1) {
//     int temp = (int) get_temp();
//     if (temp < 30) set_duty_cycle(0x00, 0x00);
//     else if (temp < 50) set_duty_cycle(0x0F, 0x03);
//     else if (temp < 50) set_duty_cycle(0xBF, 0x03);
//     else if (temp < 100) set_duty_cycle(0xF2, 0x03);
//     else set_duty_cycle(0xFF, 0x03);
//     __delay_ms(100);
//   }
// }
