/**************************************************************/
/****** Author 	: Mayar Saber 							 ******/
/****** Project	: Stopwatch and timer applications       ******/
/****** SWC		: main.c 			   				     ******/
/****** Version : 01		 							 ******/
/**************************************************************/
/*LIB*/
#include	"BIT_MATH.h"
#include	"STD_TYPES.h"

/*MCAL*/
#include	"DIO_interface.h"
#include	"GI_interface.h"
#include	"TIMERS_interface.h"
#include	"EXTI_interface.h"

/*HAL*/
#include	"LCD_interface.h"
#include	"KPD_interface.h"

#define		F_CPU	8000000UL		        //Macro to define processor frequency in order to use it inside the delay function
#include	<util/delay.h>

/*Functions prototypes*/
void App_voidStopWatch(void);		        //Stopwatch application function
void App_voidTimer(void);			        //Timer application function
void App_voidStartScreen(void);		        //A function to set the starting screen in both applications
void App_voidStopwatchOVFunction(void);     //TIMER0 ISR function, used with stopwatch application to count up time
void App_voidStopTimer0_EXTI(void);			//EXTI0 ISR function, used to pause the stopwatch
void App_voidTurnOnTimer0_EXTI2(void);		//EXTI2 ISR function, used to resume the stopwatch
void App_voidTimerOVFunction(void);			//TIMER0 ISR function, used with timer application to count time down

/*Global Variables*/
u8 App_u8TimeArr[6] = {0,0,0,0,0,0};		//Global array of u8 to store the time entered in Timer application
volatile static u8 App_u8TimerSeconds;		//Global variable of u8 to store the Timer application's seconds
volatile static u8 App_u8TimerMinutes;		//Global variable of u8 to store the Timer application's minutes
volatile static u8 App_u8TimerHours;		//Global variable of u8 to store the Timer application's hours



int main(void)
{
/*--------------------Local Variables--------------------*/
	u8 Local_u8PressedKey = KPD_u8_KEY_NOT_PRESSED;		//Local u8 variable, used to get the pressed key from the keypad

	//Strings used in the welcome screen
	u8 Local_u8LCDString0[] = "....Welcome....";
	u8 Local_u8LCDString1[] = "(1)Stopwatch";
	u8 Local_u8LCDString2[] = "(2)Timer";

/*--------------------Peripherals initializations--------------------*/
	/*initiate DIO*/
	DIO_voidInit();
	/*initiate LCD*/
	LCD_voidInit();
	/*EXTI set Callback*/
	EXTI_u8SetCallBack(EXTI_u8_INTERRUPT0 , &App_voidStopTimer0_EXTI);
	EXTI_u8SetCallBack(EXTI_u8_INTERRUPT2 , &App_voidTurnOnTimer0_EXTI2);
	/*Enable EXTI0*/
	EXTI_u8InterruptEnable(EXTI_u8_INTERRUPT0 , EXTI_u8_FALLING_EDGE);
	EXTI_u8InterruptEnable(EXTI_u8_INTERRUPT2 , EXTI_u8_FALLING_EDGE);
	/*Enable General interrupt*/
	GI_voidEnable();
	/*Setup TIMER0*/
	/*Setup Timer0 in normal mood to overflow every 1sec
	 *Timer freq = 8MHZ / 8 = 1 MHZ
	 *Tick time = 1usec
	 *time of overflow = 256usec
	 *#of overflows to overflow at 1sec = 1*10**6/256 = 3906.25
	 *Overflows = 3907
	 *preload = 256 - 0.25 * 256 = 192*/
	TIMER0_u8SetCallBack_OV(3907 , &App_voidStopwatchOVFunction);		//Number of overflows = 3907
	TIMER0_voidSetPReloadValue(192);									//OCR0 value = 192

	/*Setup TIMER2 for Timer application (OVF every 1sec)*/
	TIMER2_u8SetCallBack_OV(3907 , &App_voidTimerOVFunction);		    //Number of overflows = 3907
	TIMER2_voidSetPReloadValue(192);									//OCR0 value = 192

/*--------------------Welcome Screen--------------------*/
	LCD_voidSendString(Local_u8LCDString0);			//Send welcome message to LCD for 2 seconds
	_delay_ms(2000);
	LCD_voidClearScreen();

/*--------------------Choose Stopwatch/Timer screen--------------------*/
	//Display the applications options on LCD to be chosen from
	LCD_u8GoToXY(LCD_u8_LINE_ONE , 0);
	LCD_voidSendString(Local_u8LCDString1);			//Press '1' to choose stopwatch application
	LCD_u8GoToXY(LCD_u8_LINE_TWO , 0);
	LCD_voidSendString(Local_u8LCDString2);			//Press '0' to choose timer application

	while(1)
	{
		//Pull on KPD value
		KPD_u8GetPressedKey(&Local_u8PressedKey);
		//Decide depending on the input
		if(Local_u8PressedKey != KPD_u8_KEY_NOT_PRESSED)
		{
			if(Local_u8PressedKey == '1')	    	//'1' for stopwatch application
			{
				Local_u8PressedKey = KPD_u8_KEY_NOT_PRESSED;
				App_voidStopWatch();
			}
			else if(Local_u8PressedKey == '2')		//'2' for timer application
			{
				Local_u8PressedKey = KPD_u8_KEY_NOT_PRESSED;
				App_voidTimer();
			}
		}
	}

	return 0;
}


