/*
 * PWM_T0.c
 *
 *  Created on: Apr 17, 2020
 *      Author: tho
 */
#include "STD_Types.h"
#include "BIT_CALC.h"
#include "PWM_T0.h"
#define F_CPU 8000000
#include "avr/delay.h"
#include "avr/eeprom.h"
void PWM_Stop(void)
{
	TCCR0 = 0x00;
}
void PWM_Set_Prescalar(u8 Prescalar)
{
	if(Prescalar == 0) TCCR0 = 0x00;
	else TCCR0 = Prescalar | 0x68;
}
/*
 * 255 corresponds to 100% duty
 */
void PWM_Set_Duty(u8 Duty)
{
	/*
	 * 0 --> 80
	 * 1 --> 240
	 */

	OCR0 = 80 + Duty*170;
}
void PWM_Read_Sound(u64 Sound)
{
	char size = sizeof(Sound)*8;
	char note = 0x0;
	for(int index = 0; index < size/4 ; index++)
	{
		note = Get_4_Bits(Sound,index);
		PWM_Set_Prescalar(note & 0x7);
		PWM_Set_Duty(Get_Bit(note,3));

	}
}
