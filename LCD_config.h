/*
 * LCD_config.h
 *
 *  Created on: Aug 24, 2019
 *      Author: thomas
 */

#ifndef LCD_CONFIG_H_
#define LCD_CONFIG_H_

const u8 Pattern_Alarm[8] = {
  0x00,
  0x00,
  0x11,
  0x0E,
  0x15,
  0x17,
  0x11,
  0x0E
};

const u8 Pattern_AM[8] = {
  0x0E,
  0x11,
  0x1F,
  0x11,
  0x11,
  0x1B,
  0x15,
  0x11
};

const u8 Pattern_Arrow[8]= {
		  0x00,
		  0x00,
		  0x04,
		  0x0A,
		  0x15,
		  0x04,
		  0x04,
		  0x04
};


#endif /* LCD_CONFIG_H_ */
