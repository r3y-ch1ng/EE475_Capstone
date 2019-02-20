/*
 * File:   usart.h
 * Author: reyching
 *
 * Created on February 3, 2019, 1:12 PM
 */

#ifndef USART_H
#define	USART_H

#ifdef	__cplusplus
extern "C" {
#endif

void initializeSerial(void);
void serial_send(char c);
char serial_recieve();


#ifdef	__cplusplus
}
#endif

#endif	/* USART_H */
