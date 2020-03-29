/*******************************************************/
/* Author  : Bishoy Medhat                             */
/* Version : V01                                       */
/* Date    : 4 FEB 2019                                */
/*******************************************************/
/* Description                                         */
/* ------------                                        */
/* This file defines the APIs corresponding to DIO     */
/*******************************************************/

/* Comment: Header file guard                          */
#ifndef DIO_Interface_H
#define DIO_Interface_H

/* Comment: DIO Pin index assignation                  */
#define DIO_u8_PIN_0		0
#define DIO_u8_PIN_1		1
#define DIO_u8_PIN_2		2
#define DIO_u8_PIN_3		3
#define DIO_u8_PIN_4		4
#define DIO_u8_PIN_5		5
#define DIO_u8_PIN_6		6
#define DIO_u8_PIN_7		7
#define DIO_u8_PIN_8		8
#define DIO_u8_PIN_9		9
#define DIO_u8_PIN_10		10
#define DIO_u8_PIN_11		11
#define DIO_u8_PIN_12		12
#define DIO_u8_PIN_13		13
#define DIO_u8_PIN_14		14
#define DIO_u8_PIN_15		15
#define DIO_u8_PIN_16		16
#define DIO_u8_PIN_17		17
#define DIO_u8_PIN_18		18
#define DIO_u8_PIN_19		19
#define DIO_u8_PIN_20		20
#define DIO_u8_PIN_21		21
#define DIO_u8_PIN_22		22
#define DIO_u8_PIN_23		23
#define DIO_u8_PIN_24		24
#define DIO_u8_PIN_25		25
#define DIO_u8_PIN_26		26
#define DIO_u8_PIN_27		27
#define DIO_u8_PIN_28		28
#define DIO_u8_PIN_29		29
#define DIO_u8_PIN_30		30
#define DIO_u8_PIN_31		31

/* */
#define GROUP_A 0
#define GROUP_B 1
#define GROUP_C 2
#define GROUP_D 3

/* Comment: DIO Pin Value                             */
#define DIO_u8_HIGH			1
#define DIO_u8_LOW			0

/* Comment: DIO Pin Direction                         */
#define DIO_u8_OUTPUT		1
#define DIO_u8_INPUT		0

/* */
#define DIO_u8_MAXPINNB		32
#define DIO_u8_MAXPORTNB	4
#define DIO_u8_MAXPORTDIR 	0xff
#define DIO_u8_MINPORTDIR	0x00
#define DIO_u8_MAXPORTVAL 	0xff
#define DIO_u8_MINPORTVAL 	0x00

enum pins // giving pins numbers..
{
	A0,
	A1,
	A2,
	A3,
	A4,
	A5,
	A6,
	A7,
	B0,
	B1,
	B2,
	B3,
	B4,
	B5,
	B6,
	B7,
	C0,
	C1,
	C2,
	C3,
	C4,
	C5,
	C6,
	C7,
	D0,
	D1,
	D2,
	D3,
	D4,
	D5,
	D6,
	D7
};

/* Description: This function shall initialize the     */
/*              direction and initial value for DIO    */
/*              pins                                   */
void DIO_voidInitialize(void);

/* Description: This function shall set certain value  */
/*              to certain DIO channel                 */
u8 DIO_u8SetPinDirection(u8 Copy_u8PinNB, u8 Copy_u8PinDirection);

/* */
/* */
u8 DIO_u8SetPinValue(u8 Copy_u8PinNB, u8 Copy_u8PinValue);

/* */
/* */
u8 DIO_u8SetPortDirection(u8 Copy_u8PortNB, u8 Copy_u8PortDirection);

/* */
/* */
u8 DIO_u8SetPortValue(u8 Copy_u8PortNB, u8 Copy_u8PortValue);
/* */
/* */
u8 DIO_u8GetPinValue(u8 Copy_u8PinNB, u8 *Copy_u8PinValue);
/* */
/* */
u8 DIO_u8GetPortValue(u8 Copy_u8PortNB, u8 *Copy_u8PortValue);

#endif
