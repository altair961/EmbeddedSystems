#include <msp430.h>

// PWM channels duty cycle array
int pw[] = {1500,1500,1500,1500,1500,1500,1500,1500};

//  2500 ms is maximum +90 degrees
//  1500 ms is middle 0 degrees
//  450 ms is minimum -90 degrees

void delay()
{
    __delay_cycles(1000000);
}

int main( void )
{

    WDTCTL = WDTPW + WDTHOLD; // Disable watchdog timer

    // P1DIR = 0b00111111; // Make P1.0-5 outputs
    P1DIR = 0x7F; // Make P1.0-6 outputs
    // Configure Timer A0 Compare interrupts
    TA0CTL = TASSEL_2 + MC_1 + ID_0; // "up" mode, input divider = 1
    TA0CCR0 = 2500;                  // set timer period to 2.5ms
    TA0CCTL0 = CCIE;                 // enable CC0 interrupt
    TA0CCR1 = 1500;                  // set pulse width to 1.5ms
    TA0CCTL1 = CCIE;                 // enable CC1 interrupt
    _BIS_SR(GIE);                    // global interrupt enable

    // From this point on, we only need to write values
    // into the pw[] array to set the duty cycle on all
    // eight PWM channels (P1.0-7), or to be precise,
    // whichever channels are actually enabled as digital
    // outputs (six in this case).

      // stable
      pw[0] = 1500;
      pw[1] = 1100; //1000 // 800
      pw[2] = 1250;
      pw[3] = 2100;
      pw[4] = 2250; // 1250
      pw[5] = 1000; // 500 - 1200
      delay();
      delay();
      delay();
      pw[1] = 1000;
      delay();
      delay();
      pw[1] = 800;
      pw[2] = 800;//1100;
      pw[3] = 700;//800
      delay();
      delay();
      delay();
      pw[3] = 500;
      delay();
      pw[2] = 500;
      delay();
      pw[4] = 2250; // horizontal with engine on top
      delay();
      delay();
      delay();
      pw[1] = 600; //900
      pw[3] = 700;
      delay();
      delay();
      pw[3] = 900;
      delay();
      pw[0] = 2300;
      delay();
      //pw[5] = 500; // 500 - 1200 // open the claw preparing for grab
     // pw[3] = 1250;
   /*   delay();
      delay();
      delay();
      pw[1] = 900;*/
      delay();
      delay();
      delay();
    //  pw[1] = 1100; // 1500
      pw[3] = 1000; // 600
      delay();
      delay();
      pw[5] = 500; // 500 - 1200 // open claw to grab the object
     // pw[1] = 1500;
      delay();
      //pw[3] = 600;
      delay();
      pw[3] = 1350;
      delay();
      delay();
      pw[1] = 1000; //900
      delay();
      delay();
      pw[5] = 900; // 500 - 1200 // close claw to grab the object

      /*
      //Object has been grabbed move backwards
      //pw[1] = 1100; // 1500
      pw[3] = 1000; // 600
      delay();
      delay();
      delay();
      pw[1] = 900;
      delay();
      pw[1] = 1500;
      pw[3] = 600;
      delay();
      delay();
      pw[0] = 600;
      delay();
      delay();
      pw[5] = 500; // Open claw to release the object
      delay();
      delay();
      pw[1] = 1000;
      delay();
      delay();
      pw[2] = 1250;
      pw[1] = 1500;*/

   //   pw[3] = 1000;

  //    pw[0] = 1500;

    while(1)
    {
        ;
    }

    //
    // A quick example to test: Do a different number of
    // angle steps on each of the six PWM outputs. 1 step
    // on channel 0, 2 steps on channel 1, 3 steps on
    // channel 2, and so on.
    //
/*
    int channel, counter = 0;
    while(1)
    {
        counter++;
        for (channel=0 ; channel<6 ; ++channel)
        {
            pw[channel] = 1000 + (counter%(channel+1))*100;
        }
        __delay_cycles(500000);
    }
*/
    return 0;
}

//
// Timer A0 CC0 interrupt service routine.
// This ISR is triggered when Timer A0 reaches
// its maximum value TA0CCR0 and resets to zero.
// This ISR starts a pulse on one PWM channel
// every 2.5ms. It cycles through all eight channels
// in turn. After starting the pulse, it sets
// the TA0CCR1 register to the pulse width of
// the current pin so that the pulse will be
// ended by the partner ISR after the appropriate
// time delay (for this channel's duty cycle).
//
#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A0_CC0(void)
{

    static n=0;         // PWM channel index

    P1OUT = 1 << n;     // Set P1.n high
    TA0CCR1 = pw[n];    // Set timer for current pin's pulse width


    n = (n+1)%8;        // Move on to next PWM channel
    TA0CCTL0 &= ~CCIFG; // Reset CC interrupt flag
}

//
// Timer A0 CC1 interrupt service routine.
// This ISR is responsible for ending each PWM
// pulse on all channels. It is triggered when
// Timer A0 matches TA0CCR1, which is at a
// different time for each PWM channel.
//
#pragma vector=TIMER0_A1_VECTOR
__interrupt void Timer_A1_CC1(void)
{
    P1OUT = 0;


    TA0CCTL1 &= ~CCIFG;
}
