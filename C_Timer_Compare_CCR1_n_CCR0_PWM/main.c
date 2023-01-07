#include <msp430.h>


/**
 * blink.c
 */
int main(void)
{
     WDTCTL = WDTPW | WDTHOLD;  // stop watchdog timer

     //-- setup ports
     P1DIR |= BIT0;             // set P1.0 to output LED1
     P1OUT |= BIT0;             // set LED1=1 initially
     // PM5CTL0 &= ~LOCKLPM5;     // turn on digital I/O

     //-- setup timer
     TA0CTL = TACLR;             // reset timer A
     TA0CTL |= MC_1;             // put timer A to UP mode. CCR0 has its functionality to reset timer only in up mode
                                 // MC_1 means we set up-mode for CCR0 only.
     TA0CTL |= TASSEL_1;         // choose ACLK (32kHz)
     TA0CCR0 = 32768;            // set PWM period. PWM - Pulse Width Modulation
     TA0CCR1 = 1638;             // set PWM duty cycle

     //-- setup compare IRQs
     TA0CCTL0 |= CCIE;            // Capture/compare interrupt enable. Local enable for CCR0
     TA0CCTL1 |= CCIE;            // Capture/compare interrupt enable. Local enable for CCR1
     __enable_interrupt();       // enable maskable interrupts and only CCR0 and CCR1 can trigger an interrupt
                                 // because we enabled them with TACCTL0 |= CCIE and TACCTL1 |= CCIE
     TA0CCTL0 &= ~CCIFG;          // clear flag for CCR0
     TA0CCTL1 &= ~CCIFG;          // clear flag for CCR1

     //-- main loop
     while(1){}

     return 0;
}
//---------------- ISRs Interrupt Service Routines ----------------//
#pragma vector = TIMER0_A0_VECTOR   // ISR for period
__interrupt void ISR_TA0_CCR0(void)
{
    P1OUT |= BIT0;                  // turn LED1 on
    TA0CCTL0 &= ~CCIFG;              // clear flag for CCR0
}
#pragma vector = TIMER0_A1_VECTOR
__interrupt void ISR_TA0_CCR1(void)
{
    P1OUT &= ~BIT0;                 // turn LED1 off
    TA0CCTL1 &= ~CCIFG;              // clear flag for CCR1
}
