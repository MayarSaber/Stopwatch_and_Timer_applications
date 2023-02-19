/***********************************/
/****** Author 	: Mayar Saber ******/
/****** SWC		: Timer - AVR ******/
/****** Version : 01		  ******/
/***********************************/

#ifndef		_TIMERS_INTERFACE_H
#define		_TIMERS_INTERFACE_H

/**********************************************| TIMER0 |***********************************************/
u8 	 TIMER0_u8SetCallBack_OV		   (u16 Copy_u16CounterValue , void (*Copy_pf)(void));
u8 	 TIMER0_u8SetCallBack_CTC		   (u16 Copy_u16CounterValue , void (*Copy_pf)(void));
void TIMER0_voidSetCompareMatchValue   (u8 Copy_u8OCR0Value);
void TIMER0_voidSetPReloadValue		   (u8 Copy_u8PreloadValue);
u8   TIMER0_voidInit                   (void);
u8   TIMER0_u8GetCounterValue		   (u8 * Copy_pu8CounterValue);
void TIMER0_voidStopTIMER0			   (void);
void TIMER0_voidEnableTIMER0           (void);

/**********************************************| TIMER1 |***********************************************/
u8   TIMER1_u8SetCallBack_OV		   (u32 Copy_u32CounterValue , void (*Copy_pf)(void));
u8   TIMER1_u8SetCallBack_CTC_A		   (u32 Copy_u32CounterValue , void (*Copy_pf)(void));
u8   TIMER1_u8SetCallBack_CTC_B		   (u32 Copy_u32CounterValue , void (*Copy_pf)(void));
void TIMER1_voidSetCompareMatchValue_A (u16 Copy_u16OCR1AValue);
void TIMER1_voidSetCompareMatchValue_B (u16 Copy_u16OCR1BValue);
void TIMER1_voidSetPReloadValue		   (u16 Copy_u16PreloadValue);
void TIMER1_voidSetInputCaptureValue   (u16 Copy_u16InputCaptureValue);
u8   TIMER1_voidInit                   (void);
u8   TIMER1_u8GetCounterValue		   (u16 * Copy_pu16CounterValue);
void TIMER1_voidHW_ICUInit			   (void);												     //Used to Get TIMER1 to work, and to choose the working edge
u8   TIMER1_u8GetPWMOnPeriod		   (u16 * Copy_pu16OnTime);
u8   TIMER1_u8GetPWMTotalPeriod		   (u32 * Copy_pu32TotalTime);
u8	 TIMER1_u8GerDutyCycle			   (u8 * Copy_pu8DutyCycle);

/**********************************************| TIMER2 |***********************************************/
u8 	 TIMER2_u8SetCallBack_OV		   (u16 Copy_u16CounterValue , void (*Copy_pf)(void));
u8 	 TIMER2_u8SetCallBack_CTC		   (u16 Copy_u16CounterValue , void (*Copy_pf)(void));
void TIMER2_voidSetCompareMatchValue   (u8 Copy_u8OCR2Value);
void TIMER2_voidSetPReloadValue		   (u8 Copy_u8PreloadValue);
u8   TIMER2_voidInit                   (void);
u8   TIMER2_u8GetCounterValue		   (u8 * Copy_pu8CounterValue);
void TIMER2_voidStopTIMER2			   (void);
void TIMER2_voidEnableTIMER2           (void);

#endif
