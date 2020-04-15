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
#define Set_Alarm_Position 23
#define Alarm_Icon_Position 9
#define Alarm_Text_Position 21

#define second_row_start 16
#define KPD_Check_frequency_Slow configTICK_RATE_HZ*2
#define KPD_Check_frequency_Fast configTICK_RATE_HZ/configTICK_RATE_HZ

#define minimum_cursor_range 16
#define maximum_cursor_range 23

#define AM 0
#define PM 1

//flags
#define alarm_set 0 // alarm is set
#define alarm_adjust 1 // enter alarm adjusting mode
#define alarm_text_toggle 2//to toggle the alarm string text
#define alarm_latch 3

//what happens every second
void Clock_Second(void *pvParameters);
//what happens every minute
void Clock_Minute(void *pvParameters);
//what happens every hour
void Clock_Hours(void *pvParameters);

void Clock_Check_KPD(void *pvParameters);
//what happens in the typing mode
void Clock_Typing_Mode(void *pvParameters);
//the default interface of the clock
void Clock_Print_Default_Interface(void);
//what happens when the right arrow is pressed in typing mode
void Clock_Typing_Right_Arrow(u8* current_block);
//what happens when the left arrow is pressed in typing mode
void Clock_Typing_Left_Arrow(u8* current_block);
//what happens when the up arrow is pressed in typing mode
void Clock_Typing_Up_Arrow(u8* current_block,u8* am_pm);
//what happens when an number is pressed in typing mode
void Clock_Typing_Number(u8* pressed,u8* time_adjusted,u8* current_block,u8* hours,u8* minutes);
//what happens when you press the alarm button
void Clock_Typing_Alarm_Button(void);
//what happens when exiting typing mode
void Clock_Typing_Exit(u8* take_lcd,u8* take_lach,u8* time_adjusted,u8* current_block);
//what happens when you enter typing mode
void Clock_Typing_Enter(u8* take_lcd,u8* take_lach,u8* current_block);
//what happens in the alarm
void Clock_Alarm(void);
//print the alarm interface
void Clock_Print_Alarm_Interface(void);
#endif /* CLOCK_H_ */
