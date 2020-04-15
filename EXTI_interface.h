/*******************************************************/
/* Author  : Bishoy Medhat                             */
/* Version : V1.0                                      */
/* Date    : 19 MAR 2019                               */
/*******************************************************/
/* Description                                         */
/* ------------                                        */
/* This file defines the APIs corresponding to EXTI    */
/*******************************************************/

/* Comment: Header file guard                          */
#ifndef EXTI_INTERFACE_H
#define EXTI_INTERFACE_H

#define INTERRUPT_LEVEL_LOW_LEVEL				0b00
#define INTERRUPT_LEVEL_ANY_LOGICAL_CHANGE		0b01
#define INTERRUPT_LEVEL_FALLING_EDGE			0b10
#define INTERRUPT_LEVEL_RISING_EDGE				0b11

#define INTERRUPT_LEVEL_INT_2_RISING_EDGE		0b1
#define INTERRUPT_LEVEL_INT_2_FALLING_EDGE		0b0

#define INTERRUPT_ENABLE						0b1
#define INTERRUPT_DISABLE						0b0

#define INT_0	0
#define INT_1	1
#define INT_2	2


void voidSetCallBack(u8 Copy_u8InterruptIdx, void(*Copy_PvoidCallBack)(void));

void Interrupt_Initialize(void);
void Interrupt_Enable(u8 Copy_INT_Index);
void Interrupt_Disable(u8 Copy_INT_Index);
void Interrupt_Set_Level(u8 Copy_INT_Index, u8 Copy_INT_Level);

#endif
