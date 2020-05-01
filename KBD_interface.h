/*******************************************************/
/* Author  : Bishoy Medhat                             */
/* Version : V1.0                                      */
/* Date    : 26 FEB 2019                               */
/*******************************************************/
/* Description                                         */
/* ------------                                        */
/* This file defines the APIs corresponding to Keypad  */
/*******************************************************/

/* Comment: Header file guard                          */
#ifndef KBD_INTERFACE_H
#define KBD_INTERFACE_H

#define KBD_u8_KEYS_NB			16 // KBD_u8_ROWS * KBD_u8_COL

#define KBD_u8_NOT_PRESSED		0
#define KBD_u8_PRESSED			1

#define KBD_u8_ACTIVE_COL		0
#define KBD_u8_ACTIVE_ROW		1

#define KBD_u8_CON_PULLDOWN		0
#define KBD_u8_CON_PULLUP		1
//embedded

#define R1 B7
#define R2 B6
#define R3 B5
#define R4 B4
#define C1 B2
#define C2 B1
#define C3 B0
#define C4 D1

/*
#define R1 A7
#define R2 A6
#define R3 A5
#define R4 A4
#define C1 A3
#define C2 A2
#define C3 A1
#define C4 A0
*/


u8 KBD_u8GetKeyPadState(u8 Copy_Au8KeysState[KBD_u8_KEYS_NB]);
void KBD_u8Initialize(u8 group_nb);
u8 KBD_keys_map(u8* keys);
void KBD_u8Initialize_2(void);
#endif
