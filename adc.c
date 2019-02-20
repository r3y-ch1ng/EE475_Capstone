/*******************************************************************************/
/* optfft.c                                                                    */
/*                                                                             */
/* An optimized version of the fft function using only 16-bit integer math.    */
/*                                                                             */
/* Optimized by Brent Plump                                                    */
/* Based heavily on code by Jinhun Joung                                       */
/*                                                                             */
/* - Works only for input arrays of 256 length.                                */
/* - Requires two arrays of 16-bit ints.  The first contains the samples, the  */
/*   second contains all zeros.  The samples range from -31 to 32              */
/* - Returns the index of the peak frequency                                   */
/*******************************************************************************/
#include "adc.h"
#include "lcd.h"
#include "timer.h"
#include <stdint.h>
#include <xc.h>
#include "serial.h"

#define _XTAL_FREQ 16000000

void ADC_Init() {
	ADCON0 = 0x81;
	ADCON1 = 0x80;
}

unsigned int ADC_Read(unsigned char channel) {
	if(channel > 7)
	return 0;
	ADCON0 &= 0xC5;
	ADCON0 |= channel<<3;
	__delay_ms(2);
	GO_nDONE = 1;
	while(GO_nDONE);
	return ((ADRESH<<8)+ADRESL);
}
