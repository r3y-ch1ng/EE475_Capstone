/*
 * File:   timer.h
 * Author: reyching
 *
 * Created on January 22, 2019, 5:53 PM
 */

#ifndef TIMER_H
#define	TIMER_H

#ifdef	__cplusplus
extern "C" {
#endif

#ifndef __TIMER_0_H
#define __TIMER_0_H

#include <p18cxxx.h>

/* used to hold 16-bit timer value */
volatile unsigned long time_ms;

unsigned long ReadTime(void);
void Timer0_Init(void);
int Timer0_StartTimer(void);
int Timer0_StopTimer(void);
void __interrupt() timingISR(void);



#endif


#ifdef	__cplusplus
}
#endif

#endif	/* TIMER_H */
