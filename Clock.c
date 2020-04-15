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
u8 Alarm_Minutes = 0;
u8 Alarm_Hours = 0;
u8 Alarm_AM_PM = AM;
u8 flags = 0;
u8 current_block = second_row_start;
u8 time_adjusted = 0;//checks if the time has been adjusted
xSemaphoreHandle LCD ;
xSemaphoreHandle KPD_INT_Sem;
TaskHandle_t Minutes_handle = NULL;
TaskHandle_t Hours_handle = NULL;
TaskHandle_t Alarm_handle = NULL;
TaskHandle_t KPD_handle = NULL;

void Clock_Print_Default_Interface(void)
{
	LCD_Void_Clear();
	LCD_Set_Block(hours_position);
	LCD_Void_Write_Number_2((Hours%12)+1);
	LCD_Set_Block(minutes_position-1);
	LCD_Void_Write_Data(':');
	LCD_Void_Write_Number_2(Minutes%60);
	LCD_Set_Block(Am_PM_position);
	if(AM_PM == AM) LCD_Void_Write_String("AM");
	else LCD_Void_Write_String("PM");
	LCD_Set_Block(Alarm_Icon_Position);
	if(Get_Bit(flags,alarm_set) == 1) LCD_Void_Write_Data(Pixel_Alarm);
	else LCD_Void_Write_Data(' ');
	LCD_Set_Block(seconds_position);
	LCD_Void_Write_Number_2(Seconds);

}

