
/* To see the UART messages being sent back by the MSP430, you will need to either open up PuTTY
OR use Code Composer Studio -> View -> Console -> Serial Console
From there you need to click Computer icon in the top right of the serial console and set it to the correct UART port
Which can be found in 'Device Manager' and the following settings: 9600 Baud, Parity None, Data size 8 and Stop Bits: 1.
Click OK and it now will be connected. You also want to turn off 'local echo' under the 3 dots.
Once you type anything into the terminal it should return the next character in the alphabet. For example typing 'a' will output 'b'
*/

#include <msp430.h>

void main(void)
{
	WDTCTL = WDTPW + WDTHOLD; // Stop watchdog

	// Select UART RX / TX function on P4.3 / P4.2 (eUSCI_A1 peripheral)
	// Note the line above UCA1TXD indicates it is INVERTED do NOT use this one, use standard UART
	// So use P4SELx as '01' - normal UART not '10'.
	// See Table 6-66 in MSP430FR2355 datasheet
	P4SEL0 = BIT3 + BIT2;
	P4SEL1 &= ~(BIT3 + BIT2);

	// UART is really sensitive to clock speeds, so this is one of those times where we have
	// to note that the default clock speed isn't 1MHz, it's actually 32 * 32.768 = 1.048576MHz

	// Configure UART for 9600 baud with a 1MHz clock
	// See Table 22-5 in the user manual for details

	UCA1CTL1 |= UCSWRST;			 // Put UART module into reset mode for configuration
	UCA1CTLW0 |= UCSSEL_2;			 // UART Clock -> SMCLK
	UCA1MCTLW_H = 0x22;				 // UCBRSx value
	UCA1MCTLW_L = UCBRF_13 + UCOS16; // UCBRFx value, oversampling mode on
	UCA1BRW = 6;					 // UCBRx value
	UCA1CTL1 &= ~UCSWRST;			 // Initialise UART module

	PM5CTL0 &= ~LOCKLPM5; // Unlock GPIO

	UCA1IE = UCRXIE; // Enable RX interrupt

	__bis_SR_register(LPM0_bits + GIE); // Enter LPM0, Enable Interrupt
}

#pragma vector = EUSCI_A1_VECTOR // UART RX Interrupt Vector
__interrupt void USCIA1RX_ISR(void)
{
	while (!(UCA1IFG & UCTXIFG))
		; // Wait for any ongoing transmission to finish
	// NOTE: You might be tempted to check for completion by checking
	// UCTXCPTIFG instead, but this doesn't work for UART: See Errata USCI42.
	UCA1TXBUF = UCA1RXBUF + 1; // TX -> Received Char + 1
	UCA1IFG &= ~UCRXIFG;
}
