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

#define _XTAL_FREQ 16000000

//CONFIG
#pragma config WDT = OFF

LCD lcd;

void main (void) {
  int address = 0;
  Timer0_Init();
  Timer0_StartTimer();
  initialize_TX();
  initialize_RX();
  TRISCbits.TRISC7 = 1; //C7 is an input RX.
  TRISCbits.TRISC6 = 0; //C6 is an output TX
  int temp = (int) get_temp();
  write_op(address, temp);
  address++;
  TRISCbits.TRISC7 = 0;
  UARTSendString(int_to_char(temp));
  UARTNewLine();
  __delay_ms(750);
//  write_op(address, (int) (time_ms/60));
//  address--;
//  int stored_temp = read_op(address++);
//  int time_elapsed = read_op(address);
//  TRISCbits.TRISC7 = 0;
//  UARTSendString(int_to_char(stored_temp));
//  UARTNewLine();
//  UARTSendString(int_to_char(time_elapsed));
//  UARTNewLine();
}
