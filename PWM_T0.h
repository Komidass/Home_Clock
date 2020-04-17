/*
 * PWM_T0.h
 *
 *  Created on: Apr 17, 2020
 *      Author: tho
 */

#ifndef PWM_T0_H_
#define PWM_T0_H_

#define OCR0 *((u8*) 0x5C)
#define TCCR0 *((u8*) 0x53)

#define Fast_Rythm 2
#define Slow_Rythm 5

/*
 * No sound
 */
#define Prescalar_0 0x0
/*
 * No prescaling
 */
#define Prescalar_1 0x1
/*
 * clk/8
 */
#define Prescalar_8 0x2
/*
 * clk/64
 */
#define Prescalar_64 0x3
/*
 * clk/256
 */
#define Prescalar_256 0x4
/*
 * clk/1024
 */
#define Prescalar_1024 0x5

void PWM_Stop(void);
void PWM_Set_Duty(u8 Duty);
void PWM_Set_Prescalar(u8 Prescalar);
void PWM_Read_Sound(u64 Sound);

#endif /* PWM_T0_H_ */
