/*
 * Clock.h
 *
 *  Created on: Mar 24, 2020
 *      Author: tho
 */

#ifndef CLOCK_H_
#define CLOCK_H_

#define seconds_frequency configTICK_RATE_HZ
#define minutes_frequency configTICK_RATE_HZ*60
#define hours_frequency   configTICK_RATE_HZ*3600

#define seconds_position 14
#define minutes_position 3
#define hours_position 	 0
#define Am_PM_position 6

#define second_row_start 16
#define KPD_Check_frequency_Slow configTICK_RATE_HZ*2
#define KPD_Check_frequency_Fast configTICK_RATE_HZ/configTICK_RATE_HZ

#define minimum_cursor_range 16
#define maximum_cursor_range 23

#define AM 0
#define PM 1

#define alarm_set 0 // alarm is set
#define alarm_adjust 1 // enter alarm adjusting mode


void Clock_Second(void *pvParameters);
void Clock_Minute(void *pvParameters);
void Clock_Hours(void *pvParameters);
void Clock_Check_KPD(void *pvParameters);
void Clock_Typing_Mode(void *pvParameters);
void Clock_Print_Default_Interface(void);
void Clock_Typing_Right_Arrow(u8* current_block);
void Clock_Typing_Left_Arrow(u8* current_block);
void Clock_Typing_Up_Arrow(u8* current_block);
void Clock_Typing_Number(u8* pressed,u8* time_adjusted,u8* current_block,u8* hours,u8* minutes);
void Clock_Typing_Exit(u8* take_lcd,u8* take_lach,u8* time_adjusted,u8* current_block);
void Clock_Typing_Enter(u8* take_lcd,u8* take_lach,u8* current_block);
void Clock_Alarm(void);
#endif /* CLOCK_H_ */
