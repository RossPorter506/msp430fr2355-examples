
// Note: Sometimes the button doesn't light up or only lights up briefly when the button is pressed.
// Why do you think this is? How could you solve it?

#include <msp430.h> 

#define SW		BIT3					// Switch -> P2.3
#define GREEN	BIT6					// Green LED -> P6.6

void main(void) {
    WDTCTL = WDTPW | WDTHOLD;			// Stop watchdog timer

    P6DIR |= GREEN;						// Set LED pin -> Output

    P2DIR &= ~SW;						// Set SW pin -> Input
    P2REN |= SW;						// Enable Resistor for SW pin
    P2OUT |= SW;						// Select Pull Up for SW pin

    // Unlike the previous example, we're going to use an interrupt 
    // to toggle the LED, rather than polling the pin repeatedly.
    P2IES &= ~SW;						// Select Interrupt on Rising Edge

    PM5CTL0 &= ~LOCKLPM5;               // Unlock GPIO

    // Enable interrupt on SW pin. The user manual recommends doing this after unlocking GPIO.
    P2IE |= SW;							

    // We could put an empty loop here, but instead let's enter a low power mode.
    // This will wake up the CPU whenever an interrupt occurs, then put it back to sleep afterwards.
    __bis_SR_register(LPM4_bits + GIE);	// Enter LPM4 and enable CPU interrupt
}

// This function runs whenever any GPIO interrupts occur on port 2.
// Because we only enabled interrupts on P2.3, we don't need to check which pin
// produced the interrupt.
// C was not really designed with interrupts in mind, so this code looks a bit weird:
#pragma vector=PORT2_VECTOR
__interrupt void Port_2(void) {
	P6OUT ^= GREEN;						// Toggle Green LED

    // We also have to clear the interrupt flag that prompted this interrupt, as otherwise 
    // the interrupt will immediately fire again.
	P2IFG &= ~SW;						// Clear SW interrupt flag
}
