#include <msp430.h>

#define LED     BIT6                            // Connect an external LED on P1.6
#define AIN     BIT1                            // Present an analog voltage on pin 1.1

void main(void) {
	WDTCTL = WDTPW + WDTHOLD;                 	// Stop watchdog timer

	// P1.6 as output controlled by timer module
    P1DIR |= LED;
    P1SEL0 &= ~LED;
    P1SEL1 |= LED;

    // P1.1 as ADC input
    P1SEL0 |= AIN;
    P1SEL1 |= AIN;

	ADCMCTL0 |= ADCINCH_1;                      // VCC as reference, A1 as ADC input
	ADCCTL1 |= ADCSHP;
	ADCIE |= ADCIE0;                             // Enable interrupt on ADC conversion complete
	ADCCTL0 |= ADCSHT_4 + ADCON;                // Sample and hold for 64 clock cycles, enable ADC

    TB0CCR0 = 1023;								// Set Timer0 PWM Period
    TB0CCTL0 = CCIE;							// Enable Overflow Interrupt
	TB0CCTL1 = OUTMOD_7;						// Set TB0.1 Waveform Mode
	TB0CCR1 = 0;								// Set TB0.1 PWM toggle point
	TB0CTL = TBSSEL_2 + MC_1;					// Timer Clock -> SMCLK, Mode -> Count up

	PM5CTL0 &= ~LOCKLPM5;                       // Unlock GPIO

	__bis_SR_register(GIE);
   	while(1); //__bis_SR_register(LPM0_bits + GIE);			// Go into LPM0, enable CPU interrupts
}

#pragma vector=ADC_VECTOR						// ADC Conversion Complete Interrupt Vector
__interrupt void ADC_ISR (void) {
	TB0CCR1 = ADCMEM0;							// Set duty cycle = ADC value
	ADCIFG &= ~ADCIFG0;                         // Clear interrupt flag
}

#pragma vector = TIMER0_B0_VECTOR				// TB0CCR0 interrupt vector
__interrupt void CCR0_ISR(void) {
    // If the ADC is idle start a new conversion
	if(!(ADCCTL1 & ADCBUSY)) {
	    ADCCTL0 |= ADCENC + ADCSC;
	}
	TB0CTL &= ~TBIFG;                           // Clear interrupt flag
}
