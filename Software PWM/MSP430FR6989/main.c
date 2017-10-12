//Software PWM MSP430FR6989
//Jessica Wozniak
//Created: 10/9/17
//Last updated: 10/11/17

#include <msp430.h>


int brightness[11] = {0, 52, 104, 156, 208, 260, 312, 364, 416, 468, 511};       //non-log increase. Increase by 10% brightness each time the button is pressed
int i = 5;                 //starts at 50% duty cycle

#define EXLED BIT3          //defines EXTERNAL LED as BIT3
#define BUTTON1 BIT1        //defines button1 as BIT1

void main(void)
{
    WDTCTL = WDTPW | WDTHOLD;                   // stop watchdog timer

//LED INIT
    P1DIR |= EXLED;                              //sets LED1 as output
    P1OUT &= ~EXLED;                             //sets output high

//Button INIT
    P1DIR &= ~BUTTON1;                             //Sets BUTTON1 as input
    P1REN |=  BUTTON1;                             //Enables the pullup resistor
    P1OUT |=  BUTTON1;                             //Set the resistor to be a pullup resistor

//TIMER INIT
    TA0CTL = TASSEL_2 + MC_1;                   //SMCLK, UP MODE
    TA0CCTL0 = CCIE;                            //Enables CCR0 interrupt
    TA0CCR0 = 1000;                             //Sets CCR0 to 1 kHz
    TA0CCTL1 = CCIE;                            //Enables CCR1 interrupt
    TA0CCR1 = 500;                              //Sets CCR1 to 500 Hz


// BUTTON INTERRUPT ENABLE
    P1IE |= BUTTON1;                                //enable button interrupt
    P1IES |= BUTTON1;                               //set as falling edge
    P1IFG &= ~BUTTON1;                            //clear flag

    __bis_SR_register(LPM0_bits+GIE);               //LPM0 and global interrupt enable

    PM5CTL0 &= ~LOCKLPM5;

}

//BUTTON1 ISR
#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{
    TA0CCR1 = brightness[11-i];          //sets duty cycle
    TA0CCR2 = brightness[i+1];          //skips 0, sets duty cycle
    i++;                                 //increment i
    if(i>11)                            //if i is greater than 11, i goes back to equaling 0. (The brightness array only goes to 11) This makes LED brightness loop.
    {
       i=0;
    }

      P1IFG &= ~BUTTON1;                //clears interrupt flag
}

//Timer A0 ISR
#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A0 (void)
{
    if(i!=0)
        P1OUT |= EXLED;                           //EXTERNAL LED on
}

// TIMER A1 ISR
#pragma vector=TIMER0_A1_VECTOR
__interrupt void Timer_A1 (void)
{
    if(i!=11)
        P1OUT &= ~EXLED;                           //EXTERNAL LED off
    TA0IV=0;
}

