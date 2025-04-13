#include <msp430.h>

#define LED	BIT6						// Green LED -> P6.6

void main(void) {
    WDTCTL = WDTPW + WDTHOLD;			// Stop watchdog timer

    P6DIR |= LED;						// Set LED pin -> Output
	P6OUT &=~ LED;						// Turn OFF LED

    PM5CTL0 &= ~LOCKLPM5;               // Unlock GPIO

    // Next we configure one of the timer modules to count to 5000 using ACLK as the clock source. 
    // ACLK is a low frequency 32.768kHz clock source by default, useful for timing human-scale events. 
    // See the Timer_B section of the user manual for details.
    TB0CCR0 = 5000;						// Set timer compare value
    TB0CCTL0 |= CCIE;					// Enable interrupt on compare match
    TB0CTL |= MC_1 + TBSSEL_1 + TBCLR ;	// Set Mode -> Count up, Clock -> ACLK, and clear timer
    // This produces an interrupt every 5000/32768 = 152ms

   	__bis_SR_register(LPM3_bits + GIE);	// Go into low power mode 3 (Only ACLK active), and enable CPU interrupt
}

// This interrupt will trigger whenever timer B0 reaches the CCR0 value (5000, in this case)
// Table 6-2 in the MSP430FR2355 datasheet tells us that a TB0CCR0 interrupt should be caught by TIMER0_B0_VECTOR
// ...well it tells us that the interrupt vector address is 0xFFF8, and ctrl+clicking on TIMER0_B0_VECTOR brings
// us to the definition, where a comment labels it as 0xFFF8.
#pragma vector = TIMER0_B0_VECTOR		// CCR0 Interrupt Vector
__interrupt void CCR0_ISR(void) {
	P6OUT ^= LED;						// Toggle LED
}
