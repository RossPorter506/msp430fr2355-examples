#include <msp430.h>

void main(void)
{
	WDTCTL = WDTPW + WDTHOLD;			// Stop watchdog

	// Select UART RX / TX function on P1.6 / P1.7
	// See Table 6-63 in MSP430FR2355 datasheet
	P1SEL0 = BIT6 + BIT7;

	// UART is really sensitive to clock speeds, so this is one of those times where we have
	// to note that the default clock speed isn't 1MHz, it's actually 32*32.768 = 1.048576MHz

	// Configure UART for 9600 baud with a 1MHz clock
	// See Table 22-5 in the user manual for details
	UCA0CTL1 |= UCSWRST;                // Put UART module into reset mode for configuration
	UCA0CTLW0 |= UCSSEL_2;		        // UART Clock -> SMCLK
	UCA0MCTLW_H = 0x22;                 // UCBRSx value
	UCA0MCTLW_L = UCBRF_13 + UCOS16;    // UCBRFx value, oversampling mode on
	UCA0BRW = 6;                        // UCBRx value
	UCA0CTL1 &= ~UCSWRST;               // Initialise UART module

	PM5CTL0 &= ~LOCKLPM5;               // Unlock GPIO

	UCA0IE = UCRXIE;                    // Enable RX interrupt

	__bis_SR_register(LPM0_bits + GIE);	// Enter LPM0, Enable Interrupt
}

#pragma vector=EUSCI_A0_VECTOR			// UART RX Interrupt Vector
__interrupt void USCIA0RX_ISR(void) {
	while (!(UCA0IFG & UCTXIFG));		// Wait for any ongoing transmission to finish
	// NOTE: You might be tempted to check for completion by checking
	// UCTXCPTIFG instead, but this doesn't work for UART: See Errata USCI42.
	UCA0TXBUF = UCA0RXBUF + 1;			// TX -> Received Char + 1
	UCA0IFG &= ~UCRXIFG;
}
