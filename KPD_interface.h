/*************************************/
/****** Author 	: Mayar Saber 	******/
/****** SWC		: KEYPAD - AVR	******/
/****** Version : 01		  	******/
/*************************************/

#ifndef		_KPD_INTERFACE_H
#define		_KPD_INTERFACE_H

#define 	KPD_u8_KEY_NOT_PRESSED 		0xFF

u8 KPD_u8GetPressedKey(u8 * Copy_pu8ReturnedKey);			///Function used to return the pressed key from the keypad

#endif
