#include <msp430.h> 
#include <stdint.h> 

void main(void) {
    WDTCTL = WDTPW | WDTHOLD;		//Stop watchdog timer
    
    // Set pins 1.0 (red LED) and 6.6 (green LED) as outputs.
    // The BITX macros evaluate to the equivalent bit:
    // BIT0 = 0b00000001, BIT1 = 0b00000010, BIT7 = 0b10000000, etc.
    P1DIR |= BIT0;
    P6DIR |= BIT6;

    // Unlock GPIO
    PM5CTL0 &= ~LOCKLPM5;

    while(1)
    {
        P1OUT &= ~BIT6;	    // Green LED -> OFF
        P6OUT |= BIT0;		// Red LED -> ON

        // Software delay. Delay for 20,000 CPU cycles.
        // Default CPU freq is 1MHz, so this is 100k/1MHz = 100ms
        __delay_cycles(100000);

        P1OUT &= ~BIT0;		// Red LED -> OFF
        P6OUT |= BIT6;		// Green LED -> ON

        __delay_cycles(100000);
    }
}
