#include <msp430.h> 


/**
 * main.c
 */
int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer

    P1DIR = 0x0;
    P1OUT = 0x0;

    P1DIR |= 0x1;  // we add 0000 0001 to 0000 0000
    P1OUT |= 0x1; // we added 0000 0001 to 0000 0000

    P1DIR |= 0x40; // we add 0100 0000
    P1OUT |= 0x40; // we added 0100 0000

    return 0;
}
