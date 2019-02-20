/*
 * File:   drive_memory.h
 * Author: reyching
 *
 * Created on February 1, 2019, 5:19 PM
 */

#ifndef DRIVE_MEMORY_H
#define	DRIVE_MEMORY_H

#ifdef	__cplusplus
extern "C" {
#endif

void write_op(char address, char data);
char read_op(char address);


#ifdef	__cplusplus
}
#endif

#endif	/* DRIVE_MEMORY_H */