void App_voidStartScreen(void)
/*A function to set the starting screen in both applications*/
{
	//Creating the theme od the watch by placing zeros at specific locations
	LCD_u8GoToXY(LCD_u8_LINE_TWO , 2);
	LCD_voidSendChar('0');
	LCD_voidSendChar('0');
	LCD_u8GoToXY(LCD_u8_LINE_TWO , 5);
	LCD_voidSendChar(':');
	LCD_u8GoToXY(LCD_u8_LINE_TWO , 7);
	LCD_voidSendChar('0');
	LCD_voidSendChar('0');
	LCD_u8GoToXY(LCD_u8_LINE_TWO , 10);
	LCD_voidSendChar(':');
	LCD_u8GoToXY(LCD_u8_LINE_TWO , 12);
	LCD_voidSendChar('0');
	LCD_voidSendChar('0');
}


void App_voidStopWatch(void)
/*Stopwatch application function*/
{
	u8 Local_u8ArrStopwatch[] = "Stopwatch";		//An array of u8 to Write the choice on screen
	LCD_voidClearScreen();							//Clear screen
	LCD_voidSendString(Local_u8ArrStopwatch);		//write the stopwatch keyword on screen
	App_voidStartScreen();							//calling the start screen function to set the format of clock
	TIMER0_voidInit();								//Initiate Timer0 which controls stopwatch application
}


