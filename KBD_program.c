/*******************************************************/
/* Author  : Bishoy Medhat                             */
/* Version : V1.0                                      */
/* Date    : 26 FEB 2019                               */
/*******************************************************/
/* Description                                         */
/* ------------                                        */
/* This file has the implementation of both private and*/
/* public function corresponding to Keypad module      */
/*******************************************************/

#include "BIT_CALC.h"
#include "STD_Types.h"

//MCAL includes
#include "DIO_Interface.h"
#include "KBD_config.h"
#include "KBD_interface.h"
#include "KBD_private.h"


u8 KBD_u8GetKeyPadState(u8* Copy_Au8KeysState/*[KBD_u8_KEYS_NB]*/)
{
	u8 LOCAL_u8ColIndex = 0;
	u8 LOCAL_u8RowIndex = 0;
	
	u8 Local_u8CurrentPinValue=0;
	u8 Local_u8KeyIndex=0;
	u8 pressed = 0xff;
	// check on Activation type
	#if KBD_u8_ACTIVATION_TYPE==KBD_u8_ACTIVE_COL
		for(LOCAL_u8ColIndex = 0; LOCAL_u8ColIndex < KBD_u8_COL; LOCAL_u8ColIndex ++ )
		{
			//activate current col
			#if KBD_u8_CONNECTION_TYPE==KBD_u8_CON_PULLUP
				DIO_u8SetPinValue(KBD_Au8ColsPins[LOCAL_u8ColIndex], DIO_u8_LOW);
			#else
				DIO_u8SetPinValue(KBD_Au8ColsPins[LOCAL_u8ColIndex], DIO_u8_HIGH);
			#endif
			
			//Check on active ROW
			
			for(LOCAL_u8RowIndex = 0; LOCAL_u8RowIndex < KBD_u8_ROWS; LOCAL_u8RowIndex ++)
			{
				DIO_u8GetPinValue(KBD_Au8RowsPins[LOCAL_u8RowIndex], &Local_u8CurrentPinValue);
				
				Local_u8KeyIndex = LOCAL_u8ColIndex + (LOCAL_u8RowIndex*KBD_u8_COL);
				
				if(Local_u8CurrentPinValue == DIO_u8_HIGH)
				{
					#if KBD_u8_CONNECTION_TYPE==KBD_u8_CON_PULLUP
					//Not Pressed
						Copy_Au8KeysState[Local_u8KeyIndex]= KBD_u8_NOT_PRESSED;

					#else
					//Pressed
						Copy_Au8KeysState[Local_u8KeyIndex]= KBD_u8_PRESSED;
						return key_map[Local_u8KeyIndex];

					#endif
				}else
				{
					#if KBD_u8_CONNECTION_TYPE==KBD_u8_CON_PULLUP
					//Pressed
						Copy_Au8KeysState[Local_u8KeyIndex]= KBD_u8_PRESSED;
						pressed = key_map[Local_u8KeyIndex];
					#else
					//Not Pressed
						Copy_Au8KeysState[Local_u8KeyIndex]= KBD_u8_NOT_PRESSED;
					#endif
				}
			}
			#if KBD_u8_CONNECTION_TYPE==KBD_u8_CON_PULLUP
				DIO_u8SetPinValue(KBD_Au8ColsPins[LOCAL_u8ColIndex], DIO_u8_HIGH);
			#else
				DIO_u8SetPinValue(KBD_Au8ColsPins[LOCAL_u8ColIndex], DIO_u8_LOW);
			#endif
		}
		return pressed;
	#else
		for(LOCAL_u8RowIndex = 0; LOCAL_u8RowIndex < KBD_u8_ROWS; LOCAL_u8RowIndex ++)
		{
			//activate current row
			#if KBD_u8_CONNECTION_TYPE==KBD_u8_CON_PULLUP
				DIO_u8SetPinValue(KBD_Au8RowsPins[LOCAL_u8RowIndex], DIO_u8_LOW);
			#else
				DIO_u8SetPinValue(KBD_Au8RowsPins[LOCAL_u8RowIndex], DIO_u8_HIGH);
			#endif
			
			//Check on active COL
			
			for(LOCAL_u8ColIndex = 0; LOCAL_u8ColIndex < KBD_u8_COL; LOCAL_u8ColIndex ++ )
			{
				b(KBD_Au8ColsPins[LOCAL_u8ColIndex], &Local_u8CurrentPinValue);
				
				Local_u8KeyIndex = LOCAL_u8RowIndex + (LOCAL_u8ColIndex*KBD_u8_ROWS);
				
				if(Local_u8CurrentPinValue == DIO_u8_HIGH)
				{
					#if KBD_u8_CONNECTION_TYPE==KBD_u8_CON_PULLUP
					//Not Pressed
						Copy_Au8KeysState[Local_u8KeyIndex]= KBD_u8_NOT_PRESSED;
					#else
					//Pressed
						Copy_Au8KeysState[Local_u8KeyIndex]= KBD_u8_PRESSED;
					#endif
				}else
				{
					#if KBD_u8_CONNECTION_TYPE==KBD_u8_CON_PULLUP
					//Pressed
						Copy_Au8KeysState[Local_u8KeyIndex]= KBD_u8_PRESSED;
					#else
					//Not Pressed
						Copy_Au8KeysState[Local_u8KeyIndex]= KBD_u8_NOT_PRESSED;
					#endif
				}
			}
			#if KBD_u8_CONNECTION_TYPE==KBD_u8_CON_PULLUP
				DIO_u8SetPinValue(KBD_Au8RowsPins[LOCAL_u8RowIndex], DIO_u8_HIGH);
			#else
				DIO_u8SetPinValue(KBD_Au8RowsPins[LOCAL_u8RowIndex], DIO_u8_LOW);
			#endif
		}

	#endif

}
void KBD_u8Initialize_2(void) //initialize keypad active col pull up with Row 4 to be the the 8th pin in the port and col 4 to the 1st pin
	{

	DIO_u8SetPinDirection(R1,0);
	DIO_u8SetPinDirection(R2,0);
	DIO_u8SetPinDirection(R3,0);
	DIO_u8SetPinDirection(R4,0);
	DIO_u8SetPinDirection(C1,1);
	DIO_u8SetPinDirection(C2,1);
	DIO_u8SetPinDirection(C3,1);
	DIO_u8SetPinDirection(C4,1);
	DIO_u8SetPinValue(R1,1);
	DIO_u8SetPinValue(R2,1);
	DIO_u8SetPinValue(R3,1);
	DIO_u8SetPinValue(R4,1);
	KBD_Au8RowsPins[0] = R1;
	KBD_Au8RowsPins[1] = R2;
	KBD_Au8RowsPins[2] = R3;
	KBD_Au8RowsPins[3] = R4;
	KBD_Au8ColsPins[0] = C1;
	KBD_Au8ColsPins[1] = C2;
	KBD_Au8ColsPins[2] = C3;
	KBD_Au8ColsPins[3] = C4;

	}
