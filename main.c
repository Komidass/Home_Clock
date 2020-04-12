/*
 * main.c
 *
 *  Created on: Mar 24, 2020
 *      Author: tho
 */
#define F_CPU	8000000
#include "avr/delay.h"
#include "BIT_CALC.h"
#include "STD_Types.h"
#include "DIO_Interface.h"
#include "LCD_interface.h"
#include "FreeRTOS.h"
#include "task.h"
#include "Clock.h"
#include "queue.h"
#include "semphr.h"
#include "avr/sleep.h"
#include "KBD_interface.h"


int main(void)
{
	LCD_initialize();
	LCD_Void_Clear();
	Clock_Print_Default_Interface();
	KBD_u8Initialize(GROUP_B);
	xTaskCreate(Clock_Second,"seconds",70,NULL,2,NULL);
	xTaskCreate(Clock_Check_KPD,"KPD_check",70,NULL,2,NULL);
	xTaskCreate(Clock_Typing_Mode,"Enter typing mode",50,NULL,2,NULL);
	vTaskStartScheduler();

	/*while(1)
	{

	}*/
	return 0;
}

