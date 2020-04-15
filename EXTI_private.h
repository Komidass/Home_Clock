/*******************************************************/
/* Author  : Bishoy Medhat                             */
/* Version : V1.0                                      */
/* Date    : 19 MAR 2019                               */
/*******************************************************/
/* Description                                         */
/* ------------                                        */
/* private data and function                           */
/*******************************************************/

/* Comment: Header file guard                          */
#ifndef EXTI_PRIVATE_H
#define EXTI_PRIVATE_H

#define MCUCR	*((u8*)0x55)
#define MCUCSR	*((u8*)0x54)
#define GICR	*((u8*)0x5B)
#define SREG	*((u8*)0x5F)

#define Null ((void *)0)

void __vector_1(void) __attribute__((signal));		// Prototype for the function related to interrupt 0
void __vector_2(void) __attribute__((signal));		// Prototype for the function related to interrupt 1
void __vector_3(void) __attribute__((signal));		// Prototype for the function related to interrupt 2

#endif
