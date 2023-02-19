/***********************************/
/****** Author 	: Mayar Saber ******/
/****** SWC		: DIO - GI    ******/
/****** Version : 01		  ******/
/***********************************/
/*LIB*/
#include "STD_TYPES.h"
#include "BIT_MATH.h"

/*MCAL*/
#include "GI_interface.h"
#include "GI_private.h"

void GI_voidEnable(void)
{
	SET_BIT(SREG , 7);
}

void GI_voidDisable(void)
{
	CLR_BIT(SREG , 7);
}