void App_voidTimer(void)
/*Timer application function*/
{
	u8 Local_u8ScreenKey = KPD_u8_KEY_NOT_PRESSED;	//Reset the value of the pressed key
	volatile u8 Local_u8Position = 14;				//The initial location of the cursor

	u8 Local_u8ArrTimer[] = "Timer";				//An array of u8 to Write the choice on screen
	LCD_voidClearScreen();							//Clear screen
	LCD_voidSendString(Local_u8ArrTimer);			//Write the timer keyword on screen
	App_voidStartScreen();							//Calling the start screen function to set the format of clock

	while(Local_u8ScreenKey != 'S')					//While start button isn't pressed...
	{
		while(Local_u8ScreenKey == KPD_u8_KEY_NOT_PRESSED)	//Pull on the value of the pressed key
		{
			KPD_u8GetPressedKey(&Local_u8ScreenKey);
		}

		if(Local_u8ScreenKey == 'R')				//If 'R' is pressed, we move the cursor to right
		{
			LCD_u8_MoveCursor(LCD_u8MoveRight);
			Local_u8ScreenKey = KPD_u8_KEY_NOT_PRESSED; //Reset the value of the pressed key to get the next key
			Local_u8Position++;							//Increase location by 1
		}

		else if(Local_u8ScreenKey == 'L')			//If 'L' is pressed, we move the cursor to left
		{
			LCD_u8_MoveCursor(LCD_u8MoveLeft);
			Local_u8ScreenKey = KPD_u8_KEY_NOT_PRESSED; //Reset the value of the pressed key to get the next key
			Local_u8Position--;							//Decrease location by 1
		}
		else if((Local_u8ScreenKey >= '0') && (Local_u8ScreenKey <= '9'))	//if the pressed key is a number from 0-9
		{
			//Specifying the allowed locations to be used for writing time values
			//Locations 2,3 HOURS / Locations 7,8 MINUTES / Locations 12,13 SECONDS
			if((Local_u8Position == 2)||(Local_u8Position == 3)||(Local_u8Position == 7)||(Local_u8Position == 8)||(Local_u8Position == 12)||(Local_u8Position == 13))
			{
				LCD_voidSendNum(Local_u8ScreenKey - 48);		//Write the pressed number on screen - sub 48 to get int value fron ASCII
				//LCD_voidSendNum(Local_u8Position);

				//Store the entered values for seconds/ minutes and hours in the time array based on the location
				switch(Local_u8Position)
				{
				case 2:												//Hours
					App_u8TimeArr[0] = Local_u8ScreenKey - 48;
					break;

				case 3:
					App_u8TimeArr[1] = Local_u8ScreenKey - 48;
					break;

				case 7:												//Minutes
					App_u8TimeArr[2] = Local_u8ScreenKey - 48;
					break;

				case 8:
					App_u8TimeArr[3] = Local_u8ScreenKey - 48;
					break;

				case 12:											//Seconds
					App_u8TimeArr[4] = Local_u8ScreenKey - 48;
					break;

				case 13:
					App_u8TimeArr[5] = Local_u8ScreenKey - 48;
					break;
				}

				Local_u8Position++;
			}
			Local_u8ScreenKey = KPD_u8_KEY_NOT_PRESSED;

		}
	}
	/*When 'S' is pressed*/
	//Calculate the values of entered seconds, minutes and hours
	App_u8TimerHours   = App_u8TimeArr[0] * 10 + App_u8TimeArr[1];
	App_u8TimerMinutes = App_u8TimeArr[2] * 10 + App_u8TimeArr[3];
	App_u8TimerSeconds = App_u8TimeArr[4] * 10 + App_u8TimeArr[5];


	//If conditions to specify the location of entered values
	//Hours
	if(App_u8TimerHours > 9)
	{
		LCD_u8GoToXY(LCD_u8_LINE_TWO , 2);
		LCD_voidSendNum(App_u8TimerHours);
	}
	else
	{
		LCD_u8GoToXY(LCD_u8_LINE_TWO , 3);
		LCD_voidSendNum(App_u8TimerHours);
	}

	//-----
	//Minutes
	if(App_u8TimerMinutes > 9)
	{
		LCD_u8GoToXY(LCD_u8_LINE_TWO , 7);
		LCD_voidSendNum(App_u8TimerMinutes);
	}
	else
	{
		LCD_u8GoToXY(LCD_u8_LINE_TWO , 8);
		LCD_voidSendNum(App_u8TimerMinutes);
	}

	//-----
	//Seconds
	if(App_u8TimerSeconds > 9)
	{
		LCD_u8GoToXY(LCD_u8_LINE_TWO , 12);
		LCD_voidSendNum(App_u8TimerSeconds);
	}
	else
	{
		LCD_u8GoToXY(LCD_u8_LINE_TWO , 13);
		LCD_voidSendNum(App_u8TimerSeconds);
	}

	u8 Local_u8StartArr[] = "Start..";					//An array of u8 to inform the start of timer
	u8 Local_u8Clear[] = "       ";

	TIMER0_voidEnableTIMER0();							//Initiate Timer1 which controls timer application

	LCD_u8GoToXY(LCD_u8_LINE_ONE , 9);					//Send "Start" keyword to screen
	LCD_voidSendString(Local_u8StartArr);

	_delay_ms(500);										//Delay for 500msec
	LCD_u8GoToXY(LCD_u8_LINE_ONE , 9);
	LCD_voidSendString(Local_u8Clear);					//Erase "Start" keyword
	TIMER2_voidInit();
}


