/*
* File            : spi.c
* Author          : Ligo George
* Company         : electroSome
* Project         : SPI Library for MPLAB XC8
* Microcontroller : PIC 16F877A
* Created on April 15, 2017, 5:59 PM
*/

#include <xc.h>

#define _XTAL_FREQ 16000000

#include "usart.h"

void spiInit(Spi_Type sType, Spi_Data_Sample sDataSample, Spi_Clock_Idle sClockIdle, Spi_Transmit_Edge sTransmitEdge)
{
   TRISCbits.TRISC5 = 0;
   //TRISCbits.TRISC4 = 1;
   SSPSTAT = sDataSample | sTransmitEdge;
   TRISCbits.TRISC3 = 0;
   SSPCON1 = sType | sClockIdle;
   //SSPCON1 &= ~(0x0f);
   //SSPCON1 |= 0b10;

}

static void spiReceiveWait() {
   while (!SSPSTATbits.BF); // Wait for Data Receive complete
}

void spiWrite(char dat){
   SSPBUF = dat;
}

unsigned spiDataReady() {
   if(SSPSTATbits.BF)
       return 1;
   else
       return 0;
}

char spiRead() {
   spiReceiveWait();        // wait until the all bits receive
   return(SSPBUF); // read the received data from the buffer
}
