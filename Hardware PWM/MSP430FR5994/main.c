//Hardware PWM on the MSP430FR5994
//Jessica Wozniak
//Created: 10/1/17
//Last updated: 10/11/17

#include <msp430FR5994.h>

int brightness[11] = {0, 52, 104, 156, 208, 260, 312, 364, 416, 468, 511};       //non-log increase. Increase by 10% brightness each time the button is pressed
int i = 0;

#define EXLED BIT4                              //define EXTERNAL LED as BIT4
#define BUTTON1 BIT6                            //defines BUTTON1 as BIT6

int main(void)
{
       WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
//LED Init
        P1DIR |= EXLED;                       //set EXTERNAL LED TO OUTPUT
//Button init
        P5DIR &= ~BUTTON1; //Sets button2, pin1.2 as an input
        P5REN |=  BUTTON1; //Enables the pullup/down resistor
        P5OUT |=  BUTTON1; //Set the resistor to be a pullup resistor

//Timer Init from TI EXAMPLE CODE
        TA0CCR0 = 512-1;                          // PWM Period
        TA0CCTL1 = OUTMOD_7;                      // CCR1 reset/set
        TA0CCR1 = 256;                            // CCR1 PWM duty cycle
        TA0CCTL2 = OUTMOD_7;                      // CCR2 reset/set
        TA0CCR2 = 255;                            // CCR2 PWM duty cycle
        TA0CTL = TASSEL_2 + MC_1 + TACLR;         // ACLK, UP MODE, clear TAR

//Button1 Interrupt Enable
        P5IE |= BUTTON1;                              //enable button interrupt
        P5IES |= BUTTON1;                           //set as falling edge
        P5IFG &= ~BUTTON1;                          //clear flag

        __bis_SR_register(GIE);             // Enter LPM0

 }


   //BUTTON1 ISR
   #pragma vector = PORT5_VECTOR
      __interrupt void Port_5(void)
      {
           TA0CCR1 = brightness[11-i];          //sets duty cycle
           TA0CCR2 = brightness[i+1];          //skips 0, sets duty cycle
           i++;                                 //increment i
           if(i>11)                            //if i is greater than 11, i goes back to equaling 0. (The brightness array only goes to 11) This makes LED brightness loop.
           {
              i=0;
           }

             P5IFG &= ~BUTTON1;                //clears interrupt flag
      }

