/*
 * Clock.c
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
#include "queue.h"
#include "semphr.h"
#include "Clock.h"
#include "KBD_interface.h"

u8 Seconds = 0;
u8 Minutes = 0;
u8 Hours = 0;
u8 KPD_Check_frequency = configTICK_RATE_HZ*2;
SemaphoreHandle_t LCD ;
xQueueHandle KPD_input = NULL;

void Clock_Print_Default_Interface(void)
{
	LCD_Set_Block(hours_position);
	LCD_Void_Write_Number_2(Hours);
	LCD_Set_Block(minutes_position-1);
	LCD_Void_Write_Data(':');
	LCD_Void_Write_Number_2(Minutes);
	LCD_Set_Block(seconds_position-1);
	LCD_Void_Write_Data(':');
	LCD_Void_Write_Number_2(Seconds);

}

void Clock_Second(void *pvParameters)
{
	LCD = xSemaphoreCreateMutex();
	TickType_t MyLastUnblockS;
	MyLastUnblockS = xTaskGetTickCount();
	LCD_Set_Block(seconds_position-1);
	LCD_Void_Write_Data(':');
	while(1)
	{
		if(xSemaphoreTake(LCD,10))
		{
			LCD_Set_Block(seconds_position);
			LCD_Void_Write_Number_2(Seconds);
			xSemaphoreGive(LCD);
		}
		vTaskDelayUntil(&MyLastUnblockS,seconds_frequency);
		Seconds++;
		Seconds %= 60;
	}
}

void Clock_Minute(void *pvParameters)
{
	TickType_t MyLastUnblockM;
	MyLastUnblockM = xTaskGetTickCount();
	LCD_Set_Block(minutes_position-1);
	LCD_Void_Write_Data(':');
	while(1)
	{
		if(xSemaphoreTake(LCD,10))
		{
			LCD_Set_Block(minutes_position);
			LCD_Void_Write_Number_2(Minutes);
			xSemaphoreGive(LCD);
		}

		vTaskDelayUntil(&MyLastUnblockM,minutes_frequency);
		Minutes++;
		Minutes %= 60;
	}
}
void Clock_Hours(void *pvParameters)
{
	TickType_t MyLastUnblockH;
	MyLastUnblockH = xTaskGetTickCount();
	LCD_Set_Block(minutes_position-1);
	LCD_Void_Write_Data(':');
	while(1)
	{
		if(xSemaphoreTake(LCD,10))
		{
			LCD_Set_Block(hours_position);
			LCD_Void_Write_Number_2((Hours%12)+1);
			xSemaphoreGive(LCD);
		}

		vTaskDelayUntil(&MyLastUnblockH,hours_frequency);
		Hours++;
		Hours %= 24;
	}
}

void Clock_Check_KPD(void *pvParameters)
{
	KPD_input = xQueueCreate(1,sizeof(u8));
	TickType_t MyLastUnblockKPD;
	MyLastUnblockKPD = xTaskGetTickCount();
	u8 keys[16];
	u8 pressed;
	while(1)
	{	pressed = KBD_u8GetKeyPadState(keys);
		if(pressed != 0xff)
		{
			xQueueSend(KPD_input,&pressed,10);
		}

		vTaskDelayUntil(&MyLastUnblockKPD,KPD_Check_frequency);
	}
}

void Clock_Enter_Typing_Mode(void *pvParameters)
{
	u8 pressed  = 0xff;
	u8 take_lach = 0;
	u8 current_block = 16;
	u8 take_lcd = 0;
	while(1)
	{
		if(xQueueReceive(KPD_input,&pressed,10))
		{

			switch (pressed)
			{
			default:
				if((take_lach))
				{
					LCD_Set_Block(current_block);
					LCD_Void_Write_Data(' ');
					switch (pressed)
					{
					case '>':
						if((current_block+1)%3 == 0){
							LCD_Set_Block(current_block+1);
							LCD_Void_Write_Data(' ');
							current_block++;
						}
						current_block++;
						LCD_Void_Write_Data(Up_Arrow);
						break;
					case '<':
						if((current_block-1)%3 == 0){
							LCD_Set_Block(current_block-1);
							LCD_Void_Write_Data(' ');
							current_block--;
						}
						current_block--;
						LCD_Set_Block(current_block);
						LCD_Void_Write_Data(Up_Arrow);
						break;
					default:

						if((current_block+1)%3 == 0){
							LCD_Set_Block(current_block+1);
							LCD_Void_Write_Data(' ');
							current_block++;
						}
						current_block++;
						LCD_Void_Write_Data(Up_Arrow);
						switch (current_block)
								{
								case hours_position+16:
									Hours = ((pressed-'0')*10)+(Hours%10);
									break;
								case hours_position+17:
									Hours = (pressed-'0');
									break;
								}
						LCD_Set_Block(current_block-16);
						LCD_Void_Write_Data(pressed);
						break;

					}
					vTaskDelay(2);
					continue;
				}
				break;


			case '*':
				if((take_lcd%2)==0)
				{
					if(xSemaphoreTake(LCD,10))
					{
						current_block = 16;
					//	LCD_Void_Write_String("taken  ");
						take_lach = 1;
						vTaskDelay(configTICK_RATE_HZ);
						LCD_Set_Block(current_block);
						LCD_Void_Write_Data(Up_Arrow);
						take_lcd++;
						KPD_Check_frequency = KPD_Check_frequency_Fast;
						current_block = 16;

					}
				}
				else
				{
					xSemaphoreGive(LCD);
					LCD_Set_Block(16);
					//LCD_Void_Write_String("released");
					take_lach = 0;
					Clock_Print_Default_Interface();
					KPD_Check_frequency = KPD_Check_frequency_Slow;
					LCD_Set_Block(current_block);
					LCD_Void_Write_Data(' ');
					vTaskDelay(configTICK_RATE_HZ);
					take_lcd++;

				}
				break;
			}

		}

	}
}






