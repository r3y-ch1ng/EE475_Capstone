#include <xc.h>
#include "pwm.h"
#define _XTAL_FREQ = 4000000

void initialize_PWM() {
  PR2 = 0xFF;
  CCPR1L = 0xFF;
  CCP1CON |= 0x30; //Bits 4 and 5 are the LSB duty cycle of CCPR1L. I'm just gonna set them now, less duty cycle. 0x30
  T2CON = 0x06; //T2 is now on with 1:16 prescalar value.
  CCP1CON |= 0x0C; //Configure for PWM.
  TRISCbits.TRISC2 = 0;
}

void set_duty_cycle(char upper_8, char lower_2) {
  CCPR1L = upper_8;
  char mask = 0;
  mask |= lower_2;
  mask = mask << 4;
  CCP1CON = (CCP1CON & ~(0x30)) | mask;
}
