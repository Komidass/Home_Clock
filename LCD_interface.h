/*
 * LCD_interface.h
 *
 *  Created on: Aug 24, 2019
 *      Author: thomas
 */

#ifndef LCD_INTERFACE_H_
#define LCD_INTERFACE_H_

// LCD pin direction
#define LCD_u8_RS_Pin	C0
#define LCD_u8_RW_Pin	C1
#define LCD_u8_E_Pin	C2
#define LCD_u8_D0_Pin	A7
#define LCD_u8_D1_Pin	A6
#define LCD_u8_D2_Pin	A5
#define LCD_u8_D3_Pin	A4
#define LCD_u8_D4_Pin	A3
#define LCD_u8_D5_Pin 	A2
#define LCD_u8_D6_Pin	A1
#define LCD_u8_D7_Pin	A0


#define Pixel_Alarm 0
#define Pixel_Arrow 1

void LCD_Void_Write_Cmd(u8 copy_u8_cmd);
void LCD_Void_Write_Data(u8 copy_u8_data);//writes characters
void LCD_initialize(void);
void LCD_Void_Write_String(u8* copy_string);//writes string
void LCD_Void_Clear(void);
void LCD_Void_Write_Moving_String(u8* copy_string);
void LCD_Set_Block(u8 copy_block_nb);
void LCD_Void_Write_Number(u32 Number);
void LCD_Void_Write_Number_2(u8 Number);
void LCD_Void_Write_CGRAM(const u8 copy_rows[8],u8 copy_CGRAM_address);

#endif /* LCD_INTERFACE_H_ */
