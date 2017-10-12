//Hardware PWM on the MSP430FR2311
//Jessica Wozniak
//Created: 10/1/17
//Last updated: 10/11/17

#include <msp430FR2311.h>

int brightness[11] = {0, 52, 104, 156, 208, 260, 312, 364, 416, 468, 511};       //non-log increase. Increase by 10% brightness each time the button is pressed
int i = 0;

#define EXLED BIT1
#define BUTTON1 BIT1

int main(void)
{
       WDTCTL = WDTPW + WDTHOLD;             // Stop WDT

//External LED Init
        P1DIR |= EXLED;                       //set EXTERNAL LED TO OUTPUT

//Button Init
        P1DIR &= ~BUTTON1; //Sets button2, pin1.2 as an input
        P1REN |=  BUTTON1; //Enables the pullup/down resistor
        P1OUT |=  BUTTON1; //Set the resistor to be a pullup resistor

//Timer Init
        TB0CCR0 = 512-1;                          // PWM Period
        TB0CCTL1 = OUTMOD_7;                      // CCR1 reset/set
        TB0CCR1 = 256;                            // CCR1 PWM duty cycle
        TB0CCTL2 = OUTMOD_7;                      // CCR2 reset/set
        TB0CCR2 = 255;                            // CCR2 PWM duty cycle
        TB0CTL = TBSSEL_1 + MC_1 + TBCLR;         // ACLK, UP MODE, clear TBR

//BUTTON1 Interrupt enable
        P1IE |=BUTTON1;                           //Button1 interrupt enable
        P1IES |=BUTTON1;                          //set as falling edge
        P1IFG &= ~BUTTON1;                        //clear interrupt flag

        __bis_SR_register(LPM0_bits + GIE);             // Enter LPM0 and enable global interrupt
 }

//BUTTON1 ISR
#pragma vector = PORT1_VECTOR
   __interrupt void Port_1(void)
{
        TB0CCR1 = brightness[11-i];
        TB0CCR2 = brightness[i];
        i++;
        if(i>11)
        {
           i=0;
        }

          P1IFG &= ~BUTTON1;
}


