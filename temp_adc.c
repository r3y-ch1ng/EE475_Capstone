#include <stdint.h>
#include <xc.h>
#include "drive_memory.h"
#include "serial.h"
#include "adc_temp.h"
#include "adc.h"

#define _XTAL_FREQ 16000000

float get_temp() {
  ADC_Init();
  ADCON1 &= ~(0x01); //Only port A is analog input
  TRISA |= 0x01; //A0 is analog input
  unsigned int adc_voltage = ADC_Read((unsigned char) 0); //10-bit number ADC reading
  float real_voltage = (adc_voltage * 0.0053); //Convert to actual voltage float
  /*Actual demo temperature range is from 25 to 200  degrees farenheit*/
  float temp = 200.0 - 170.0 / 5.18 * real_voltage;
  return temp;
}
