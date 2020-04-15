/*
 * main.c
 *
 *  Created on: Mar 24, 2020
 *      Author: tho
 */
#define F_CPU	8000000
//#include "avr/delay.h"
#include "BIT_CALC.h"
#include "STD_Types.h"
#include "DIO_Interface.h"
#include "LCD_interface.h"
#include "FreeRTOS.h"
#include "task.h"
#include "Clock.h"
#include "queue.h"
#include "semphr.h"
#include "KBD_interface.h"
#include "EXTI_interface.h"


int main(void)
{
	LCD_initialize();
	LCD_Void_Clear();
	Clock_Print_Default_Interface();
	KBD_u8Initialize(GROUP_B);
	Clock_Semaphore_Init();
	voidSetCallBack(INT_0,KPD_Button_INT);
	DIO_u8SetPinDirection(D2,DIO_u8_INPUT);
	DIO_u8SetPinValue(D2,1);
	DIO_u8SetPinDirection(C7,DIO_u8_OUTPUT);
	Interrupt_Set_Level(INT_0,INTERRUPT_LEVEL_RISING_EDGE);
	Interrupt_Initialize();
	Interrupt_Enable(INT_0);
	xTaskCreate(Clock_Second,"seconds",70,NULL,2,NULL);
	xTaskCreate(KPD_Button_INT_ISR,"KPD_Button_ISR",50,NULL,2,NULL);

	vTaskStartScheduler();

	return 0;
}

