//Debounce on MSP430F5529
//Jessica Wozniak
//Created: 10/1/17
//Last updated: 10/10/17

#include "msp430F5529.h"

#define LED1 BIT0
#define LED2 BIT7
#define BUTTON BIT1

int temp = 0;

void main(void)
{
WDTCTL = WDTPW + WDTHOLD; // Stop WDT

P1DIR |= LED1;                       // Sets LED1 to output
P4DIR |= LED2;                       // Sets LED2 to output

P1OUT &= ~LED1;                      //Clears LED bit
P4OUT &= ~LED2;                      //Clears LED bit

P2DIR &= ~BUTTON;                    //Sets BUTTON1 as input
P2REN |= BUTTON;                     //Enables a puller-Resistor on the button-pin
P2OUT |= BUTTON;                     //Writes a "1" to the portpin, telling the resistor to pullup

P2IE |= BUTTON;                      //BUTTON interrupt enabled
P2IES |= BUTTON;                     //falling edge
P2IFG &= ~BUTTON;                    //Flag cleared

TA0CCTL0 = CCIE;                     // CCR0 interrupt enabled
TA0CTL = TASSEL_2 + MC_0 + ID_3;     // SMCLK, STOP, DIV 8
TA0CCR0 = 500;

__bis_SR_register(GIE);       // global interrupt enable

}


//Timer A_0 ISR
#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A (void)
{
    TA0CTL = TASSEL_2 + MC_0 + ID_3;     // SMCLK, STOP , DIV 8
    P2IE |= BUTTON;                      //BUTTON interrupt enable
}

//Button ISR
#pragma vector=PORT2_VECTOR
__interrupt void Port_2(void)
{

    P2IFG &= ~BUTTON;                     //clears flag
    P2IE &= ~BUTTON;                      //button interrupt enabled

    TA0CTL = TASSEL_2 + MC_1 + ID_3;      //SMCLK, UP MODE, DIV 8

    if (temp == 0)
          {
            P1OUT |= LED1;              //LED1 on
            P4OUT &= ~LED2;             //LED2 off
            temp  = 1;
            P2IES &= ~BUTTON;           //flip edge of interrupt
          }
    else
          {
            P1OUT &= ~LED1;             //LED1 off
            P4OUT |= LED2;              //LED2 on
            temp  = 0;
            P2IES |= BUTTON;            //flip edge of interrupt
          }
}
