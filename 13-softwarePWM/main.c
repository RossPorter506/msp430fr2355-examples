/*

 This example generates a Pulse Width Modulation (PWM) output to drive the red LED on P1.0.
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

 In this example we show how to implement this manually in software.

 In order to implement PWM control, we need two set points - one denotes the maximum PWM point,
 and another denotes the toggle point at which the output will change. By changing this second set point
 at runtime we can control how much of the time the output spends high vs low.

									  v--------- PWM toggle point
											 v-- PWM period
 Values: |----------------------------T------|-----------------------------T------|
 Output: _____________________________|‾‾‾‾‾‾|_____________________________|‾‾‾‾‾‾|

 In the code below PWM_PERIOD is the, well, PWM period. In the main loop `onTime` is the toggle point.
*/
#include <msp430.h>
#include <stdint.h>

#define LED BIT0 // Red LED on P1.0
#define PWM_PERIOD 500

// Custom delay function, as __delay_cycles accepts only constants!
void delay(uint16_t t)
{
	for (uint16_t i = t; i > 0; i--)
	{
		__delay_cycles(1);
	}
}

void main(void)
{
	WDTCTL = WDTPW | WDTHOLD; // Stop watchdog timer

	P1DIR |= LED; // LED -> Output

	PM5CTL0 &= ~LOCKLPM5; // Unlock GPIO

    // As an explanation of the code below:
    // In the first for loop, as onTime increases, the second delay will decrease as PWM_PERIOD is constant.
    // This means the LED is on for longer and off for shorter, increasing the intensity.
    // In the second for loop, as onTime decreases, the second delay will increase.
    // This means the LED is on for shorter and off for longer, decreasing the intensity.
	while (1)
	{
		for (uint16_t onTime = 1; onTime < PWM_PERIOD; onTime++)
		{								// Increasing intensity
			P1OUT |= LED;				// LED ON
			delay(onTime);				// Delay for ON time
			P1OUT &= ~LED;				// LED OFF
			delay(PWM_PERIOD - onTime); // OFF time = Period - ON time
		}
		for (uint16_t onTime = PWM_PERIOD; onTime > 1; onTime--)
		{								// Decreasing intensity
			P1OUT |= LED;				// LED ON
			delay(onTime);				// Delay for ON time
			P1OUT &= ~LED;				// LED OFF
			delay(PWM_PERIOD - onTime); // OFF time = Period - ON time
		}
	}
}
