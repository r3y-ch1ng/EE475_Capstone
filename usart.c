#include <xc.h>

#define _XTAL_FREQ 16000000

//RC4 is SDI, RC5 is SDO

void initializeSerial() {
    TRISCbits.RC3 = 0; //SCK
    TRISCbits.RC4 = 1; //SDI
    TRISCbits.RC5 = 0; //SDO
    SSPSTATbits.SMP = 1;
    SSPSTATbits.CKE = 1;
    SSPCON1 = 0b00100000;
}

void serial_send(char c) {
  SSPBUF = c;
  //__delay_us(10);
  //SSPBUF = 'A';
}

char serial_recieve() {
  return (char) SSPBUF;
}
