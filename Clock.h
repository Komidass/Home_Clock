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
#define KPD_INT_double_press_frequency configTICK_RATE_HZ
#define Debounce_frequency configTICK_RATE_HZ/4

#define seconds_position 14
#define minutes_position 3
#define hours_position 	 0
#define Am_PM_position 6
#define Set_Alarm_Position 23
#define Alarm_Icon_Position 9
#define Alarm_Text_Position 21

#define second_row_start 16
#define KPD_Check_frequency configTICK_RATE_HZ/3

#define minimum_cursor_range 16
#define maximum_cursor_range 23

#define AM 0
#define PM 1

//flags
#define alarm_set 0 // alarm is set
#define alarm_adjust 1 // enter alarm adjusting mode
#define alarm_text_toggle 2//to toggle the alarm string text
#define alarm_latch 3
#define KPD_flag 4
#define timer_ended_flag 5
#define LED_toggle 6
#define KPD_alarm_flag 7
#define time_adjusted_flag 8
#define alarm_adjusted_flag 9

/*
 * stack sizes of tasks
 */
#define stack_seconds 60
#define stack_minutes 70
#define stack_hours 70
#define stack_kpd_typing_mode 85
#define stack_alarm 60
#define stack_kpd_int 85

/*
 * task priorities
 */

#define priority_seconds 1
#define priority_minutes 2
#define priority_hours 3
#define priority_kpd_typing_mode 4
#define priority_alarm 5
#define priority_kpd_int 4

/*
 * what happens every second
 */
void Clock_Second(void *pvParameters);
/*
 * what happens every minute
 */
void Clock_Minute(void *pvParameters);
/*
 * what happens every hour
 */
void Clock_Hours(void *pvParameters);
/*
 *
 */
void Clock_Typing_Mode(void *pvParameters);
/*
 * the default interface of the clock
 */
void Clock_Print_Default_Interface(void);
/*
 * what happens when the right arrow is pressed in typing mode
 */
void Clock_Typing_Right_Arrow(u8* current_block);
/*
 * what happens when the left arrow is pressed in typing mode
 */
void Clock_Typing_Left_Arrow(u8* current_block);
/*
 * what happens when the up arrow is pressed in typing mode
 */
void Clock_Typing_Up_Arrow(u8* current_block,u8* am_pm);
/*
 * what happens when an number is pressed in typing mode
 */
void Clock_Typing_Number(u8* pressed,u8 time_adjusted,u8* current_block,u8* hours,u8* minutes);

/*
 * what happens when exiting typing mode
 */
void Clock_Typing_Exit(u8* current_block);
/*
 * what happens when you enter typing mode
 */
void Clock_Typing_Enter(u8* current_block);
/*
 * what happens in the alarm
 */
void Clock_Alarm(void);
/*
 * what happens when you enter alarm setting mode
 */
void Clock_Alarm_Enter(void);
/*
 * what happens when you exit alarm setting mode
 */
void Clock_Alarm_Exit(void);
/*
 * print the alarm interface
 */
void Clock_Print_Alarm_Interface(void);
/*
 * what happens when the KPD button interrupt happens (which starts scanning for KPD input)
 */
void KPD_Button_INT(void);
/*
 * KPD_INT_ISR
 */

void KPD_Button_INT_ISR(void *pvParameters);
/*
 * create semaphores
 */
void Clock_Semaphore_Init(void);
/*
 *
 */

#endif /* CLOCK_H_ */