void Clock_Second(void *pvParameters)
{

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
			if((Hours == Alarm_Hours)&&(Minutes == Alarm_Minutes)&&(AM_PM == Alarm_AM_PM))
			{
				xTaskCreate(Clock_Alarm,"Alarm",50,NULL,4,&Alarm_handle);

			}

		}
		else
		{

		}
		if(xSemaphoreTake(LCD,10))
		{

			LCD_Set_Block(minutes_position);
			LCD_Void_Write_Number_2(Minutes);
			LCD_Set_Block(Alarm_Icon_Position);
			if(Get_Bit(flags,alarm_set) == 1) LCD_Void_Write_Data(Pixel_Alarm);
			else LCD_Void_Write_Data(' ');
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

void Clock_Typing_Up_Arrow(u8* current_block,u8* am_pm)// what happens when up arrrow is pressed in typing mode
{
	if(*current_block == Am_PM_position + second_row_start)
	{
		/* if on the AM_Pm pos pressing the up arrow changes between AM and PM*/
		*am_pm += 1;
		*am_pm %= 2;
		LCD_Set_Block(Am_PM_position);
		if(*am_pm == AM) LCD_Void_Write_String("AM");
		else LCD_Void_Write_String("PM");
		LCD_Set_Block(*current_block);
		LCD_Void_Write_Data(Pixel_Arrow);
	}
}

void Clock_Typing_Alarm_Button(void)
{
	//entering alarm setting mode
	if(Get_Bit(flags,alarm_adjust) == 0)
	{
		LCD_Set_Block(second_row_start);
		LCD_Void_Write_Data(Pixel_Arrow);
		Set_Bit(flags,alarm_adjust);
		LCD_Set_Block(hours_position);
		LCD_Void_Write_Number_2(0);
		LCD_Set_Block(minutes_position-1);
		LCD_Void_Write_Data(':');
		LCD_Void_Write_Number_2(0);
		LCD_Set_Block(Am_PM_position);
		if(Alarm_AM_PM == AM) LCD_Void_Write_String("AM");
		else LCD_Void_Write_String("PM");
		LCD_Set_Block(Set_Alarm_Position);
		LCD_Void_Write_String("Set Alarm");
	}
	//exiting alarm setting mode
	else
	{
		LCD_Void_Clear();
		Clear_Bit(flags,alarm_adjust);
		Clock_Print_Default_Interface();

	}
}

void Clock_Typing_Number(u8* pressed,u8* time_adjusted,u8* current_block,u8* hours,u8* minutes)//what happens when a number is pressed in typing mode
{
	/*change the time according to the user input*/
	switch (*current_block)
			{
			case hours_position+16:
				*hours = ((*pressed-'0')*10)+(*hours%10);
				*time_adjusted = 1;
				break;
			case hours_position+17:
				*hours = (*pressed-1-'0')+((*hours/10)*10);
				*time_adjusted = 1;
				break;
			case minutes_position+16:
				*minutes = ((*pressed-'0')*10)+(*minutes%10);
				*time_adjusted = 1;
				break;
			case minutes_position+17:
				*minutes = (*pressed-'0')+((*minutes/10)*10);
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

void Clock_Typing_Exit(u8* time_adjusted,u8* current_block)//what happens when you exit typing mode
{
	xSemaphoreGive(LCD);
	LCD_Set_Block(second_row_start);
	Clock_Print_Default_Interface();//print default interface after giving back the LCD
	LCD_Set_Block(*current_block);
	LCD_Void_Write_Data(' ');
	current_block = second_row_start;
	if(*time_adjusted)
	{
		if(Minutes_handle != NULL)
		{
			vTaskDelete(Minutes_handle);
			Minutes_handle = NULL;
		}
		xTaskCreate(Clock_Minute,"minutes",75,NULL,2,&Minutes_handle);

		if(Hours_handle != NULL)
		{
			vTaskDelete(Hours_handle);
			Hours_handle = NULL;
		}
		*time_adjusted = 0;
		Seconds = 0;
	}

	xTaskCreate(Clock_Hours,"hours",70,NULL,3,&Hours_handle);


}

void Clock_Typing_Enter(u8* current_block)//what happens when you enter typing mode
{
	if(xSemaphoreTake(LCD,10))
	{
		*current_block = second_row_start; //when taking the LCD set the cursor to the 2nd row
		LCD_Set_Block(*current_block);
		LCD_Void_Write_Data(Pixel_Arrow);
	}
}

void Clock_Print_Alarm_Interface(void)
{
	LCD_Void_Clear();
	LCD_Set_Block(hours_position);
	LCD_Void_Write_Number_2((Alarm_Hours%12)+1);
	LCD_Set_Block(minutes_position-1);
	LCD_Void_Write_Data(':');
	LCD_Void_Write_Number_2(Alarm_Minutes%60);
	LCD_Set_Block(Am_PM_position);
	if(AM_PM == AM) LCD_Void_Write_String("AM");
	else LCD_Void_Write_String("PM");
	LCD_Set_Block(Alarm_Text_Position);
	if(Get_Bit(flags,alarm_text_toggle) == 1) LCD_Void_Write_String("ALARM!");
	else LCD_Void_Write_String("      ");
}

void Clock_Alarm(void)//what happens when there is an alarm
{

	if(xSemaphoreTake(LCD,10))
	{
		Clock_Print_Alarm_Interface();
		Set_Bit(flags,alarm_latch);
	}
	while(1)
	{

		Toggle_Bit(flags,alarm_text_toggle);
		if(Get_Bit(flags,alarm_set) == 1)//if an alarm is set
		{
			if(Get_Bit(flags,alarm_latch) == 1)
			{
				Clock_Print_Alarm_Interface();
			}
		}
		else
		{
			Clock_Print_Default_Interface();
			xSemaphoreGive(LCD);
			Clear_Bit(flags,alarm_latch);
			vTaskDelete(Alarm_handle);
		}
		vTaskDelay(seconds_frequency);
	}
}
void Clock_Typing_Mode(void *pvParameters)
{
	u8 pressed  = 0xff;
	u8 keys[16];
	u8 alarm_adjusted = 0;
	while(1)
	{
		pressed = KBD_u8GetKeyPadState(keys);
		if(pressed != 0xff)
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
				//in time adjusting mode
				if(Get_Bit(flags,alarm_adjust) == 0)
				{
					Clock_Typing_Up_Arrow(&current_block,&AM_PM);
				}
				//in alarm setting mode
				else
				{
					Clock_Typing_Up_Arrow(&current_block,&Alarm_AM_PM);
				}
				break;

			case 'A'://in alarm setting mode
				Clock_Typing_Alarm_Button();
				break;
			default:
				//in time adjusting mode
				if(Get_Bit(flags,alarm_adjust) == 0)
				{
					Clock_Typing_Number(&pressed,&time_adjusted,&current_block,&Hours,&Minutes);
				}
				//in alarm setting mode
				else
				{
					Clock_Typing_Number(&pressed,&alarm_adjusted,&current_block,&Alarm_Hours,&Alarm_Minutes);
					//alarm is set
					if(alarm_adjusted == 1)
					{
						Set_Bit(flags,alarm_set);
					}
					alarm_adjusted = 0;
				}
				break;

			}
		}
		vTaskDelay(KPD_Check_frequency);
	}

}

void KPD_Button_INT(void)
{

	xSemaphoreGiveFromISR(KPD_INT_Sem,NULL);

}

void Clock_Semaphore_Init(void)
{
	LCD = xSemaphoreCreateMutex();
	vSemaphoreCreateBinary(KPD_INT_Sem);
}

void KPD_Button_INT_ISR(void)
{
	while(1)
	{
		if(xSemaphoreTake(KPD_INT_Sem,9999))
		{
			Toggle_Bit(flags,KPD_flag);
			if(Get_Bit(flags,KPD_flag) == 0)
			{
				DIO_u8SetPinValue(C7,1);
				Clock_Typing_Enter(&current_block);
				if(KPD_handle == NULL)
				{
					xTaskCreate(Clock_Typing_Mode,"KPD",80,NULL,5,&KPD_handle);
				}

			}
			else
			{
				DIO_u8SetPinValue(C7,0);
				if(KPD_handle != NULL)
				{
					vTaskDelete(KPD_handle);
				}
				Clock_Typing_Exit(&time_adjusted,&current_block);
			}
		}
	}
}




