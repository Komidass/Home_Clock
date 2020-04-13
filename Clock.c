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
u8 Alarm_Minutes;
u8 Alarm_Hours;
u8 Alarm_AM_PM;
u8 flags;
u8 KPD_Check_frequency = KPD_Check_frequency_Slow;
SemaphoreHandle_t LCD ;
xQueueHandle KPD_input = NULL;
xQueueHandle minutes_adjusted = NULL; //for adjusted minutes to add in the hours
TaskHandle_t Minutes_handle = NULL;
TaskHandle_t Hours_handle = NULL;


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
		if(Get_Bit(flags,alarm_set) == 1)//alarm is set
		{
			LCD_Set_Block(26);
			LCD_Void_Write_Number_2(Alarm_Hours);
			LCD_Void_Write_String("?=");
			LCD_Void_Write_Number_2(Alarm_Minutes);

			if((Hours == Alarm_Hours)&&(Minutes == Alarm_Minutes))
			{
				Clock_Alarm();
			}
		}
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
	MyLastUnblockH = xTaskGetTickCount() - Minutes*minutes_frequency;
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

void Clock_Typing_Right_Arrow(u8* current_block)//what happens when right arrow is pressed in typing mode
{
	/* if the cursor is going to be on a ':' next time increment it by 1*/
	if((*current_block+1)%3 == 0){
		LCD_Set_Block(*current_block+1);
		LCD_Void_Write_Data(' ');
		*current_block += 1;
	}
	*current_block += 1;
	if(*current_block  == maximum_cursor_range) /*if cursor hits maximum range get it back to the beginning*/
	{
		*current_block = minimum_cursor_range;
		LCD_Set_Block(*current_block );

	}
	LCD_Void_Write_Data(Pixel_Arrow);
}

void Clock_Typing_Left_Arrow(u8* current_block)//what happens when left arrow is pressed in typing mode
{
	if((*current_block-1)%3 == 0){
		LCD_Set_Block(*current_block-1);
		LCD_Void_Write_Data(' ');
		*current_block -= 1;
	}
	*current_block -=1;
	if(*current_block == minimum_cursor_range-2)
	{
		*current_block = maximum_cursor_range-1;
		LCD_Set_Block(*current_block);

	}
	LCD_Set_Block(*current_block);
	LCD_Void_Write_Data(Pixel_Arrow);
}

void Clock_Typing_Up_Arrow(u8* current_block)// what happens when up arrrow is pressed in typing mode
{
	if(*current_block == Am_PM_position + second_row_start)
	{
		/* if on the AM_Pm pos pressing the up arrow changes between AM and PM*/
		AM_PM++;
		AM_PM %= 2;
		LCD_Set_Block(Am_PM_position);
		if(AM_PM == AM) LCD_Void_Write_String("AM");
		else LCD_Void_Write_String("PM");
		LCD_Set_Block(*current_block);
		LCD_Void_Write_Data(Pixel_Arrow);
	}
}

void Clock_Typing_Number(u8* pressed,u8* time_adjusted,u8* current_block,u8* hours,u8* minutes)//what happens when a number is pressed in typing mode
{
	/*change the time according to the user input*/
	switch (*current_block)
			{
			case hours_position+16:
				*hours = ((*pressed-'0')*10)+(Hours%10);
				*time_adjusted = 1;
				break;
			case hours_position+17:
				*hours = (*pressed-1-'0')+((Hours/10)*10);
				*time_adjusted = 1;
				break;
			case minutes_position+16:
				*minutes = ((*pressed-'0')*10)+(Minutes%10);
				*time_adjusted = 1;
				break;
			case minutes_position+17:
				*minutes = (*pressed-'0')+((Minutes/10)*10);
				*time_adjusted = 1;
				break;
			}


	LCD_Set_Block(*current_block-second_row_start);
	if(*current_block != Am_PM_position + second_row_start)	LCD_Void_Write_Data(*pressed);

	LCD_Set_Block(*current_block+1);
	if((*current_block+1)%3 == 0){
		LCD_Set_Block(*current_block+1);
		LCD_Void_Write_Data(' ');
		*current_block += 1;
	}
		*current_block += 1;
	if(*current_block == maximum_cursor_range)
	{
		*current_block = minimum_cursor_range;
		LCD_Set_Block(*current_block);

	}
	LCD_Void_Write_Data(Pixel_Arrow);
}

