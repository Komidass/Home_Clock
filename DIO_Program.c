/*******************************************************/
/* Author  : Bishoy Medhat                             */
/* Version : V01                                       */
/* Date    : 4 FEB 2019                                */
/*******************************************************/
/* Description                                         */
/* ------------                                        */
/* This file has the implementation of both private and*/
/* public function corresponding to DIO module         */
/*******************************************************/
#include "STD_Types.h"
#include "BIT_CALC.h"
#include "DIO_Private.h"
#include "DIO_Config.h"
#include "DIO_Interface.h"

#define CONC_8BIT(b0,b1,b2,b3,b4,b5,b6,b7) CONC_HELPER(b0,b1,b2,b3,b4,b5,b6,b7)
#define CONC_HELPER(b0,b1,b2,b3,b4,b5,b6,b7) 0b##b7##b6##b5##b4##b3##b2##b1##b0

/* Description: This function shall initialize the     */
/*              direction and initial value for DIO    */
/*              pins                                   */
void DIO_voidInitialize(void)
{
	DIO_u8_DDRA->ByteAccess = CONC_8BIT(DIO_u8_PIN0_DIR,
										DIO_u8_PIN1_DIR,
										DIO_u8_PIN2_DIR,
										DIO_u8_PIN3_DIR,
										DIO_u8_PIN4_DIR,
										DIO_u8_PIN5_DIR,
										DIO_u8_PIN6_DIR,
										DIO_u8_PIN7_DIR);

	DIO_u8_DDRB->ByteAccess = CONC_8BIT(DIO_u8_PIN8_DIR,
										DIO_u8_PIN9_DIR,
										DIO_u8_PIN10_DIR,
										DIO_u8_PIN11_DIR,
										DIO_u8_PIN12_DIR,
										DIO_u8_PIN13_DIR,
										DIO_u8_PIN14_DIR,
										DIO_u8_PIN15_DIR);

	DIO_u8_DDRC->ByteAccess = CONC_8BIT(DIO_u8_PIN16_DIR,
										DIO_u8_PIN17_DIR,
										DIO_u8_PIN18_DIR,
										DIO_u8_PIN19_DIR,
										DIO_u8_PIN20_DIR,
										DIO_u8_PIN21_DIR,
										DIO_u8_PIN22_DIR,
										DIO_u8_PIN23_DIR);

	DIO_u8_DDRD->ByteAccess = CONC_8BIT(DIO_u8_PIN24_DIR,
										DIO_u8_PIN25_DIR,
										DIO_u8_PIN26_DIR,
										DIO_u8_PIN27_DIR,
										DIO_u8_PIN28_DIR,
										DIO_u8_PIN29_DIR,
										DIO_u8_PIN30_DIR,
										DIO_u8_PIN31_DIR);
}

/* Description: This function shall set certain        */
/*              direction to certain DIO_pin           */
u8 DIO_u8SetPinDirection(u8 Copy_u8PinNB, u8 Copy_u8PinDirection)
{
	/*Local variables*/
	u8 u8ErrorState = 0;
	u8 u8PORT_ID;
	u8 u8PIN_ID;

	/*validate inputs*/
	if (Copy_u8PinNB >= DIO_u8_MAXPINNB)
	{
		u8ErrorState = 1;
	}else if ((Copy_u8PinDirection != DIO_u8_INPUT) && (Copy_u8PinDirection != DIO_u8_OUTPUT))
	{
		u8ErrorState = 1;
	}else
	{
//		u8ErrorState = 0;
		u8PORT_ID = Copy_u8PinNB/DIO_u8_PINS_PER_PORT;
		u8PIN_ID = Copy_u8PinNB%DIO_u8_PINS_PER_PORT;
		switch(u8PORT_ID)
		{
		case GROUP_A :
			Assign_Bit(DIO_u8_DDRA->ByteAccess, u8PIN_ID, Copy_u8PinDirection);
			break;
		case GROUP_B :
			if(Copy_u8PinDirection == DIO_u8_INPUT)
			{
				Clear_Bit(DIO_u8_DDRB->ByteAccess, u8PIN_ID);
			}else
			{
				Set_Bit(DIO_u8_DDRB->ByteAccess, u8PIN_ID);
			}
			break;
		case GROUP_C :
			Assign_Bit(DIO_u8_DDRC->ByteAccess, u8PIN_ID, Copy_u8PinDirection);
			break;
		case GROUP_D :
			Assign_Bit(DIO_u8_DDRD->ByteAccess, u8PIN_ID, Copy_u8PinDirection);
			break;
		default:

			break;
		}
	}

	/*Function return*/
	return u8ErrorState;
}
/* Description: This function shall set certain value  */
/*              to certain DIO pin                     */
u8 DIO_u8SetPinValue(u8 Copy_u8PinNB, u8 Copy_u8PinValue)
{
	/*Local variables*/
	u8 u8ErrorState = 0;
	u8 u8PORT_ID;
	u8 u8PIN_ID;

	/*validate inputs*/
	if (Copy_u8PinNB >= DIO_u8_MAXPINNB)
	{
		u8ErrorState = 1;
	}else if ((Copy_u8PinValue != DIO_u8_HIGH) && (Copy_u8PinValue != DIO_u8_LOW))
	{
		u8ErrorState = 1;
	}else
	{
//		u8ErrorState = 0;
		u8PORT_ID = Copy_u8PinNB/DIO_u8_PINS_PER_PORT;
		u8PIN_ID = Copy_u8PinNB%DIO_u8_PINS_PER_PORT;
		switch(u8PORT_ID)
		{
		case GROUP_A :
			Assign_Bit(DIO_u8_PORTA -> ByteAccess, u8PIN_ID, Copy_u8PinValue);
			break;
		case GROUP_B :
			if(Copy_u8PinValue == DIO_u8_LOW)
			{
				Clear_Bit(DIO_u8_PORTB -> ByteAccess, u8PIN_ID);
			}else
			{
				Set_Bit(DIO_u8_PORTB -> ByteAccess, u8PIN_ID);
			}
			break;
		case GROUP_C :
			Assign_Bit(DIO_u8_PORTC -> ByteAccess, u8PIN_ID, Copy_u8PinValue);
			break;
		case GROUP_D :
			Assign_Bit(DIO_u8_PORTD -> ByteAccess, u8PIN_ID, Copy_u8PinValue);
			break;
		default:

			break;
		}
	}

	/*Function return*/
	return u8ErrorState;
}

