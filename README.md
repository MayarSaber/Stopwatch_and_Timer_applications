# Stopwatch_and_Timer_applications
A system that simulates stopwatch and timer applications based on Atmega32 microcontroller
## System Description
This system introduces two different clock applications
- **First:** A stopwatch application that can count up till 100 hours, with the ability of pausing and resuming the stopwatch at any point.
- **Second:** A timer application that can be adjusted to any amount of time consisting of hours, minutes and seconds and by pressing the start button it'll count down till the entered time is up, and that's when a buzzer alarm will be initiated.
## System Components
1. Atmega32 AVR microcontroller with the usage SWC
   - DIO
   - TIMER0 to control the counting of the stopwatch application.
   - TIMER1 to control the counting of the timer application.
   - EXTI0 to control pausing of stopwatch application
   - EXTI2 to control resuming of stopwatch application
2. LCD LMB161A 
3. Kepad consists of 16 tactile switches
4. 2 tactile switches to control EXTI0, EXTI2 interrupts
5. USBASP burner
## Project Pictures
![20230219_072558](https://user-images.githubusercontent.com/94296738/219932800-021a8707-ad0a-4899-b955-5706ada1b2ee.jpg)
![20230219_072348](https://user-images.githubusercontent.com/94296738/219932810-ac2df571-60bc-40e5-b6b1-ccf3b86a0ceb.jpg)
## Operation Video
