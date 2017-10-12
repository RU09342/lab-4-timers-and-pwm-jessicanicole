//Debounce on MSP430G2553
//Jessica Wozniak
//Created: 10/9/17
//Last updated: 10/11/17

#include "msp430G2553.h"

#define LED1 BIT0
#define LED2 BIT6
#define BUTTON BIT3

int temp = 0;

void main(void)
{
WDTCTL = WDTPW + WDTHOLD; // Stop WDT

P1DIR |= LED1;                       // Sets LED1 to output
P1DIR |= LED2;                       // Sets LED2 to output

P1OUT &= ~LED1;                      //Clears LED bit
P1OUT &= ~LED2;                      //Clears LED bit

P1DIR &= ~BUTTON;                    //Sets BUTTON1 as input
P1REN |= BUTTON;                     //Enables a puller-Resistor on the button-pin
P1OUT |= BUTTON;                     //Writes a "1" to the portpin, telling the resistor to pullup

P1IE |= BUTTON;                      //BUTTON interrupt enabled
P1IES |= BUTTON;                     //High/ Low edge trigger
P1IFG &= ~BUTTON;                    //Flag cleared

TA0CCTL0 = CCIE;                     // CCR0 interrupt enabled
TA0CTL = TASSEL_2 + MC_0 + ID_3;     // SMCLK, STOP, DIV 8
TA0CCR0 = 500;

__bis_SR_register(GIE);       // Enter LPM0 w/ interrupt

}


//Timer A_0 ISR
#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A (void)
{
    TA0CTL = TASSEL_2 + MC_0 + ID_3; // SMCLK, STOP, DIV 8
    P1IE |= BUTTON;                    //BUTTON interrupt enable
}

//Button ISR
#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{
    P1IFG &= ~BUTTON;                     //clears flag
    P1IE &= ~BUTTON;                      //button interrupt enabled

    TA0CTL = TASSEL_2 + MC_1 + ID_3;      //SMCLK, UP MODE, DIV 8

    if (temp == 0)
          {
            P1OUT |= LED1;              //LED1 on
            P1OUT &= ~LED2;             //LED2 off
            temp  = 1;
            P1IES &= ~BUTTON;           //flip edge of interrupt
          }
    else
          {
            P1OUT &= ~LED1;             //LED1 off
            P1OUT |= LED2;              //LED2 on
            temp  = 0;
            P1IES |= BUTTON;            //flip edge of interrupt
          }
}
