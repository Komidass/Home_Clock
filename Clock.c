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
u8 AM_PM = AM;
u8 KPD_Check_frequency = configTICK_RATE_HZ*2;
SemaphoreHandle_t LCD ;
xQueueHandle KPD_input = NULL;

void Clock_Print_Default_Interface(void)
{
	LCD_Set_Block(hours_position);
	LCD_Void_Write_Number_2((Hours%12)+1);
	LCD_Set_Block(minutes_position-1);
	LCD_Void_Write_Data(':');
	LCD_Void_Write_Number_2(Minutes%60);
	LCD_Set_Block(Am_PM_position);
	if(AM_PM == AM) LCD_Void_Write_String("AM");
	else LCD_Void_Write_String("PM");
	LCD_Set_Block(seconds_position);
	LCD_Void_Write_Number_2(Seconds);

}

void Clock_Second(void *pvParameters)
{
	LCD = xSemaphoreCreateMutex();
	TickType_t MyLastUnblockS;
	MyLastUnblockS = xTaskGetTickCount();
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
			LCD_Set_Block(Am_PM_position);
			if(AM_PM == AM) LCD_Void_Write_String("AM");
			else LCD_Void_Write_String("PM");
			xSemaphoreGive(LCD);
		}

		vTaskDelayUntil(&MyLastUnblockH,hours_frequency);
		Hours++;
		if(Hours == 12)
		{
			AM_PM++;
			AM_PM %= 2;
		}
		Hours %= 12;

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
	u8 take_lach = 0;//to latch entering any key other the the LCD taking key if the entering key is not pressed continue the loop
	u8 current_block = second_row_start;
	u8 take_lcd = 0;//to latch taking LCD
	while(1)
	{
		if(xQueueReceive(KPD_input,&pressed,10))
		{

			switch (pressed)
			{
			default:
				if((take_lach))
				{
					/* Set to the current block and clear the last cursor*/
					LCD_Set_Block(current_block);
					LCD_Void_Write_Data(' ');


					switch (pressed)
					{
					case '>':
						/* if the cursor is going to be on a ':' next time increment it by 1*/
						if((current_block+1)%3 == 0){
							LCD_Set_Block(current_block+1);
							LCD_Void_Write_Data(' ');
							current_block++;
						}
						current_block++;
						if(current_block == maximum_cursor_range) /*if cursor hits maximum range get it back to the beginning*/
						{
							current_block = minimum_cursor_range;
							LCD_Set_Block(current_block);

						}
						LCD_Void_Write_Data(Pixel_Arrow);
						break;
					case '<':
						if((current_block-1)%3 == 0){
							LCD_Set_Block(current_block-1);
							LCD_Void_Write_Data(' ');
							current_block--;
						}
						current_block--;
						if(current_block == minimum_cursor_range-2)
						{
							current_block = maximum_cursor_range-1;
							LCD_Set_Block(current_block);

						}
						LCD_Set_Block(current_block);
						LCD_Void_Write_Data(Pixel_Arrow);
						break;

					case Up_Arrow:

						if(current_block == Am_PM_position + second_row_start)
						{


							AM_PM++;
							AM_PM %= 2;
							LCD_Set_Block(Am_PM_position);
							if(AM_PM == AM) LCD_Void_Write_String("AM");
							else LCD_Void_Write_String("PM");
							LCD_Set_Block(current_block);
							LCD_Void_Write_Data(Pixel_Arrow);
						}
							break;

					default:
						/*change the time according to the user input*/
						switch (current_block)
								{
								case hours_position+16:
									Hours = ((pressed-'0')*10)+(Hours%10);
									break;
								case hours_position+17:
									Hours = (pressed-1-'0')+((Hours/10)*10);
									break;
								case minutes_position+16:
									Minutes = ((pressed-'0')*10)+(Minutes%10);
									break;
								case minutes_position+17:
									Minutes = (pressed-'0')+((Minutes/10)*10);
									break;
								}
						LCD_Set_Block(current_block-second_row_start);
						LCD_Void_Write_Data(pressed);

						LCD_Set_Block(current_block+1);
						if((current_block+1)%3 == 0){
							LCD_Set_Block(current_block+1);
							LCD_Void_Write_Data(' ');
							current_block++;
						}
						current_block++;
						if(current_block == maximum_cursor_range+2)
						{
							current_block = minimum_cursor_range;
							LCD_Set_Block(current_block);

						}
						LCD_Void_Write_Data(Pixel_Arrow);
						break;

					}

					vTaskDelay(3);
					continue;
				}

				break;


			case '*':
				if((take_lcd%2)==0)
				{
					if(xSemaphoreTake(LCD,10))
					{
						current_block = second_row_start; //when taking the LCD set the cursor to the 2nd row
						take_lach = 1;
						LCD_Set_Block(current_block);
						LCD_Void_Write_Data(Pixel_Arrow);
						take_lcd++;
						KPD_Check_frequency = KPD_Check_frequency_Fast; // enter fast KPD checking freq
					}
				}
				else
				{
					xSemaphoreGive(LCD);
					LCD_Set_Block(second_row_start);
					take_lach = 0;
					Clock_Print_Default_Interface();//print default interface after giving back the LCD
					KPD_Check_frequency = KPD_Check_frequency_Slow;//change KPD check freq to slow
					LCD_Set_Block(current_block);
					LCD_Void_Write_Data(' ');
					vTaskDelay(configTICK_RATE_HZ);//to avoid quick taking and giving the LCD
					take_lcd++;

				}
				break;
			}

		}

	}
}






