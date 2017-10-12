# Hardware PWM
* Creator: Jessica Wozniak
* Created: October 8, 2017
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
** MSP430FR2311: P1.1
** MSP430FR5994: P1.4
** MSP430FR6989: used LED2 (P9.7)
** MSP430G2553: used LED2 (P1.6)

* BUTTON1 pins
** MSP430F5529: P2.1
** MSP430FR2311: P1.1
** MSP430FR5994: P5.6
** MSP430FR6989: P1.1
** MSP430G2553: P1.3

## Breadboard Configuration 
For both PWM programs I personally found it easier to see the change in brightness if the LED was an External LED. I used a green LED and 1k resister on a non- solder breadboard. The resistor was between the cathode of the LED and ground, P1.2 was connected to anode of LED (Port I selected to be output pin). 

## Overall Program Description
The Hardware PWM Program was made to . After the LED, BUTTON, TIMER initilization, the code only consisted of one interrupt on TIMER A0. As talked about in the software PWM readme, a global variable "brightness" was declared as an array and an int "i" was declared to keep track of position in the array

* int brightness[11] = {0, 51, 102, 153, 204, 255, 306, 357, 408, 459, 511}; 
* int i;

Within the interrupt, TA0CCR1 was set equal to brightness[11-i] and TA0CCR2 was set equal to brightness[i+1]. This allowed the duty cycle to be set based on 'i'. For example: if i = 3 : TACCR1 = brightness[8] = 357, and TACCR0 = 153. Then "i" was incremented, and if i < 11 then the interrupt flag was cleared. If i > 11, then i was reset to 0.

## Extra Work
### Using ACLK
Using ACLK on MSP430F5529 the code seemed to still run fine. Maybe even better.