void App_voidStopwatchOVFunction(void)
/*TIMER0 ISR function, used with stopwatch application to count up time*/
{
	static u8 Local_u8Counter = 0;			//A local variable of u8 to control the timing for seconds
	static u8 Local_u8MinCounter = 0;		//A local variable of u8 to control the timing for minutes
	static u8 Local_u8HourCounter = 0;		//A local variable of u8 to control the timing for hours

	Local_u8Counter++;						//Increase seconds counter with each overflow which happens every 1sec

	/*If condition to check the location of the seconds count on the screen format*/
	if(Local_u8Counter < 10)
	{
		LCD_u8GoToXY(LCD_u8_LINE_TWO , 13);
		LCD_voidSendNum(Local_u8Counter);
	}
	else if((Local_u8Counter > 9) && (Local_u8Counter < 60))
	{
		LCD_u8GoToXY(LCD_u8_LINE_TWO , 12);
		LCD_voidSendNum(Local_u8Counter);
	}
	else									//If the number of seconds exceeded 60sec, the screen will move to minutes field
	{
		Local_u8MinCounter++;				//Increase minutes counter each time this condition is achieved
		Local_u8Counter = 0;				//Reset the counter of seconds again

		LCD_u8GoToXY(LCD_u8_LINE_TWO , 12); //Write 0s in seconds fields when this conditions is achieved
		LCD_voidSendNum(0);
		LCD_u8GoToXY(LCD_u8_LINE_TWO , 13);
		LCD_voidSendNum(0);

		/*If condition to check the location of the minutes count on the screen format*/
		if(Local_u8MinCounter < 10)
		{
			LCD_u8GoToXY(LCD_u8_LINE_TWO , 8);
			LCD_voidSendNum(Local_u8MinCounter);
		}
		else if((Local_u8MinCounter > 9) && (Local_u8MinCounter < 60))
		{
			LCD_u8GoToXY(LCD_u8_LINE_TWO , 7);
			LCD_voidSendNum(Local_u8MinCounter);
		}
		else								//If the number of minutes exceeded 60min, the screen will move to hours field
		{
			Local_u8HourCounter++;			//Increase hours counter each time this condition is achieved
			Local_u8MinCounter = 0;			//Reset the counter of minutes again

			LCD_u8GoToXY(LCD_u8_LINE_TWO , 7);	//Write 0s in minutes fields when this conditions is achieved
			LCD_voidSendNum(0);
			LCD_u8GoToXY(LCD_u8_LINE_TWO , 8);
			LCD_voidSendNum(0);

			/*If condition to check the location of the hours count on the screen format*/
			if(Local_u8HourCounter < 10)
			{
				LCD_u8GoToXY(LCD_u8_LINE_TWO , 3);
				LCD_voidSendNum(Local_u8HourCounter);
			}
			else if((Local_u8HourCounter > 9) && (Local_u8HourCounter < 100))
			{
				LCD_u8GoToXY(LCD_u8_LINE_TWO , 2);
				LCD_voidSendNum(Local_u8HourCounter);
			}
		}
	}
}


void App_voidStopTimer0_EXTI(void)
/*EXTI0 ISR function, used to pause the stopwatch*/
{
	u8 Local_u8PauseArr[] = "Pause";			//An array of u8 to hold a "Pause" message
	TIMER0_voidStopTIMER0();					//Stop timer0 interrupt

	LCD_u8GoToXY(LCD_u8_LINE_ONE , 11);			//send "Pause" message
	LCD_voidSendString(Local_u8PauseArr);
}


