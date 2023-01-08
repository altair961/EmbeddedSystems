#include <msp430.h>

int pw[] = {500,500,500,500,500,500,500,500};

void closeClaw()
{
    //-- setup ports
    //P1DIR |= BIT0;              // set P1.0 to output LED1
    P1DIR = 0b00111111; // Make P1.0-5 outputs
    P1OUT |= BIT0;              // set LED1=1 initially
    TA0CCR0 = 20000;            // set PWM period. PWM - Pulse Width Modulation
    // set PWM duty cycle
    TA0CCR1 = 900;              // almost full close claw
    __delay_cycles(20000000);   // 1 cycle lasts 1 microsecond. 1 000 000 microseconds in 1 second
}

void openClaw()
{
    //-- setup ports
    P1DIR |= BIT0;              // set P1.0 to output LED1
    P1OUT |= BIT0;              // set LED1=1 initially
    TA0CCR0 = 20000;            // set PWM period. PWM - Pulse Width Modulation
    // set PWM duty cycle
    TA0CCR1 = 500;              // open claw a little bit
    __delay_cycles(20000000);   // 1 cycle lasts 1 microsecond. 1 000 000 microseconds in 1 second
}

void rotateCntrClkVertJointOne()
{
    //-- setup ports
    P1DIR |= BIT4;              // set P1.4 to output LED1
    P1OUT |= BIT4;              //
    TA0CCR0 = 20000;            // set PWM period. PWM - Pulse Width Modulation
    // set PWM duty cycle
    TA0CCR1 = 900;              // open claw a little bit
    __delay_cycles(20000000);   // 1 cycle lasts 1 microsecond. 1 000 000 microseconds in 1 second
}

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer

    //-- setup timer
    TA0CTL = TACLR;             // reset timer A
    TA0CTL |= MC_1;             // put timer A to UP mode. CCR0 has its functionality to reset timer only in up mode
                                // MC_1 means we set up-mode for CCR0 only.
    TA0CTL |= TASSEL_2;         // choose SMCLK 1 MHz

    //-- setup compare IRQs
    TA0CCTL0 |= CCIE;           // Capture/compare interrupt enable. Local enable for CCR0
    TA0CCTL1 |= CCIE;           // Capture/compare interrupt enable. Local enable for CCR1
    __enable_interrupt();       // enable maskable interrupts and only CCR0 and CCR1 can trigger an interrupt
                                // because we enabled them with TACCTL0 |= CCIE and TACCTL1 |= CCIE
    TA0CCTL0 &= ~CCIFG;         // clear flag for CCR0
    TA0CCTL1 &= ~CCIFG;         // clear flag for CCR1

    openClaw();
    //closeClaw();
    //rotateCntrClkVertJointOne();


    //-- main loop
    //while(1){}

    return 0;
}

//---------------- ISRs Interrupt Service Routines ----------------//
#pragma vector = TIMER0_A0_VECTOR   // ISR for period
__interrupt void ISR_TA0_CCR0(void)
{
    static n=0;         // PWM channel index

    P1OUT = 1 << n;     // Set P1.n high
    TA0CCR1 = pw[n];    // Set timer for current pin's pulse width

    n = (n+1)%8;        // Move on to next PWM channel
    TA0CCTL0 &= ~CCIFG; // Reset CC interrupt flag

   // P1OUT |= BIT0;                  // turn LED1 on
  //  TA0CCTL0 &= ~CCIFG;              // clear flag for CCR0
}
#pragma vector = TIMER0_A1_VECTOR
__interrupt void ISR_TA0_CCR1(void)
{
    //P1OUT &= ~BIT0;                 // turn LED1 off
    P1OUT = 0;
    TA0CCTL1 &= ~CCIFG;              // clear flag for CCR1
}
