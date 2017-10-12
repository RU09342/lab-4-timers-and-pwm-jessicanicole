//Button Based Delay on the MSP430G2553
//Jessica Wozniak
//Created: 10/1/17
//Last updated: 10/5/17

#include "msp430F5529.h"

#define LED1 BIT0
#define LED2 BIT7
#define BUTTON1 BIT1
#define BUTTON2 BIT1

int temp = 0;

void main(void)
{
WDTCTL = WDTPW + WDTHOLD; // Stop WDT

//LED
P1DIR |= LED1;                       // Sets LED1 to output
P4DIR |= LED2;                       // Sets LED2 to output

P1OUT &= ~LED1;                      //Clears LED1 bit
P4OUT &= ~LED2;                      //Clears LED2 bit

//Button1
P2DIR &= ~BUTTON1;                    //Sets BUTTON1 as input
P2REN |= BUTTON1;                     //Enables a puller-Resistor on the button-pin
P2OUT |= BUTTON1;                     //Writes a "1" to the portpin, telling the resistor to pullup

P2IE |= BUTTON1;                      //BUTTON1 interrupt enabled
P2IES |= BUTTON1;                     //Set as rising/ falling edge
P2IFG &= ~BUTTON1;                    //Flag cleared

//Button2
P1DIR &= ~BUTTON2;                    //Sets BUTTON2 as input
P1REN |= BUTTON2;                     //Enables a puller-Resistor on the button-pin
P1OUT |= BUTTON2;                     //Writes a "1" to the portpin, telling the resistor to pullup

P1IE |= BUTTON2;                      //BUTTON2 interrupt enabled
P1IES |= BUTTON2;                     //Set as rising/ falling edge
P1IFG &= ~BUTTON2;                    //Flag cleared

TA0CCTL0 = CCIE;                      // CCR0 interrupt enabled
TA0CTL = TASSEL_2 + MC_0 + ID_3;      // SMCLK, STOP, DIV 8
TA0CCR0 = 500;

__bis_SR_register(LPM0_bits + GIE);   //enter LPM0 mode and enable global interrupt

}


//Timer A_0 ISR
#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A (void)
{
    TA0CTL = TASSEL_2 + MC_0 + ID_3;    //SMCLK, STOP, DIV 8
    P1IE |= BUTTON2;                    //BUTTON2 interrupt enable
    P2IE |= BUTTON1;                    //BUTTON1 interrupt enable
}

//BUTTON1 ISR
#pragma vector=PORT2_VECTOR
__interrupt void Port_2(void)
{

    P2IFG &= ~BUTTON1;                  //Clears flag
    P2IE &= ~BUTTON1;                   //BUTTON1 interrupt enable

    TA0CTL = TASSEL_2 + MC_1 + ID_3;    //SMCLK, UP MODE, DIV 8

    if (temp == 0)
          {
            P1OUT |= LED1;              //LED1 on
            temp  = 1;
            P2IES &= ~BUTTON1;          //Flips edge
          }
    else
          {
            P1OUT &= ~LED1;             //LED1 off
            temp  = 0;
            P2IES |= BUTTON1;           //Flips edge
          }
}
//BUTTON2 ISR
#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{

    P1IFG &= ~BUTTON2;                  //Clears Flag
    P1IE &= ~BUTTON2;                   //Enable BUTTON2 interrupt

    TA0CTL = TASSEL_2 + MC_1 + ID_3;    //SMCLK, UP MODE, DIV 8

    if (temp == 0)
          {
            P4OUT &= ~LED2;             //LED2 off
            temp  = 1;
            P1IES &= ~BUTTON2;          //Flips edge
          }
    else
    {
            P4OUT |= LED2;              //LED2 on
            temp  = 0;
            P1IES |= BUTTON2;           //Flips edge
          }
}

