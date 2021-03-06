/*
 * LCD_interface.h
 *
 *  Created on: Aug 24, 2019
 *      Author: thomas
 */

#ifndef LCD_INTERFACE_H_
#define LCD_INTERFACE_H_

// LCD pin direction

#define LCD_u8_RS_Pin	A5
#define LCD_u8_RW_Pin	A6
#define LCD_u8_E_Pin	A7
#define LCD_u8_D0_Pin	C0
#define LCD_u8_D1_Pin	C1
#define LCD_u8_D2_Pin	C2
#define LCD_u8_D3_Pin	C3
#define LCD_u8_D4_Pin	C4
#define LCD_u8_D5_Pin 	C5
#define LCD_u8_D6_Pin	C6
#define LCD_u8_D7_Pin	C7

/*
#define LCD_u8_RS_Pin	D7
#define LCD_u8_RW_Pin	D6
#define LCD_u8_E_Pin	D5
#define LCD_u8_D0_Pin	C0
#define LCD_u8_D1_Pin	C1
#define LCD_u8_D2_Pin	C2
#define LCD_u8_D3_Pin	C3
#define LCD_u8_D4_Pin	C4
#define LCD_u8_D5_Pin 	C5
#define LCD_u8_D6_Pin	C6
#define LCD_u8_D7_Pin	C7
*/
#define Up_Arrow 0x5E
#define Pixel_Alarm 0
#define Pixel_Arrow 1
#define Pixel_Mug 2
#define NumberOfCustomChars 3

void LCD_Void_Write_Cmd(u8 copy_u8_cmd);
void LCD_Void_Write_Data(u8 copy_u8_data);//writes characters
void LCD_initialize(void);
void LCD_Void_Write_String(u8* copy_string);//writes string
void LCD_Void_Clear(void);
void LCD_Void_Write_Moving_String(u8* copy_string);
void LCD_Set_Block(u8 copy_block_nb);
void LCD_Void_Write_CGRAM(u8 copy_rows[8],u8 copy_CGRAM_address);
void LCD_Void_Write_Number(u32 Number);
void LCD_Void_Write_Number_2(u8 Number);
void LCD_Void_Write_Number_1(u8 Number);
LCD_Void_Write_Number_3(u16 Number);

//load custom characters in the CGRAM
void LCD_Load_CGRAM(void);
#endif /* LCD_INTERFACE_H_ */
