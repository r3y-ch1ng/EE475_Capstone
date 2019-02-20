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
#include "optfft.h"
#include "lcd.h"
#include "tables.c"
#include "timer.h"
#include <pic18f452.h>
#include <stdint.h>
#include <xc.h>
#include "serial.h"

#define _XTAL_FREQ 16000000
#define ABS(x)  (((x)<0)?(-(x)):(x))
#define CEILING(x) (((x)>511)?511:(x))

signed int CaptureSignal[256];
signed int what[256];

unsigned int optfft(signed int real[256], signed int imag[256]) {
	unsigned int i, i1, j, l, l1, l2, t1, t2, u;
	/* Bit reversal. */
	/*Do the bit reversal */
	l2 = 128;
	i=0;
	for(l=0;l<255;l++) {
		if(l < i) {
			j=real[l];real[l]=real[i];real[i]=j;
		}
		l1 = l2;
		while (l1 <= i){
			i -= l1;
			l1 >>= 1;
		}
		i += l1;
	}

	/* Compute the FFT */
	u = 0;
	l2 = 1;
	for(l=0;l<8;l++){
		l1 = l2;
		l2 <<= 1;
		for(j=0;j<l1;j++){
			for(i=j;i<256;i+=l2){
				i1 = i + l1;
				t1 = (u1[u]*real[i1] - u2[u]*imag[i1])/32;
				t2 = (u1[u]*imag[i1] + u2[u]*real[i1])/32;
				real[i1] = real[i]-t1;
				imag[i1] = imag[i]-t2;
				int z = real[i1];
				real[i] += t1;
				imag[i] += t2;
			}
			u++;
		}
	}

	/* Find the highest amplitude value */
	/* start at index 1 because 0 can hold high values */
	j=1;
	l=0;
	for ( i=1; i<(128); i++ ) {
		l1 = square[CEILING(ABS(real[i]))]+square[CEILING(ABS(imag[i]))];
		if (l1 > l) {
			j = i;
			l = l1;
		}
	}
	return (j);
}

long long int ImageCapture() {
	int i;
	unsigned int a;
	unsigned long difference;
	// ADCON0 |= 0x01; //Power AD module up
	// ADCON0 &= ~(0b11000000); //OSC/2
	// ADCON1 |= ~(0b01000000); //OSC/2
	// ADCON0 &= ~(0b00111000); //AN0 activation
	// ADCON0 -> (0b0000_0100); GODONE bit 1 means in progress, GODONE bit 0 means done.
	Timer0_Init();
	Timer0_StartTimer();
	for (i = 0; i < 256; i++) {
		//__delay_us(200);
		a = ADC_Read((unsigned char) 0);
		CaptureSignal[i] = /*(unsigned int) */(a * 0.0053 - 2.5);
		//__delay_us(200);
		what[i] = 0;
	}
	Timer0_StopTimer();
	difference = ReadTime();
	unsigned int index = optfft(CaptureSignal, what);
	long long int freq = (index * difference) / 1024;
	//LCD_putrs(int_to_char(difference));
	return freq;
}

unsigned long getFreq() {
	ADC_Init();
	unsigned long freq = 0;
	unsigned int first = (unsigned int) (ADC_Read((unsigned char) 0) * 0.0049 - 2.5);
	unsigned int second = (unsigned int) (ADC_Read((unsigned char) 0) * 0.0049 - 2.5);
	while (first > second) {
		first = second;
		second = (unsigned int) (ADC_Read((unsigned char) 0) * 0.0049 - 2.5);
	}
	char isRising1 = 1;
	char isRising2 = 1;

	Timer0_Init();
	Timer0_StartTimer();
	while (ReadTime() < 1000000) {
		first = second;
		second = (unsigned int) (ADC_Read((unsigned char) 0) * 0.0049 - 2.5);
		isRising1 = isRising2;
		if (first < second) {
			isRising2 = 1;
		}
		else {
			isRising2 = 0;
		}
		if (isRising1 == 0 && isRising2 == 1) {
			freq++;
		}
	}
	Timer0_StopTimer();
	return freq;
}


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
