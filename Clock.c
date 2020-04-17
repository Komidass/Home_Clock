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
#include "timers.h"
#include "KBD_interface.h"
#include "PWM_T0.h"
#include "EEPROM.h"

u8 Seconds = 0;
u8 Minutes = 0;
u8 Hours = 0;
u8 AM_PM = AM;
u8 Alarm_Minutes = 0;
u8 Alarm_Hours = 0;
u8 Alarm_AM_PM = AM;
u16 flags = 0;
u8 current_block = second_row_start;
xSemaphoreHandle LCD ;
TaskHandle_t Minutes_handle = NULL;
TaskHandle_t Hours_handle = NULL;
TaskHandle_t Alarm_handle = NULL;
TaskHandle_t KPD_handle = NULL;
TaskHandle_t KPD_handle_INT = NULL;
TaskHandle_t Beep_handle = NULL;
TimerHandle_t KPD_INT_Timer;
TimerHandle_t Debounce_Timer;

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
	u8 pin_value;
	while(1)
	{
		//vTaskResume(Beep_handle);
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
	u8 Audio_Address = Clock_Alarm_Ringtone;
	while(1)
	{
		if(Get_Bit(flags,alarm_set) == 1)//alarm is set
		{
			if((Hours == Alarm_Hours)&&(Minutes == Alarm_Minutes)&&(AM_PM == Alarm_AM_PM))
			{
				xTaskCreate(Clock_Alarm,"Alarm",stack_alarm,(void*)&Audio_Address,priority_alarm,&Alarm_handle);
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

void Clock_Typing_Number(u8* pressed,u8 time_adjusted,u8* current_block,u8* hours,u8* minutes)//what happens when a number is pressed in typing mode
{
	/*change the time according to the user input*/
	switch (*current_block)
			{
			case hours_position+16:
				*hours = ((*pressed-'0')*10)+(*hours%10);
				Set_Bit(flags,time_adjusted);
				break;
			case hours_position+17:
				*hours = (*pressed-1-'0')+((*hours/10)*10);
				Set_Bit(flags,time_adjusted);
				break;
			case minutes_position+16:
				*minutes = ((*pressed-'0')*10)+(*minutes%10);
				Set_Bit(flags,time_adjusted);
				break;
			case minutes_position+17:
				*minutes = (*pressed-'0')+((*minutes/10)*10);
				Set_Bit(flags,time_adjusted);
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

void Clock_Typing_Exit(u8* current_block)//what happens when you exit typing mode
{
	xSemaphoreGive(LCD);
	LCD_Set_Block(second_row_start);
	Clock_Print_Default_Interface();//print default interface after giving back the LCD
	LCD_Set_Block(*current_block);
	LCD_Void_Write_Data(' ');
	current_block = second_row_start;
	if(Get_Bit(flags,time_adjusted_flag) == 1)
	{
		if(Minutes_handle != NULL)
		{
			vTaskDelete(Minutes_handle);
			Minutes_handle = NULL;
		}
		xTaskCreate(Clock_Minute,"minutes",stack_minutes,NULL,priority_minutes,&Minutes_handle);

		if(Hours_handle != NULL)
		{
			vTaskDelete(Hours_handle);
			Hours_handle = NULL;
		}
		Clear_Bit(flags,time_adjusted_flag);
		Seconds = 0;
	}

	xTaskCreate(Clock_Hours,"hours",stack_hours,NULL,priority_hours,&Hours_handle);


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

void Clock_Alarm_Enter(void)
{
	if(xSemaphoreTake(LCD,10))
	{
		current_block = second_row_start;
		LCD_Set_Block(current_block);
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

}
void Clock_Alarm_Exit(void)
{
	//alarm is set
	if(Get_Bit(flags,alarm_adjusted_flag) == 1)
	{
		Set_Bit(flags,alarm_set);
	}
	Clear_Bit(flags,alarm_adjusted_flag);
	LCD_Void_Clear();
	current_block = second_row_start;
	LCD_Set_Block(current_block);
	LCD_Void_Write_Data(Pixel_Arrow);
	Clear_Bit(flags,alarm_adjust);
	Clock_Print_Default_Interface();
	xSemaphoreGive(LCD);
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

void Clock_Alarm(void* pvParameters)//what happens when there is an alarm
{
	u8 Audio_Address = *(u8*)pvParameters;
	u8 note = 0x0;
	TickType_t freq;
	u8 index = 0;
	if(xSemaphoreTake(LCD,10))
	{
		Clock_Print_Alarm_Interface();
		Set_Bit(flags,alarm_latch);
	}
	while(1)
	{
		note = Get_4_Bits(eeprom_read_byte((u8*) Audio_Address + index/2), index%2);
				switch (note)
				{
			    default:
					PWM_Set_Prescalar(note & 0x7);
					PWM_Set_Duty(Get_Bit(note,3));
					break;
				case 0xF:
					freq = Fast_Rythm;
					break;
				case 0xE:
					freq = Slow_Rythm;
					break;
				}

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
		vTaskDelay(freq);
		index++;
		index = index % (Buzzer_Audio_Size/4);
	}
}
void Clock_Typing_Mode(void *pvParameters)
{
	u8 pressed  = 0xff;
	u8 keys[16];
	while(1)
	{
		pressed = KBD_u8GetKeyPadState(keys);
		if(pressed != 0xff)
		{
			vTaskResume(Beep_handle);
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
			default:
				//in time adjusting mode
				if(Get_Bit(flags,alarm_adjust) == 0)
				{
					Clock_Typing_Number(&pressed,time_adjusted_flag,&current_block,&Hours,&Minutes);
				}
				//in alarm setting mode
				else
				{
					Clock_Typing_Number(&pressed,alarm_adjusted_flag,&current_block,&Alarm_Hours,&Alarm_Minutes);

				}
				break;

			}
		}
		vTaskDelay(KPD_Check_frequency);
	}

}

void KPD_Button_INT(void)
{

	xTaskResumeFromISR(KPD_handle_INT);
	taskYIELD();

}

void SignlePress( TimerHandle_t xTimer )
{

	if((Get_Bit(flags,KPD_flag) == 0)&&(Get_Bit(flags,KPD_alarm_flag) == 0))
	{
		Clock_Typing_Enter(&current_block);
		vTaskResume(KPD_handle);

	}
	else if (Get_Bit(flags,KPD_alarm_flag) == 0)
	{

		Clock_Typing_Exit(&current_block);
		vTaskSuspend(KPD_handle);
	}
	Toggle_Bit(flags,KPD_flag);
}
void Debounce( TimerHandle_t xTimer )
{

}
void Clock_Semaphore_Init(void)
{
	LCD = xSemaphoreCreateMutex();
	KPD_INT_Timer  = xTimerCreate("timer",KPD_INT_double_press_frequency,pdFALSE,(void *) 0,SignlePress);
	Debounce_Timer = xTimerCreate("timer2",Debounce_frequency,pdFALSE,(void *) 1,Debounce);
	xTaskCreate(Clock_Typing_Mode,"KPD",stack_kpd_typing_mode,NULL,priority_kpd_typing_mode,&KPD_handle);
	xTaskCreate(KPD_Button_INT_ISR,"KPD_ISR1",stack_kpd_int,NULL,priority_kpd_int,&KPD_handle_INT);
	xTaskCreate(Clock_Beep,"Beep",stack_beep,NULL,priority_beep,&Beep_handle);
	vTaskSuspend(KPD_handle);



}

void KPD_Button_INT_ISR(void *pvParameters)
{


	while(1)
	{
		vTaskSuspend(NULL);//task suspend itself
		if(xTimerIsTimerActive(Debounce_Timer) == pdFALSE)//for debounce effect
		{
			vTaskResume(Beep_handle);
			xTimerReset(Debounce_Timer,10);
			/*
			 * single press , function is the timer function
			 */
			if(xTimerIsTimerActive(KPD_INT_Timer) == pdFALSE)
			{
				xTimerReset(KPD_INT_Timer,10);
			}
			/*
			 * double press
			 */
			else
			{

				xTimerStop(KPD_INT_Timer,10);

				/*
				 * if an alarm is set double pressing will cancel/stop the current alarm that is set/firing
				 */
				if (Get_Bit(flags,alarm_set) == 1)
				{
					Clear_Bit(flags,alarm_set);
				}
				else if((Get_Bit(flags,KPD_alarm_flag) == 0)&&(Get_Bit(flags,KPD_flag) == 0))
				{
					PWM_Stop();
					Clock_Alarm_Enter();
					vTaskResume(KPD_handle);
					Toggle_Bit(flags,KPD_alarm_flag);
				}
				else if(Get_Bit(flags,KPD_flag) == 0)
				{
					Clock_Alarm_Exit();
					vTaskSuspend(KPD_handle);
					Toggle_Bit(flags,KPD_alarm_flag);
				}


			}
		}
	}
}
void Clock_Beep(void *pvParameters)
{
	while(1)
	{
		vTaskSuspend(NULL);
		PWM_Set_Prescalar(2);
		PWM_Set_Duty(0);
		vTaskDelay(Beep_frequency);
		PWM_Stop();
	}
}

