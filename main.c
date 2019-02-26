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
#define _XTAL_FREQ 16000000

LCD lcd;

void main (void) {
  TRISCbits.TRISC2 = 0;
  TMR2IE = 1;
  TMR2IP = 1;
  RCONbits.IPEN = 1;
  initialize_PWM(0xFF);
  while(1) {
    int address = 0;
    Timer0_Init();
    Timer0_StartTimer();
    initialize_TX();
    initialize_RX();
    TRISCbits.TRISC7 = 1; //C7 is an input RX.
    TRISCbits.TRISC6 = 0; //C6 is an output TX
    int temp = (int) get_temp();
    //write_op(address, temp);
    address++;
    TRISCbits.TRISC7 = 0;
    if (temp < 100) set_duty_cycle(0xA0, 0x03);
    else set_duty_cycle(0xFF, 0x03);
  }
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
