#include "timer.h"
#include "lcd.h"
#include <xc.h>
#include "main.h"

#define _XTAL_FREQ 16000000

void Timer0_Init(void){
	time_ms = 0; //time in milliseconds
	T0CONbits.T08BIT = 0; //16-bit timer, the more bits the better!
	TMR0H = 0xFF;
	TMR0L = 0x34;
	INTCONbits.TMR0IF = 0; //Interrupt flags are 0
	T0CON = 0x00; //Timer mode, enabled prescalar of 1:2, increment on low to high transition, transition on T0CLKI pin (4 MHz). Timer off
}

unsigned long ReadTime(void) {
	return time_ms;
}

int Timer0_StartTimer(void) {
	T0CONbits.TMR0ON = 1; //Timer on
	return 0;
}

int Timer0_StopTimer(void) {
	T0CONbits.TMR0ON = 0; //Timer off
	return 0;
}

void __interrupt () timingISR(void) {
	if (TMR0IF && TMR0IE) {
		TMR0IF = 0; //Manually reset this bit
		time_ms++;
		TMR0H = 0xFF;
		TMR0L = 0x34;
		Timer0_StartTimer();
	}
	return;
}
