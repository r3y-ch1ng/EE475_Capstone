#include <xc.h>
#include "pwm.h"
#define _XTAL_FREQ 16000000

void initialize_PWM(int freq) {
  T2CON &= ~(0x40);
  PR2 = 0xFF;
  CCPR1L = 0xAA;
  CCP1CON |= 0x00; //Bits 4 and 5 are the LSB duty cycle of CCPR1L. I'm just gonna set them now, less duty cycle. 0x30
  T2CON = 0x07; //T2 is now on with 1:16 prescalar value.
  CCP1CON |= 0x0C; //Configure for PWM.
}

void set_freq(int freq) {
  PR2 = (_XTAL_FREQ/(freq*4*16)) - 1;
}

void set_duty_cycle(char upper_8, char lower_2) {
  CCPR1L = upper_8;
  char mask = 0;
  mask |= lower_2;
  mask = mask << 4;
  CCP1CON = (CCP1CON & ~(0x30)) | mask;
}
