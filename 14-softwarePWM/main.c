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

 In this example we show how to implement this manually in software.
 The next example will show how to use the timer peripheral to do this.
*/
#include <msp430.h> 
#include <stdint.h>

#define LED	BIT6				    // External LED on P1.6
#define PWM_PERIOD 500

// Custom delay function, as __delay_cycles accepts only constants!
void delay(uint16_t t) {            
	for(uint16_t i = t; i > 0; i--){
		__delay_cycles(1);
    }
}

void main(void) {
    WDTCTL = WDTPW | WDTHOLD;		// Stop watchdog timer

    P1DIR |= LED;					// LED -> Output

    PM5CTL0 &= ~LOCKLPM5;           // Unlock GPIO

    while(1) {
    	uint16_t on_time;
    	for(on_time = 1; on_time < PWM_PERIOD; on_time++) {     // Increasing intensity
   			P1OUT |= LED;			                            // LED ON
   			delay(on_time);				                        // Delay for ON time
   			P1OUT &= ~LED;		                                // LED OFF
   			delay(PWM_PERIOD-on_time);			                // OFF time = Period - ON time
    	}
    	for(on_time = PWM_PERIOD; on_time > 1; on_time--) {     // Decreasing intensity
   			P1OUT |= LED;			                            // LED ON
   			delay(on_time);				                        // Delay for ON time
   			P1OUT &= ~LED;		                                // LED OFF
   			delay(PWM_PERIOD-on_time);			                // OFF time = Period - ON time
    	}
    }
}
