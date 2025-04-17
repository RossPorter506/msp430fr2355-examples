/*
 In this example we use a timer to measure the period of an external signal.
 P1.2 is an input pin controlled by the timer module and should be fed a digital signal with
 rising edges separated by no more than ~30ms, i.e. >60Hz.
*/

#include <msp430.h>
#include <stdint.h>
#include <stdbool.h>

// Global variables. Because first_edge, edge1 and edge2 are changed in the ISR below they must be marked volatile.
volatile bool first_edge;
volatile uint16_t edge1, edge2;

// These ones technically don't need to be global, but it makes it easier to view them while debugging if they're global.
uint16_t period;
uint32_t freq;

void main(void) {
	WDTCTL = WDTPW + WDTHOLD;                 		// Stop watchdog timer

	// Set P1.6 -> TB0.1 Capture Mode
	P1DIR &= ~BIT6;
	P1SEL0 &= ~BIT6;
	P1SEL1 |= BIT6;

    PM5CTL0 &= ~LOCKLPM5;                           // Unlock GPIO

    // TODO: Add more explanations
    // Capture mode, Rising edge, Interrupt enable, Synchronize, Source -> CCI1A
    // Table 6-16 in the MSP430FR2355 datasheet shows that CCI1A = TB0.1 = P1.6
	TB0CCTL1 |= CAP + CM_1 + CCIE + SCS + CCIS_0;
	// Clock -> SMCLK, Continuous mode, Clear timer
	TB0CTL |= TBSSEL_2 + MC_2 + TBCLR;

	while(1) {
	    first_edge = true;							// Initialise flag for new capture
		__bis_SR_register(LPM0_bits + GIE);   		// Enter low power mode 0, Enable Interrupt

		//Exits LPM0 after 2 rising edges are captured

		if(edge2 > edge1) {                         // Only calculate if no overflow occured
			period = edge2 - edge1;             	// Calculate period
			freq = 1000000L/period;                 // TODO: Explain L suffix
		}
		__no_operation();                     		// For inserting breakpoint in debugger
	}
}

// Table 6-2 in the MSP430FR2355 datasheet tells us that a TB0CCR1 interrupt should be caught by TIMER0_B1_VECTOR
// ...well it tells us that the interrupt vector address is 0xFFF6, and ctrl+clicking on TIMER0_B1_VECTOR brings
// us to the definition, where a comment labels it as 0xFFF6.
#pragma vector = TIMER0_B1_VECTOR
__interrupt void TIMER0_B1_ISR (void) {
    if (TB0IV == TB0IV_TBCCR1) {                        // Check if TACCR1 generated this interrupt
        if (first_edge) {
            edge1 = TB0CCR1;                            // Store timer value of 1st edge
            first_edge = false;                         // Increment count
        }
        else {
            edge2 = TB0CCR1;                            // Store timer value of 2nd edge
            first_edge = true;                          // Reset flag
            __bic_SR_register_on_exit(LPM0_bits + GIE); // Exit LPM0 on return to main
        }
    }
}
