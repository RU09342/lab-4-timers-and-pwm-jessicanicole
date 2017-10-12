//Debounce on MSP430FR2311
//Jessica Wozniak
//Created: 10/9/17
//Last updated: 10/11/17

#include "msp430FR2311.h"

#define LED1 BIT0
#define LED2 BIT0
#define BUTTON BIT1

int temp = 0;

void main(void)
{
WDTCTL = WDTPW + WDTHOLD; // Stop WDT

P1DIR |= LED1;                       // Sets LED1 to output
P2DIR |= LED2;                       // Sets LED2 to output

P1OUT &= ~LED1;                      //Clears LED bit
P2OUT &= ~LED2;                      //Clears LED bit

P1DIR &= ~BUTTON;                    //Sets BUTTON1 as input
P1REN |= BUTTON;                     //Enables a puller-Resistor on the button-pin
P1OUT |= BUTTON;                     //Writes a "1" to the portpin, telling the resistor to pullup

P1IE |= BUTTON;                      //BUTTON interrupt enabled
P1IES |= BUTTON;                     //High/ Low edge trigger
P1IFG &= ~BUTTON;                    //Flag cleared

TB0CCTL0 = CCIE;                     // CCR0 interrupt enabled
TB0CTL = TBSSEL_2 + MC_0 + ID_3;     // SMCLK, STOP, DIV 8
TB0CCR0 = 500;

PM5CTL0 &= ~LOCKLPM5;

__bis_SR_register(GIE);       // Enter LPM0 w/ interrupt

}


//Timer A_0 ISR
#pragma vector=TIMER0_B0_VECTOR
__interrupt void Timer_B0 (void)
{
    TB0CTL = TBSSEL_2 + MC_0 + ID_3; // SMCLK, stop, /8
    P1IE |= BUTTON;                    //BUTTON interrupt enable
}

//Button ISR
#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{
    P1IFG &= ~BUTTON;                     //clears flag
    P1IE &= ~BUTTON;                      //button interrupt enabled

    TB0CTL = TBSSEL_2 + MC_1 + ID_3;      //SMCLK, UP MODE, DIV 8

    if (temp == 0)
          {
            P1OUT |= LED1;              //LED1 on
            P2OUT &= ~LED2;             //LED2 off
            temp  = 1;
            P1IES &= ~BUTTON;           //flip edge of interrupt
          }
    else
          {
            P1OUT &= ~LED1;             //LED1 off
            P2OUT |= LED2;              //LED2 on
            temp  = 0;
            P1IES |= BUTTON;            //flip edge of interrupt
          }
}
