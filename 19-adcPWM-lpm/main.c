#include <msp430.h>

#define LED     BIT6                            // Connect an external LED on P1.6
#define AIN     BIT1                            // Present an analog voltage on pin 1.1

void main(void) {
	WDTCTL = WDTPW + WDTHOLD;                 	// Stop watchdog timer

	ADCMCTL0 = ADCSREF_0 + ADCINCH1;            // VCC as reference, A0 as ADC input
	ADCCTL1 = ADCDIV_3;                         // ADC clock div / 4
	ADCIE = ADCIE;                              // Enable interrupts
	ADCCTL0 = ADCSHT_4 + ADCON + ADCENC;        // Sample and hold for 64 clock cycles, enable ADC

	// P1.6 as output controlled by timer module
    P1DIR |= LED;
    P1SEL0 &= ~LED;
    P1SEL1 |= LED;

    // P1.1 as ADC input
    P1SEL0 |= AIN;
    P1SEL1 |= AIN;

    TB0CCR0 = 1023;								// Set Timer0 PWM Period
    TB0CCTL0 |= CCIE;							// Enable Overflow Interrupt
	TB0CCTL1 = OUTMOD_7;						// Set TB0.1 Waveform Mode
	TB0CCR1 = 0;								// Set TB0.1 PWM toggle point
	TB0CTL = TBSSEL_2 + MC_1;					// Timer Clock -> SMCLK, Mode -> Count up

	PM5CTL0 &= ~LOCKLPM5;                       // Unlock GPIO

   	__bis_SR_register(LPM0_bits + GIE);			// Go into LPM0, enable CPU interrupts
}

#pragma vector=ADC_VECTOR						// ADC Conversion Complete Interrupt Vector
__interrupt void ADC_ISR (void) {
	TB0CCR1 = ADCMEM0;							// Set duty cycle = ADC value
}

#pragma vector = TIMER0_B0_VECTOR				// TB0CCR0 interrupt vector
__interrupt void CCR0_ISR(void) {
    // If the ADC is idle start a new conversion
	if(!(ADCCTL1 & ADCBUSY))
		ADCCTL0 |= ADCSC;
}
