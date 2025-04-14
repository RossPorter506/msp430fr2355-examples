#include <msp430.h>
#include <stdint.h>

char message[] = "Hello World! \r\n";

void print(char text[]) {
	uint8_t i = 0;
	while(text[i] != '\0') {            // Check for end of string
		while (!(UCA0IFG & UCTXIFG));	// Wait for any ongoing transmissions to finish
		// NOTE: You might be tempted to check for completion by checking
		// UCTXCPTIFG instead, but this doesn't work for UART: See Errata USCI42.
		UCA0TXBUF = text[i];			// TX -> current char
		i++;							// Increment counter
	}
}

void main(void) {
	WDTCTL = WDTPW + WDTHOLD;			// Stop watchdog

	// Select UART RX / TX function on P1.6 / P1.7
    // See Table 6-63 in MSP430FR2355 datasheet
    P1SEL0 = BIT6 + BIT7;

    // Configure UART for 9600 baud with a 1MHz clock
    // See Table 22-5 in the user manual for details
    UCA0CTL1 |= UCSWRST;                // Put UART module into reset mode for configuration
    UCA0CTLW0 |= UCSSEL_2;              // UART Clock -> SMCLK
    UCA0MCTLW_H = 0x20;                 // UCBRSx value
    UCA0MCTLW_L = UCBRF_8 + UCOS16;     // UCBRFx value, oversampling mode on
    UCA0BRW = 6;
    UCA0CTL1 &= ~UCSWRST;               // Initialise UART module

    PM5CTL0 &= ~LOCKLPM5;               // Unlock GPIO

    UCA0IE = UCRXIE;                    // Enable RX interrupt

	while(1) {
		print(message);
		__delay_cycles(100000);         // 100ms pause
	}
}
