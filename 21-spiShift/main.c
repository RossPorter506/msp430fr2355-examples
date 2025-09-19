#include <msp430.h>

#define CLK     BIT5                // P1.5
#define DATA    BIT7                // P1.7
#define LATCH   BIT6                // P1.6

void main(void) {
	WDTCTL = WDTPW + WDTHOLD;                 		    // Stop watchdog timer

	P1DIR |= LATCH;                     			    // Set P1.5 as Output (Latch)
	P1SEL0 = DATA + CLK;							    // P1.7 -> SIMO (Data), P1.5 -> SCK (Clock)

	UCA0CTLW0 |= UCSWRST;                               // Hold USCI in reset state during configuration
	UCA0CTLW0 |= UCSYNC + UCMST + UCCKPL + UCSSEL_2;    // 3-pin SPI mode, SPI Master, Clock idles high, Clock -> SMCLK
    UCA0BR0 = 2;                                        // Clock div = 2  =>  SCK = SMCLK/2
    UCA0CTLW0 &= ~UCSWRST;                              // Initialise USCI module

    PM5CTL0 &= ~LOCKLPM5;                               // Unlock GPIO

	while(1) {
		P1OUT &= ~LATCH;				    // Set Latch to LOW
		while (!(UCA0IFG & UCTXIFG));		// Check if TX Buffer is empty
		UCA0TXBUF = 0xAA;					// Transmit first pattern
		while ((UCA0STATW & UCBUSY));		// Wait till TX Completes
		P1OUT |= LATCH;						// Set Latch to HIGH
		__delay_cycles(500000);				// Delay 500 ms

		P1OUT &= ~LATCH;					// Set Latch to LOW
		while (!(UCA0IFG & UCTXIFG));		// Check if TX Buffer is empty
		UCA0TXBUF = 0x55;					// Transmit second pattern
		while ((UCA0STATW & UCBUSY));		// Wait till TX Completes
		P1OUT |= LATCH;						// Set Latch to HIGH
		__delay_cycles(500000);				// Delay 500 ms
	}
}
