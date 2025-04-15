/*
 This example generates a Pulse Width Modulation (PWM) output to drive an external LED.
 PWM is a way to drive a component at less than 100% intensity using only high or low values by
 rapidly turning the component on and off. If done fast enough this is invisible to the user.
 In this example we use it to drive an LED at varying intensities.
 The ratio of time spent on to off is called the 'duty cycle'

 An almost-100% duty cycle PWM signal might look like:
 |‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾|__|‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾|__

 A 50% duty cycle PWM signal:
 |‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾|____________________|‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾|____________________

 An almost 0% PWM signal:
 |‾‾|______________________________________|‾‾|______________________________________

*/

#include <msp430.h>
#include <stdint.h>

#define LED	BIT6						    // External LED on P1.6
#define PWM_PERIOD 500

void main(void) {
	WDTCTL = WDTPW + WDTHOLD;				// Stop WDT
	P1DIR |= LED;							// Green LED -> Output

	// Allow P1.6 to be controlled by timer TB0 CCR1 module
	// See Table 6-63 in MSP430FR2355 datasheet for details.
	P1SEL0 &= ~LED;
	P1SEL1 |= LED;

	/*
	 In order to implement PWM control, we need two set points - one denotes the maximum PWM point,
	 and another denotes the toggle point at which the output will change. By changing this second set point
	 at runtime we can control how much of the time the output spends high vs low.

	                                      v--------- PWM toggle point
	                                             v-- PWM period
	 Values: |----------------------------T------|-----------------------------T------|
	 Output: _____________________________|‾‾‾‾‾‾|_____________________________|‾‾‾‾‾‾|

	 The MSP430 Reset/Set mode (see Table 14-4 in the user manual) is exactly what we need for this:
	 In the Reset/Set mode, CCR0 stores the PWM period. CCR1 through CCR6 can each hold a separate toggle point.
	 In our case we only want one PWM output so we'll only use CCR1, but we could control 5 other pins with Timer B0 if we needed to.
    */

	TB0CCR0 = PWM_PERIOD;					// Set Timer B0 PWM max point
	TB0CCTL1 = OUTMOD_7;					// Set TB0.1 to Reset/Set mode
	TB0CCR1 = 1;							// Set TB0.1 PWM toggle point to 1 initially (almost 100% on)
	TB0CTL = TBSSEL_2 + MC_1;				// Timer Clock -> SMCLK, Mode -> Count up

	PM5CTL0 &= ~LOCKLPM5;                   // Unlock GPIO

	while(1) {
		for(uint16_t i = 0; i < PWM_PERIOD; i++) {
			TB0CCR1 = i;						// Increase duty cycle from min to max
			__delay_cycles(10*PWM_PERIOD);
		}
		for(uint16_t i = PWM_PERIOD; i > 0; i--) {
			TB0CCR1 = i;						// Decrease duty cycle from max to min
			__delay_cycles(10*PWM_PERIOD);
		}
	}
}
