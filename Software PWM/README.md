# Software PWM
* Creator: Jessica Wozniak
* Created: October 9, 2017
* Last updated: October 11, 2017

## Libraries Used
* <msp430.h>

## Compatability:
* MSP430F5529
* MSP430FR2311
* MSP430FR5994
* MSP430FR6989
* MSP430G2553

## Differences between Boards
* All MSP430FR need this line of code " PM5CTL0 &= ~LOCKLPM5; " to disable high impedance mode.
* External LED Bits
** MSP430F5529: P1.2
** MSP430FR2311: P1.2
** MSP430FR5994: Used LED2 (P1.1)
** MSP430FR6989: Used LED2 (P9.7)
** MSP430G2553: P1.7

* BUTTON1 pins
** MSP430F5529: P2.1
** MSP430FR2311: P1.1
** MSP430FR5994: P5.6
** MSP430FR6989: P1.1
** MSP430G2553: P1.3

## Breadboard Configuration 
For both PWM programs I personally found it easier to see the change in brightness if the LED was an External LED. I used a green LED and 1k resister on a non- solder breadboard. The resistor was between the cathode of the LED and ground, P1.2 was connected to anode of LED (Port I selected to be output pin). 

## Overall Program Description
The software PWM was made using interrupts and timers. This program allows a user to change the brightness of an LED with the click of BUTTON1. The variable 'brightness' was declared as an array. The numbers in the array are hardcoded and will not change unless developer changes the code. After the initialization of LEDS, BUTTONS, and TIMERS, the code uses three interrupts: BUTTON1, TIMER_A0, and TIMER_A1. 
* int brightness[11] = {0, 52, 104, 156, 208, 260, 312, 364, 416, 468, 511};       
* int i = 5;    

Initilizing i to 5 allows the duty cycle to start at 50%. Within the interrupt, TA0CCR1 was set equal to brightness[11-i] and TA0CCR2 was set equal to brightness[i+1]. This allowed the duty cycle to be set based on 'i'. Then "i" was incremented, and if i < 11 then the interrupt flag was cleared. IN TIMER_A0 intterupt if i did not equal 0, the External LED was on. And in TIMER_A1 interrupt, if i > 11 then the External LED was off.

## Extra Work
### Linear Brightness
Humans perceive brightness in a logarithmic manor. In the main Software PWM, the brightness changes by 10% every time BUTTON1 is pressed (for a period of 511, each 10% step is about 52). In the log software PWM program, each time BUTTON1 is pressed, the brightness increases logarithmically. A 'brightness[10]' array was declared as a global variable and was set to values from 0 to 511 that followed a logarithic pattern. 

* int brightness[10] = {0, 10, 20, 50, 75, 100, 150, 250, 400, 511};

* Performed on MSP430F5529
