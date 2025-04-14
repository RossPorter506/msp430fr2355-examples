#include <msp430.h> 
#include <stdint.h>

#define	CLK		BIT4                // P1.4
#define DATA	BIT2                // P1.2
#define LATCH	BIT5                // P1.5

void shiftOut(uint16_t value, uint16_t length) {
	P1OUT &= ~LATCH;				// Latch LOW
	uint16_t i;
	for(i = 0; i < length; i++)	{   // For each bit
		P1OUT &= ~CLK;				// Clock LOW
		if(value & 1) {			    // Check LSB of value
			P1OUT |= DATA;			// Data HIGH if LSB = 1
		}
		else {
			P1OUT &= ~DATA;			// Data LOW if LSB = 0
		}
		P1OUT |= CLK;				// Clock HIGH
		value = value >> 1;			// Move next bit to LSB
	}
	P1OUT |= LATCH;					// Latch HIGH
}

void main(void) {
    WDTCTL = WDTPW | WDTHOLD;		// Stop watchdog timer
	
    P1DIR |= (CLK+DATA+LATCH);		// Set CLK, DATA & LATCH pins as output

    PM5CTL0 &= ~LOCKLPM5;           // Unlock GPIO

    uint16_t pattern = 0x80;	    // Initial value of pattern

    while(1) {
    	shiftOut(pattern, 8);		// Output current pattern
    	__delay_cycles(50000);		// Delay 50 ms

    	pattern = pattern >> 1;		// Shift pattern right by 1 bit
    	if(pattern == 0) {			// If sequence complete
    		pattern = 0x80;			// Re-initialise value
    	}
    }
}