/* Description: This function shall set certain        */
/*              direction to certain DIO_port          */
u8 DIO_u8SetPortDirection(u8 Copy_u8PortNB, u8 Copy_u8PortDirection)
{
	/*Local variables*/
	u8 u8ErrorState = 0;

	/*validate inputs*/
		if (Copy_u8PortNB >= DIO_u8_MAXPORTNB)
		{
			u8ErrorState = 1;
		}else if ((Copy_u8PortDirection > DIO_u8_MAXPORTDIR) || (Copy_u8PortDirection < DIO_u8_MINPORTDIR))
		{
			u8ErrorState = 1;
		}else
		{
	//		u8ErrorState = 0;

			switch(Copy_u8PortNB)
			{
			case GROUP_A :
				DIO_u8_DDRA->ByteAccess = Copy_u8PortDirection;
				break;
			case GROUP_B :
				DIO_u8_DDRB->ByteAccess = Copy_u8PortDirection;
				break;
			case GROUP_C :
				DIO_u8_DDRC->ByteAccess = Copy_u8PortDirection;
				break;
			case GROUP_D :
				DIO_u8_DDRD->ByteAccess = Copy_u8PortDirection;
				break;
			default:

				break;
			}
		}
	/*Function return*/
	return u8ErrorState;
}

/* Description: This function shall set certain        */
/*              value to certain DIO_port              */
u8 DIO_u8SetPortValue(u8 Copy_u8PortNB, u8 Copy_u8PortValue)
{
		/*Local variables*/
		u8 u8ErrorState = 0;

		/*validate inputs*/
			if (Copy_u8PortNB >= DIO_u8_MAXPORTNB)
			{
				u8ErrorState = 1;
			}else if ((Copy_u8PortValue > DIO_u8_MAXPORTVAL) || (Copy_u8PortValue < DIO_u8_MINPORTVAL))
			{
				u8ErrorState = 1;
			}else
			{
		//		u8ErrorState = 0;

				switch(Copy_u8PortNB)
				{
				case GROUP_A :
					DIO_u8_PORTA->ByteAccess = Copy_u8PortValue;
					break;
				case GROUP_B :
					DIO_u8_PORTB->ByteAccess = Copy_u8PortValue;
					break;
				case GROUP_C :
					DIO_u8_PORTC->ByteAccess = Copy_u8PortValue;
					break;
				case GROUP_D :
					DIO_u8_PORTD->ByteAccess = Copy_u8PortValue;
					break;
				default:

					break;
				}
			}
		/*Function return*/
		return u8ErrorState;
}

/* */
/* */
u8 DIO_u8GetPinValue(u8 Copy_u8PinNB, u8 *Copy_u8PinValue)
{
	/*Local variables*/
	u8 u8ErrorState = 0;
	u8 u8PORT_ID;
	u8 u8PIN_ID;

	/*validate inputs*/
	if (Copy_u8PinNB >= DIO_u8_MAXPINNB)
	{
		u8ErrorState = 1;
	}else
	{
		u8PORT_ID = Copy_u8PinNB/DIO_u8_PINS_PER_PORT;
		u8PIN_ID = Copy_u8PinNB%DIO_u8_PINS_PER_PORT;
		switch(u8PORT_ID)
		{
		case GROUP_A :
			*Copy_u8PinValue = Get_Bit(DIO_u8_PINA -> ByteAccess, u8PIN_ID);
			break;
		case GROUP_B :
			*Copy_u8PinValue = Get_Bit(DIO_u8_PINB -> ByteAccess, u8PIN_ID);
			break;
		case GROUP_C :
			*Copy_u8PinValue = Get_Bit(DIO_u8_PINC -> ByteAccess, u8PIN_ID);
			break;
		case GROUP_D :
			*Copy_u8PinValue = Get_Bit(DIO_u8_PIND -> ByteAccess, u8PIN_ID);
			break;
		default:

			break;
		}
	}
	/*Function return*/
	return u8ErrorState;
}

/* */
/* */
u8 DIO_u8GetPortValue(u8 Copy_u8PortNB, u8 *Copy_u8PortValue)
{
	/*Local variables*/
	u8 u8ErrorState = 0;

	/*validate inputs*/
	if (Copy_u8PortNB >= DIO_u8_MAXPORTNB)
	{
		u8ErrorState = 1;
	}else
	{
	//		u8ErrorState = 0;

		switch(Copy_u8PortNB)
		{
			case GROUP_A :
				*Copy_u8PortValue = DIO_u8_PINA->ByteAccess;
				break;
			case GROUP_B :
				*Copy_u8PortValue = DIO_u8_PINB->ByteAccess;
				break;
			case GROUP_C :
				*Copy_u8PortValue = DIO_u8_PINC->ByteAccess;
				break;
			case GROUP_D :
				*Copy_u8PortValue = DIO_u8_PIND->ByteAccess;
				break;
			default:

				break;
		}
	}

	/*Function return*/
	return u8ErrorState;
}
