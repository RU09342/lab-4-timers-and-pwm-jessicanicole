# Debouncing
* Creator: Jessica Wozniak
* Created: October 5, 2017
* Last updated: October 8, 2017

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
* BUTTON1 pins
** MSP430F5529: P2.1
** MSP430FR2311: P1.1
** MSP430FR5994: P5.6
** MSP430FR6989: P1.1
** MSP430G2553: P1.3

## Program Description
The Debounce Program was made to correct the unwanted behavior of the mechanical switch (button). The program first initilized all pins used (LEDS, BUTTONS) and Timers. A total of two interrupts were used for BUTTON1 and TIMER A0. Within the BUTTON1 interrupt, first the interrupt flag was cleared and the interrupt was enabled. Timer A0 starts to count using SMCLK in UPMODE (Timer A0 was initially in hault mode). An if else statement was then used to see if the variable temp was 0 or 1. If it was 0 LED1 turns on, temp was set to 1, and the edge of the interrupt was flipped, and else LED1 was off, temp was set to 0, and the edge of the interrupt was flipped. In the Timer A0 interrupt, Timer A0 was set to SMCLK, STOP and BUTTON1 interrupt was enabled. 

## Extra Work
### Double the fun
Two buttons were bedounced using the same timer peripheral. The button interrupt descriped above was duplicated for the second button. A differnt LED turned on and off (opposite of the first: on when unpressed and off when pressed).

* Performed on MSP430F5529
