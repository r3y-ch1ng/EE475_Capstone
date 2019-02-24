/*
 * File:   pwm.h
 * Author: reyching
 *
 * Created on February 23, 2019, 3:48 PM
 */

#ifndef PWM_H
#define	PWM_H

#ifdef	__cplusplus
extern "C" {
#endif

volatile unsigned long time_ms_2;
void initialize_PWM(void);
void set_duty_cycle(char upper_8, char lower_2);


#ifdef	__cplusplus
}
#endif

#endif	/* PWM_H */
