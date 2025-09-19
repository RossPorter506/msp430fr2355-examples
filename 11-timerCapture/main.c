/*
 In this example we use a timer to measure the period of an external signal.
 P1.2 is an input pin controlled by the timer module and should be fed a digital signal with
 rising edges separated by no more than ~30ms, i.e. >60Hz.
*/

#include <msp430.h>
#include <stdint.h>
#include <stdbool.h>

// The 'volatile' keyword tells the compiler that a variable may change at any time, 
// such as from hardware, interrupts, or other threads. Without 'volatile', the compiler 
// might optimize code by assuming the variable does not change unexpectedly, which can 
// lead to incorrect behavior. This is especially important for variables shared with 
// ISRs, hardware registers, or in multithreaded code.
// See website below for a detailed explanation:
// https://medium.com/@shadababe04/volatile-embedded-syatem-a9bf83b19591

// Global variables. Because first_edge, edge1 and edge2 are changed in the ISR below they must be marked volatile.
volatile bool first_edge;
volatile uint16_t edge2;

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

    /*
     Unlike previous examples, here we are using the timer in Capture mode (as opposed to Compare mode).
     In Capture mode, the timer will record the timer value on an input signal event (as opposed to Compare mode,
     where an output signal changes based on the timer value). We tell the timer to capture the timer value on a rising or falling edge
     of an input signal (or both). The user manual also recommends operating in synchronous mode to avoid race conditions (user manual, 14.2.4.1).
     */

    // Capture mode - synchronously record on rising edge, Interrupt enable, Source -> CCI1A
    // Table 6-16 in the MSP430FR2355 datasheet shows that CCI1A = TB0.1 = P1.6
	TB0CCTL1 |= (CAP + SCS + CM_1) + CCIE + CCIS_0;
	// Clock -> SMCLK, Continuous mode, Clear timer
	TB0CTL |= TBSSEL_2 + MC_2 + TBCLR;

	while(1) {
	    first_edge = true;							// Initialise flag for new capture
		__bis_SR_register(LPM0_bits + GIE);   		// Enter low power mode 0, Enable Interrupt

		//Exits LPM0 after 2 rising edges are captured

			period = edge2;             	// Calculate period
			freq = 1000000UL / period;              // Note: The 'UL' suffix makes the literal an 'unsigned long'. Literals are signed unless specified.
		
		__no_operation();                     		// For inserting breakpoint in debugger
	}
}

// Table 6-2 in the MSP430FR2355 datasheet tells us that a TB0CCR1 interrupt should be caught by TIMER0_B1_VECTOR
// ...well it tells us that the interrupt vector address is 0xFFF6, and ctrl+clicking on TIMER0_B1_VECTOR brings
// us to the definition, where a comment labels it as 0xFFF6.
#pragma vector = TIMER0_B1_VECTOR
__interrupt void TIMER0_B1_ISR (void) {
    if (TB0IV & TB0IV_TBCCR1) {                        // Check if TACCR1 generated this interrupt
        if (first_edge) {
            first_edge = false;                         // Increment count
            TB0CTL |= TBCLR;                            // Clear the Timer0 Counter register to avoid overflow between captures

        }
        else {
            edge2 = TB0CCR1;                            // Store timer value of 2nd edge
            first_edge = true;                          // Reset flag

            __bic_SR_register_on_exit(LPM0_bits + GIE); // Exit LPM0 on return to main
        }
        TB0CCTL1 &= ~CCIFG;                             // Clear interrupt flag
    }
}
