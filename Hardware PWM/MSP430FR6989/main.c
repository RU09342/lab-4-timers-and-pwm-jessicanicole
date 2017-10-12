//Hardware PWM on the MSP430FR6989
//Jessica Wozniak
//Created: 10/1/17
//Last updated: 10/11/17

#include <msp430fr6989.h>

int brightness[11] = {0, 52, 104, 156, 208, 260, 312, 364, 416, 468, 511};       //non-log increase. Increase by 10% brightness each time the button is pressed
int i = 0;

#define EXLED BIT7                              //define EXTERNAL LED as BIT7
#define BUTTON1 BIT1                            //defines BUTTON1 as BIT1

int main(void)
{
       WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
//LED Init
        P9DIR |= EXLED;                       //set EXTERNAL LED TO OUTPUT

//Button init
        P1DIR &= ~BUTTON1; //Sets button2, pin1.2 as an input
        P1REN |=  BUTTON1; //Enables the pullup/down resistor
        P1OUT |=  BUTTON1; //Set the resistor to be a pullup resistor

//Timer Init from TI EXAMPLE CODE
        TA0CCR0 = 512-1;                          // PWM Period
        TA0CCTL1 = OUTMOD_7;                      // CCR1 reset/set
        TA0CCR1 = 256;                            // CCR1 PWM duty cycle
        TA0CCTL2 = OUTMOD_7;                      // CCR2 reset/set
        TA0CCR2 = 255;                            // CCR2 PWM duty cycle
        TA0CTL = TASSEL_2 + MC_1 + TACLR;         // ACLK, UP MODE, clear TAR

//Button1 Interrupt Enable
        P1IE |= BUTTON1;                              //enable button interrupt
        P1IES |= BUTTON1;                           //set as falling edge
        P1IFG &= ~BUTTON1;                          //clear flag

        PM5CTL0 &= ~LOCKLPM5;

        __bis_SR_register(LPM0_bits + GIE);             // Enter LPM0

 }


   //BUTTON1 ISR
   #pragma vector = PORT1_VECTOR
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


