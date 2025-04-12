#include <msp430.h> 

#define SW	BIT3					// Switch -> P2.3
#define LED	BIT0					// Red LED -> P1.0

void main(void) {
    WDTCTL = WDTPW | WDTHOLD;		// Stop watchdog timer

    P1DIR |= LED;					// Set LED pin -> Output

    P2DIR &= ~SW;					// Set SW pin -> Input
    P2REN |= SW;					// Enable Resistor for SW pin
    P2OUT |= SW;					// Select Pull Up for SW pin

    // Unlock GPIO
    PM5CTL0 &= ~LOCKLPM5;

    while(1) {
        // If SW is pressed
    	if( !(P2IN & SW) ) {    
    		__delay_cycles(20000);	// Wait 20ms to debounce
    		while( !(P2IN & SW) );	// Wait till SW is released
       		P1OUT ^= LED;			// Toggle LED
    	}
    }
}