void KBD_u8Initialize(u8 group_nb) //initialize keypad active col pull up with Row 4 to be the the 8th pin in the port and col 4 to the 1st pin
	{
		switch (group_nb)
		{
		case 0:
			DIO_u8SetPortDirection(GROUP_A,0b00001111);
			DIO_u8SetPinValue(A4,1);
			DIO_u8SetPinValue(A5,1);
			DIO_u8SetPinValue(A6,1);
			DIO_u8SetPinValue(A7,1);
			for (int i=0;i<4;i++)
			{
				KBD_Au8RowsPins[i] = A7 - i;
				KBD_Au8ColsPins[i] = A3 - i;
			}
			break;
		case 1:
			DIO_u8SetPortDirection(GROUP_B,0b00001111);
			DIO_u8SetPinValue(B4,1);
			DIO_u8SetPinValue(B5,1);
			DIO_u8SetPinValue(B6,1);
			DIO_u8SetPinValue(B7,1);
			for (int i=0;i<4;i++)
			{
				KBD_Au8RowsPins[i] = B7 - i;
				KBD_Au8ColsPins[i] = B3 - i;
			}
			break;
		case 2:
			DIO_u8SetPortDirection(GROUP_C,0b00001111);
			DIO_u8SetPinValue(C4,1);
			DIO_u8SetPinValue(C5,1);
			DIO_u8SetPinValue(C6,1);
			DIO_u8SetPinValue(C7,1);
			for (int i=0;i<4;i++)
			{
				KBD_Au8RowsPins[i] = C7 - i;
				KBD_Au8ColsPins[i] = C3 - i;
			}
			break;
		case 3:
			DIO_u8SetPortDirection(GROUP_D,0b00001111);
			DIO_u8SetPinValue(D4,1);
			DIO_u8SetPinValue(D5,1);
			DIO_u8SetPinValue(D6,1);
			DIO_u8SetPinValue(D7,1);
			for (int i=0;i<4;i++)
			{
				KBD_Au8RowsPins[i] = D7 - i;
				KBD_Au8ColsPins[i] = D3 - i;
			}
			break;
		}
	}
u8 KBD_keys_map(u8* keys)
{
    u8 key_index;
    for(key_index = 0;key_index < KBD_u8_ROWS*KBD_u8_COL;key_index++)
    {
        if (keys[key_index] == KBD_u8_PRESSED) return key_map[key_index];
    }
    return 0xff;
}
