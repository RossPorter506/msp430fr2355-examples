#include <msp430.h> 

void main(void) {
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
	
    P1DIR |= BIT0;				// P1.0 (Red) -> Output
    P2DIR &= ~BIT3;				// P2.3 (SW2) -> Input

    /* 
    When the switch is pressed P2.3 will be shorted to ground. When the switch isn't pressed the 
    pin isn't connected to anything. In order to ensure it always has a definite high or low value
    we should use a pullup resistor to pull the pin high when it isn't shorted to ground through the switch.

    If a pin is configured as an input and its PxREN bit is also set, then PxOUT controls whether 
    a pullup or pulldown resistor is connected. Table 8-1 from the 'MSP430FR4xx and MSP430FR2xx Family User's Guide':
    PxDIR   PxREN   PxOUT   I/O Configuration
    0       0       x       Input
    0       1       0       Input with pulldown resistor
    0       1       1       Input with pullup resistor
    1       x       x       Output
    */
    P2REN |= BIT3;				// P2.3 Enable pull resistor
    P2OUT |= BIT3;				// Pull P2.3 up

    // Unlock GPIO
    PM5CTL0 &= ~LOCKLPM5;

    while(1) {
        // The pin will be 0 when the switch is pressed, 
        // so when the switch is NOT pressed then (P1IN & BIT3) will be non-zero (true).
    	if(P2IN & BIT3) {		// If SW is NOT pressed
    		P1OUT &= ~BIT0;		// LED OFF
        }
    	else {
    		P1OUT |= BIT0;		// else LED ON
        }
    }
}
