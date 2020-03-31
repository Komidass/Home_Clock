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

#define seconds_position 6
#define minutes_position 3
#define hours_position 	 0

#define KPD_Check_frequency_Slow configTICK_RATE_HZ*2
#define KPD_Check_frequency_Fast 1

void Clock_Second(void *pvParameters);
void Clock_Minute(void *pvParameters);
void Clock_Hours(void *pvParameters);
void Clock_Check_KPD(void *pvParameters);
void Clock_Enter_Typing_Mode(void *pvParameters);
void Clock_Print_Default_Interface(void);


#endif /* CLOCK_H_ */