void Clock_Typing_Exit(u8* take_lcd,u8* take_lach,u8* time_adjusted,u8* current_block)//what happens when you exit typing mode
{
	xSemaphoreGive(LCD);
	LCD_Set_Block(second_row_start);
	*take_lach = 0;
	Clock_Print_Default_Interface();//print default interface after giving back the LCD
	KPD_Check_frequency = KPD_Check_frequency_Slow;//change KPD check freq to slow
	LCD_Set_Block(*current_block);
	LCD_Void_Write_Data(' ');
	vTaskDelay(configTICK_RATE_HZ);//to avoid quick taking and giving the LCD
	*take_lcd += 1;

	if(*time_adjusted)
	{
		if(Minutes_handle != NULL)
		{
			vTaskDelete(Minutes_handle);
			Minutes_handle = NULL;
		}
		xTaskCreate(Clock_Minute,"minutes",70,NULL,2,&Minutes_handle);

		if(Hours_handle != NULL)
		{
			vTaskDelete(Hours_handle);
			Hours_handle = NULL;
		}
		*time_adjusted = 0;
	}

	xTaskCreate(Clock_Hours,"hours",70,NULL,2,Hours_handle);


}

void Clock_Typing_Enter(u8* take_lcd,u8* take_lach,u8* current_block)//what happens when you enter typing mode
{
	if(xSemaphoreTake(LCD,10))
	{
		*current_block = second_row_start; //when taking the LCD set the cursor to the 2nd row
		*take_lach = 1;
		LCD_Set_Block(*current_block);
		LCD_Void_Write_Data(Pixel_Arrow);
		*take_lcd += 1;
		KPD_Check_frequency = KPD_Check_frequency_Fast; // enter fast KPD checking freq
	}
}
void Clock_Alarm(void)//what happens when there is an alarm
{
	LCD_Set_Block(24);
	LCD_Void_Write_String("ALARM");
	Clear_Bit(flags,alarm_set);

}
void Clock_Typing_Mode(void *pvParameters)
{
	u8 pressed  = 0xff;
	u8 take_lach = 0;//to latch entering any key other the the LCD taking key if the entering key is not pressed continue the loop
	u8 current_block = second_row_start;
	u8 take_lcd = 0;//to latch taking LCD
	u8 time_adjusted = 0;//checks if the time has been adjusted
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
						Clock_Typing_Right_Arrow(&current_block);
						break;
					case '<':
						Clock_Typing_Left_Arrow(&current_block);
						break;

					case Up_Arrow:
						Clock_Typing_Up_Arrow(&current_block);
						break;

					case 'A'://alarm adjusting
						if(Get_Bit(flags,alarm_adjust) == 0)
						{
							Set_Bit(flags,alarm_adjust);
							LCD_Set_Block(hours_position);
							LCD_Void_Write_Number_2(0);
							LCD_Set_Block(minutes_position-1);
							LCD_Void_Write_Data(':');
							LCD_Void_Write_Number_2(0);
							LCD_Set_Block(Am_PM_position);
							LCD_Void_Write_String("AM");
							//LCD_Set_Block(23);
							//LCD_Void_Write_String("Set Alarm");
						}
						else
						{
							LCD_Void_Clear();
							Clear_Bit(flags,alarm_adjust);
							Clock_Print_Default_Interface();

						}
						break;
					default:

						if(Get_Bit(flags,alarm_adjust) == 0)
						{
							Clock_Typing_Number(&pressed,&time_adjusted,&current_block,&Hours,&Minutes);
						}
						else
						{
							Clock_Typing_Number(&pressed,&time_adjusted,&current_block,&Alarm_Hours,&Alarm_Minutes);
							if(time_adjusted == 1)
							{
								Set_Bit(flags,alarm_set);
							}
							else
							{
								Clear_Bit(flags,alarm_set);
							}
						}
						break;

					}

					vTaskDelay(3);
					continue;
				}

				break;


			case '*':
				if((take_lcd%2)==0)
				{
					Clock_Typing_Enter(&take_lcd,&take_lach,&current_block);
				}
				else
				{
					Clock_Typing_Exit(&take_lcd,&take_lach,&time_adjusted,&current_block);
				}
				break;
			}

		}

	}
}






