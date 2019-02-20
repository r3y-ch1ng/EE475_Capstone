/*
 * File:   optfft.h
 * Author: reyching
 *
 * Created on January 19, 2019, 4:54 PM
 */

#ifndef OPTFFT_H
#define	OPTFFT_H

#ifdef	__cplusplus
extern "C" {
#endif

unsigned long getFreq();
unsigned int ADC_Read(unsigned char channel);
void ADC_Init();
long long int ImageCapture();
unsigned int optfft(signed int x[256], signed int y[256]);

#ifdef	__cplusplus
}
#endif

#endif	/* OPTFFT_H */
