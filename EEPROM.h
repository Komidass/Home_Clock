/*
 * EEPROM.h
 *
 *  Created on: Apr 17, 2020
 *      Author: tho
 */

#ifndef EEPROM_H_
#define EEPROM_H_

#define LCD_Custom_Char_Size 8
#define Buzzer_Audio_Size 128
#define Map_Size 10
#define Maps_Number 64

#define EEPROM_Pixel_Alarm__add 0
#define EEPROM_Pixel_Arrow_add 8

#define EEPROM_SpongeBob_add 240

#define EEPROM_Map_Start_add 368
#define EEPROM_Map_1_add 0
void Map_Write(u8 Map[11],u8 Map_add);
void Map_Read(u8 Number,u8 Map_add);
void Maps_Burner(u8 Maps[Maps_Number][Map_Size+1]);

/*
 u8 Pattern_Alarm[8] = {
	0x00,
  	0x11,
	0x0E,
	0x15,
	0x17,
	0x11,
	0x0E,
	0x11
};

 u8 Pattern_Arrow[8]= {
		  0x00,
		  0x00,
		  0x04,
		  0x0A,
		  0x15,
		  0x04,
		  0x04,
		  0x04
};
*/

/*
 * previous writes performed
 */
/*eeprom_update_block((const void*) Pattern_Alarm, (void*) EEPROM_Pixel_Alarm__add, 8);
eeprom_update_block((const void*) Pattern_Arrow, (void*) EEPROM_Pixel_Arrow_add, 8);

	uint64_t SpongeBob[2];
	SpongeBob[0] = 0xA030B0A030B0A03E;
	SpongeBob[1] = 0x0909009090090FB0;
	eeprom_update_block((const void*) SpongeBob, (void*) EEPROM_SpongeBob_add, Buzzer_Audio_Size);
*/


#endif /* EEPROM_H_ */
