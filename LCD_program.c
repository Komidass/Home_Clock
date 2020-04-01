/*
 * LCD_program.c
 *
 *  Created on: Aug 24, 2019
 *      Author: thomas
 */
#include "STD_Types.h"
#include "BIT_CALC.h"
#include "DIO_Interface.h"
#include "LCD_private.h"
#include "LCD_interface.h"
#include "LCD_config.h"
#define F_CPU 8000000
#include "avr/delay.h"

static void LCD_Void_Write(u8 copy_u8_value) // the user wont use this func because only other functions will use it so the prototype will be in private.h
//and to make this func visible only in this file not on the whole project we will put static in front of it
{
	DIO_u8SetPinValue(LCD_u8_RW_Pin,DIO_u8_LOW);
	DIO_u8SetPinValue(LCD_u8_D0_Pin,Get_Bit(copy_u8_value,0));
	DIO_u8SetPinValue(LCD_u8_D1_Pin,Get_Bit(copy_u8_value,1));
	DIO_u8SetPinValue(LCD_u8_D2_Pin,Get_Bit(copy_u8_value,2));
	DIO_u8SetPinValue(LCD_u8_D3_Pin,Get_Bit(copy_u8_value,3));
	DIO_u8SetPinValue(LCD_u8_D4_Pin,Get_Bit(copy_u8_value,4));
	DIO_u8SetPinValue(LCD_u8_D5_Pin,Get_Bit(copy_u8_value,5));
	DIO_u8SetPinValue(LCD_u8_D6_Pin,Get_Bit(copy_u8_value,6));
	DIO_u8SetPinValue(LCD_u8_D7_Pin,Get_Bit(copy_u8_value,7));
	DIO_u8SetPinValue(LCD_u8_E_Pin,DIO_u8_HIGH);
	_delay_ms(1);
	DIO_u8SetPinValue(LCD_u8_E_Pin,DIO_u8_LOW);
}

void LCD_Void_Write_Cmd(u8 copy_u8_cmd)
{
	DIO_u8SetPinValue (LCD_u8_RS_Pin,DIO_u8_LOW);
	LCD_Void_Write(copy_u8_cmd);
}

void LCD_Void_Write_Data(u8 copy_u8_data)
{
	DIO_u8SetPinValue(LCD_u8_RS_Pin,DIO_u8_HIGH);
	LCD_Void_Write(copy_u8_data);
}


void LCD_initialize(void)
{
	DIO_u8SetPinDirection(LCD_u8_RS_Pin,1);
	DIO_u8SetPinDirection(LCD_u8_RW_Pin,1);
	DIO_u8SetPinDirection(LCD_u8_E_Pin,1);
	DIO_u8SetPinDirection(LCD_u8_D0_Pin,1);
	DIO_u8SetPinDirection(LCD_u8_D1_Pin,1);
	DIO_u8SetPinDirection(LCD_u8_D2_Pin,1);
	DIO_u8SetPinDirection(LCD_u8_D3_Pin,1);
	DIO_u8SetPinDirection(LCD_u8_D4_Pin,1);
	DIO_u8SetPinDirection(LCD_u8_D5_Pin,1);
	DIO_u8SetPinDirection(LCD_u8_D6_Pin,1);
	DIO_u8SetPinDirection(LCD_u8_D7_Pin,1);
	_delay_ms(50);
	_delay_ms(40);
	LCD_Void_Write_Cmd(0b00111000);
	_delay_us(50);
	LCD_Void_Write_Cmd(0b00001100);
	_delay_us(50);
	LCD_Void_Write_Cmd(0b00000001);
	_delay_ms(2);
	LCD_Void_Write_CGRAM(Pattern_Alarm,Pixel_Alarm);
	LCD_Void_Write_CGRAM(Pattern_Arrow,Pixel_Arrow);
}

void LCD_Void_Write_String(u8* copy_string)
{
	u8 i = 0;
	while(copy_string[i] != '\0')
	{	if(i==16)
		{
			LCD_Set_Block(16);
		}
		LCD_Void_Write_Data(copy_string[i]);
		i++;
	}
}

void LCD_Void_Clear(void)
{
	LCD_Void_Write_Cmd(0b00000001);
}

void LCD_Void_Write_Blinking_String(u8* copy_string)
{
	u8 i = 0;
	while(copy_string[i] != '\0')
	{
		LCD_Void_Write_Data(copy_string[i]);
		_delay_ms(500);
		i++;
	}
}

void LCD_Set_Block(u8 copy_block_nb) // sets the ddram address to the block you want to write
{
	if(copy_block_nb<16)
	{
		LCD_Void_Write_Cmd(128+copy_block_nb);
	}
	else
	{
		LCD_Void_Write_Cmd(176+copy_block_nb);
	}
}

void LCD_Void_Write_Moving_String(u8* copy_string)
{
	u8 i;
	for (int j = 0; j<5 ;j++)
	{
		LCD_Set_Block(j);
		i = 0;
		while(copy_string[i] != '\0')
		{

			LCD_Void_Write_Data(copy_string[i]);
			i++;
		}
		_delay_ms(1000);
		LCD_Void_Clear();
	}
}

void LCD_Void_Write_CGRAM(const u8 copy_rows[8],u8 copy_CGRAM_address)
{
	LCD_Void_Write_Cmd(0x40 + copy_CGRAM_address*8);
	for(int i=0;i<8;i++)
	{
		LCD_Void_Write_Data(copy_rows[i]);
	}
	LCD_Set_Block(0);
}


void LCD_Void_Write_Number(u32 Number)
{
	u32 Reversed_Num = 0;
	u8 count = 0;
	while(Number > 0)
	{
		Reversed_Num = (Number%10) + (Reversed_Num*10);
		Number /= 10;
		count ++;
	}

	for( Number=0;Number<count;Number++)
	{
		LCD_Void_Write_Data((Reversed_Num%10)+'0');
		Reversed_Num /= 10;
	}

}


void LCD_Void_Write_Number_2(u8 Number)
{

	LCD_Void_Write_Data((Number/10)+'0');
	LCD_Void_Write_Data((Number%10)+'0');

}

void LCD_Void_Write_Number_1(u8 Number)
{
	LCD_Void_Write_Data(Number+'0');
}



