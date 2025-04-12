#include <msp430.h> 
#include <stdbool.h>

#define SW		BIT3						// Switch -> P2.3
#define RED		BIT0						// Red LED -> P1.0
#define GREEN 	BIT6						// Green LED -> P6.6

void main(void) {
    WDTCTL = WDTPW | WDTHOLD;			// Stop watchdog timer

    P1DIR |= RED;					    // Set LED pins -> Output
    P6DIR |= GREEN;					    // Set LED pins -> Output

    P2DIR &= ~SW;						// Set SW pin -> Input
    P2REN |= SW;						// Enable Resistor for SW pin
    P2OUT |= SW;						// Select Pull Up for SW pin

    // LED default states: Green on, red off.
    P1OUT |= GREEN;
    P6OUT &= ~RED;

    // Unlock GPIO
    PM5CTL0 &= ~LOCKLPM5;

    while(1) {
        bool update = false;
    	if(!(P2IN & SW)) { 				// If SW is pressed
    		__delay_cycles(20000);		// Wait 20ms
    		if(!(P2IN & SW)) {          // Check if SW is still pressed. Ignores presses shorter than 20ms
    			while(!(P2IN & SW));	// Wait till SW released
    			update = true;			// Change flag value
    		}
    	}
    	if(update) {
            // Toggle LEDs
    		P6OUT ^= GREEN;
    		P1OUT ^= RED;
    	}
    }
}