void App_voidTurnOnTimer0_EXTI2(void)
/*EXTI2 ISR function, used to resume the stopwatch*/
{
	u8 Local_u8PauseArr[] = "Resume";			//An array of u8 to hold a "Resume" message
	u8 Local_u8Clear[] = "      ";

	TIMER0_voidEnableTIMER0();					//Enable timer0 interrupt to resume counting

	LCD_u8GoToXY(LCD_u8_LINE_ONE , 10);			//Send "Resume" message
	LCD_voidSendString(Local_u8PauseArr);

	_delay_ms(700);								//Clear "Resume" message after 700msec
	LCD_u8GoToXY(LCD_u8_LINE_ONE , 10);
	LCD_voidSendString(Local_u8Clear);
}


void App_voidTimerOVFunction(void)
/*TIMER0 ISR function, used with timer application to count time down*/
{
	//Counting down after 's' button is pressed

	if(App_u8TimerSeconds != 0)		//Seconds
	{
		App_u8TimerSeconds--;		//Decrese seconds count

		//If condition to specify the location of the new seconds count
		if(App_u8TimerSeconds > 9)
		{
			LCD_u8GoToXY(LCD_u8_LINE_TWO , 12);
			LCD_voidSendNum(App_u8TimerSeconds);
		}

		else
		{
			LCD_u8GoToXY(LCD_u8_LINE_TWO , 12);
			LCD_voidSendNum(0);
			LCD_u8GoToXY(LCD_u8_LINE_TWO , 13);
			LCD_voidSendNum(App_u8TimerSeconds);
		}
	}

	else
	{
		if(App_u8TimerMinutes != 0)		//Minutes
		{
			App_u8TimerMinutes--;		//Decrese minutes count
			App_u8TimerSeconds = 60;	//Reset seconds count

			//If condition to specify the location of the new minutes count
			if(App_u8TimerMinutes > 9)
			{
				LCD_u8GoToXY(LCD_u8_LINE_TWO , 7);
				LCD_voidSendNum(App_u8TimerMinutes);
			}

			else
			{
				LCD_u8GoToXY(LCD_u8_LINE_TWO , 7);
				LCD_voidSendNum(0);
				LCD_u8GoToXY(LCD_u8_LINE_TWO , 8);
				LCD_voidSendNum(App_u8TimerMinutes);
			}
		}
		else
		{
			if(App_u8TimerHours != 0)		//Hours
			{
				App_u8TimerHours--;			//Decrese hours count
				App_u8TimerMinutes = 60;	//Reset minutes count

				//If condition to specify the location of the new hours count
				if(App_u8TimerHours > 9)
				{
					LCD_u8GoToXY(LCD_u8_LINE_TWO , 2);
					LCD_voidSendNum(App_u8TimerHours);
				}

				else
				{
					LCD_u8GoToXY(LCD_u8_LINE_TWO , 2);
					LCD_voidSendNum(0);
					LCD_u8GoToXY(LCD_u8_LINE_TWO , 3);
					LCD_voidSendNum(App_u8TimerHours);
				}
			}
		}
	}

	//Sending a Time's up alert at the end of timer's counting down process
	if((App_u8TimerHours == 0)&&(App_u8TimerMinutes == 0)&&(App_u8TimerSeconds == 0))
	{
		u8 Local_u8TimerArr[] = "Time's Up!";			//An array of u8 to hold "Time's up" message
		LCD_voidClearScreen();
		LCD_voidSendString(Local_u8TimerArr);

		//Set buzzer alarm for 1sec at the end of counting down operation
		DIO_u8SetPinValue(DIO_u8_PORTB , DIO_u8_PIN0 , DIO_u8_HIGH);
		_delay_ms(1000);
		DIO_u8SetPinValue(DIO_u8_PORTB , DIO_u8_PIN0 , DIO_u8_LOW);
		TIMER2_voidStopTIMER2();
	}
}
