#include <msp430.h>
#include <stdint.h>

volatile uint16_t bytesRemaining;
volatile uint16_t data;

void main(void) {
	WDTCTL = WDTPW + WDTHOLD;					            // Stop watchdog

	P1SEL0 |= BIT2 + BIT3;						            // Set P1.3 -> SCL & P1.2 -> SDA

	UCB0CTLW0 |= UCSWRST;						            // Hold USCI Module in Reset State
	UCB0CTLW0 |= UCMST + UCMODE_3 + UCSYNC + UCSSEL_2;		// I2C Master, use SMCLK
	UCB0BRW = 10;								            // fSCL = SMCLK/10 = ~100kHz
	UCB0I2CSA = 0x48;							            // Set slave address for LM75
	UCB0CTL1 &= ~UCSWRST;						            // Initialise USCI Module

	PM5CTL0 &= ~LOCKLPM5;                                   // Unlock GPIO

	UCB0IE = UCRXIE0;							            // Enable RX interrupt

	while (1) {
	  bytesRemaining = 2;                       // Reset RX byte counter
	  UCB0CTLW0 |= UCTXSTT;                    	// Send I2C start condition
	  __bis_SR_register(CPUOFF + GIE);        	// Enter LPM0, enable interrupts
	  // Remains in LPM0 until data is received

	  data &= 0x01FF;							// Use only 9 bits of data
	  data = data / 2;
	  __delay_cycles(100000);					// Delay 100 ms
  }
}

#pragma vector = EUSCI_B0_VECTOR
__interrupt void USCIB0TX_ISR(void) {
	bytesRemaining--;							// Decrement RX byte counter

	if (bytesRemaining) {
		data = (unsigned int)UCB0RXBUF<<1;		// Get first received byte
		UCB0CTL1 |= UCTXSTP;					// Generate I2C stop condition
	}
	else {
		data |= UCB0RXBUF>>7;					// Get second received byte & combine
		__bic_SR_register_on_exit(CPUOFF);		// Exit LPM0
	}
}

