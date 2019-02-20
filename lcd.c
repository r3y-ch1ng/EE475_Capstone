/*
* File:   lcd.c
* Author: reyching
*
* Created on January 18, 2019, 2:35 PM
*/

#define _XTAL_FREQ 16000000

#include <stdbool.h>
#include <xc.h>
#include "lcd.h"
#include <stdio.h>
#include "serial.h"

LCD lcd;

char* int_to_char(unsigned long num) {
  char result[10];
  unsigned long the_num = num;
  sprintf(result, "%lu", the_num);
  getchar();
  return result;
}

char* float_to_char(double num) {
  char result[17];
  double the_num = num;
  snprintf(result, sizeof(result), "%d", num);
  //sprintf(result, "%d", the_num);
  getchar();
  return result;
}

void LCD_OutMain(char c) {
  if (c & 8) PORTD |= 0x80;
  else PORTD &= ~(0x80);
  if (c & 4) PORTD |= 0x40;
  else PORTD &= ~(0x40);
  if (c & 2) PORTD |= 0x20;
  else PORTD &= ~(0x20);
  if (c & 1) PORTD |= 0x10;
  else PORTD &= ~(0x10);
}

void LCD_Char(char c) {
  PORTD |= 0x04; //RS is high, data to be displayed
  LCD_OutMain((c & 0xf0) >> 4); //Data transfer of the top bits.
  __delay_ms(5);
  PORTD |= 0x08; //EN is high
  __delay_ms(5);
  PORTD &= ~0x08; //EN is low
  LCD_OutMain(c & 0xf);
  PORTD |= 0x08; //EN is high
  __delay_ms(5);
  PORTD &= ~0x08; //EN is low
}

void LCD_WriteMain(char c) {
  PORTD |= 0x08; //EN is high
  __delay_ms(5);
  PORTD &= ~(0x04); //RS is low
  __delay_ms(5);
  LCD_OutMain(c);
  __delay_ms(5);
  PORTD &= ~(0x08); //EN is low
}

void LCD_Initialize(LCD display) {
  lcd = display;
  if ( lcd.PORT == &PORTA ) {
    TRISA = 0x00;
  }
  else if ( lcd.PORT == &PORTB ) {
    TRISB = 0x00;
  }
  else if ( lcd.PORT == &PORTD ) {
    TRISC = 0x00;
  }
  #if defined(_16F877) || defined(_16F877A)
  else if ( lcd.PORT == &PORTD ) {
    TRISD = 0x00;
  }
  else if ( lcd.PORT == &PORTE ) {
    TRISE = 0x00;
  }
  #endif
  else {
    return;
  }
  __delay_ms(20);
  LCD_WriteMain(0x03);
  __delay_ms(5);
  LCD_WriteMain(0x03);
  __delay_ms(16);
  LCD_WriteMain(0x03);
  LCD_WriteMain(0x02);
  LCD_WriteMain(0x02);
  LCD_WriteMain(0x08);
  // LCD_Cmd(0x28);
  LCD_WriteMain(0x00);
  LCD_WriteMain(0x08);
  LCD_WriteMain(0x00);
  LCD_WriteMain(0x01);
  LCD_WriteMain(0x00);
  LCD_WriteMain(0x06);
  LCD_Display(1, 0, 0); // Turn on display and set cursor off
  LCD_Clear();
}

void LCD_puts (char *a) {
  for (int i = 0; a[i] != '\0'; i++) {
    LCD_Char(a[i]);
  }
}

void LCD_putrs (const char *a) {
  for (int i = 0; a[i] != '\0'; i++) {
    LCD_Char(a[i]);
  }
}
