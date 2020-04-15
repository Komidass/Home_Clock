/*******************************************************/
/* Author  : Bishoy Medhat                             */
/* Version : V1.0                                      */
/* Date    : 19 MAR 2019                               */
/*******************************************************/
/* Description                                         */
/* ------------                                        */
/* This file has the implementation of both private and*/
/* public function corresponding to EXTI module        */
/*******************************************************/

#include "BIT_CALC.h"
#include "STD_Types.h"
#include "EXTI_private.h"
#include "EXTI_config.h"
#include "EXTI_interface.h"



static void (*PvoidCallBack[3])(void) = {Null,Null,Null}; // array of pointers to functions
/* */
/* */

void __vector_1(void)
{
	if(PvoidCallBack[0] != Null)
	{
		PvoidCallBack[0]();
	}
}
/* */
/* */
void __vector_2(void)
{
	if(PvoidCallBack[1] != Null)
	{
		PvoidCallBack[1]();
	}
}
/* */
/* */
void __vector_3(void)
{
	if(PvoidCallBack[2] != Null)
	{
		PvoidCallBack[2]();
	}
}
/* */
/* */
void voidSetCallBack(u8 Copy_u8InterruptIdx, void(*Copy_PvoidCallBack)(void))
{
	PvoidCallBack[Copy_u8InterruptIdx] = Copy_PvoidCallBack;
}
/* */
/* */
void Interrupt_Initialize(void)
{
	Assign_Bit(SREG,7,INTERRUPT_ENABLE);
}
/* */
/* */
void Interrupt_Enable(u8 Copy_INT_Index)
{
	switch(Copy_INT_Index)
	{
		case INT_0:
			Assign_Bit(GICR,6,INTERRUPT_ENABLE);
		break;
		case INT_1:
			Assign_Bit(GICR,7,INTERRUPT_ENABLE);
		break;
		case INT_2:
			Assign_Bit(GICR,5,INTERRUPT_ENABLE);
		break;
	}
}
/* */
/* */
void Interrupt_Disable(u8 Copy_INT_Index)
{
	switch(Copy_INT_Index)
	{
		case INT_0:
			Assign_Bit(GICR,6,INTERRUPT_DISABLE);
		break;
		case INT_1:
			Assign_Bit(GICR,7,INTERRUPT_DISABLE);
		break;
		case INT_2:
			Assign_Bit(GICR,5,INTERRUPT_DISABLE);
		break;
	}
}
/* */
/* */
void Interrupt_Set_Level(u8 Copy_INT_Index, u8 Copy_INT_Level)
{
	switch(Copy_INT_Index)
	{
		case INT_0:
			ASSIGN_2BITS(MCUCR,0,Copy_INT_Level);
		break;
		case INT_1:
			ASSIGN_2BITS(MCUCR,1,Copy_INT_Level);
		break;
		case INT_2:
			Assign_Bit(MCUCSR,6,Copy_INT_Level);
		break;
	}
}
