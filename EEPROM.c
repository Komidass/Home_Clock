/*
 * EEPROM.c
 *
 *  Created on: Apr 21, 2020
 *      Author: tho
 */
#include "STD_Types.h"
#include "BIT_CALC.h"
#include "DIO_Interface.h"
#define F_CPU 8000000
#include "avr/delay.h"
#include "LCD_interface.h"
#include "avr/eeprom.h"
#include "EEPROM.h"



void Map_Write(u8 Map[11],u8 Map_add)
{
	eeprom_update_block((const void*) Map, (void*) EEPROM_Map_Start_add + Map_add*Map_Size, Map_Size);
}
void Map_Read(u8 Number,u8 Map_add)
{
	if ( Number < 10) // 1 digit
	{
		LCD_Void_Write_Data(eeprom_read_byte((u8*) EEPROM_Map_Start_add + Map_add*Map_Size + Number ));
	}
	else if (Number <100) // 2 digits
	{
		LCD_Void_Write_Data(eeprom_read_byte((u8*) EEPROM_Map_Start_add + Map_add*Map_Size + Number / 10 ));
		LCD_Void_Write_Data(eeprom_read_byte((u8*) EEPROM_Map_Start_add + Map_add*Map_Size + Number % 10));
	}
	else // 3 digits
	{
		LCD_Void_Write_Data(eeprom_read_byte((u8*) EEPROM_Map_Start_add + Map_add*Map_Size + Number / 100));
		LCD_Void_Write_Data(eeprom_read_byte((u8*) EEPROM_Map_Start_add + Map_add*Map_Size + (Number/10) % 10));
		LCD_Void_Write_Data(eeprom_read_byte((u8*) EEPROM_Map_Start_add + Map_add*Map_Size + Number % 10));
	}
}
/*
 * burns maps, the maps are stored in a 2D array of strings
 */
void Maps_Burner(u8 Maps[Maps_Number][Map_Size+1])
{
	for (u8 maps_ind = 0;maps_ind < Maps_Number ; maps_ind++)
	{
		Map_Write(Maps[maps_ind],maps_ind);
	}
}
