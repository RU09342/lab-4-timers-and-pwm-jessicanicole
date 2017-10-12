//Software PWM MSP430F5529
//Jessica Wozniak
//Created: 10/9/17
//Last updated: 10/11/17

#include <msp430.h>

#define EXLED BIT2          //defines EXTERNAL LEED as BIT2
#define BUTTON1 BIT1        //defines button1 as BIT1

int count=5;                //stars at 50% duty cycle

void main(void)
{
    WDTCTL = WDTPW | WDTHOLD;                   // stop watchdog timer

//LED INIT
    P1DIR |= EXLED;                              //sets LED1 as output
    P1OUT &= ~EXLED;                             //sets output high

//Button INIT
    P2DIR &= ~BUTTON1;                             //Sets button2, pin1.2 as an input
    P2REN |=  BUTTON1;                             //Enables the pullup/down resistor
    P2OUT |=  BUTTON1;                             //Set the resistor to be a pullup resistor

//TIMER INIT
    TA0CTL = TASSEL_2 + MC_1;                   //SMCLK, UP MODE
    TA0CCTL0 = CCIE;                            //Enables CCR0 interrupt
    TA0CCR0 = 1000;                             //Sets CCR0 to 1 kHz
    TA0CCTL1 = CCIE;                            //Enables CCR1 interrupt
    TA0CCR1 = 500;                              //Sets CCR1 to 500 Hz


// BUTTON INTERRUPT ENABLE
    P2IE |= BUTTON1;                                //enable button interrupt
    P2IES |= BUTTON1;                               //set as falling edge
    P2IFG &= ~BUTTON1;                            //clear flag

    __bis_SR_register(LPM0_bits+GIE);               //LPM0 and global interrupt enable
}

//BUTTON1 ISR
#pragma vector=PORT2_VECTOR
__interrupt void Port_2(void)
{
    if(count==10)
    {
        count=0;                            //sets count back to 0
        TA0CCR1=0;                          //sets TA0CCR1 to 0
    }
    else
    {
        count++;                             //increments count
        TA0CCR1 = count*100;                 //Increments CCR1 by 100 Hz
    }
P1OUT ^= EXLED;                             //toggles EXTERNAL LED
P2IFG &=~(BIT1);                            //clears flag
}

//Timer A0 ISR
#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A0 (void)
{
    if(count!=0)                                    //if count does not = 0
        P1OUT |= EXLED;                           //EXTERNAL LED on
}

// TIMER A1 ISR
#pragma vector=TIMER0_A1_VECTOR
__interrupt void Timer_A1 (void)
{
    if(count!=10)                                   //if count does not = 10
        P1OUT &= ~EXLED;                           //EXTERNAL LED off
    TA0IV=0;
}

