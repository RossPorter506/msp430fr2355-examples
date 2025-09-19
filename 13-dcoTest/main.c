#include <msp430.h> 
#include <stdint.h>
#define SW		BIT3 // P2.3
#define LED	    BIT6 // External LED on P1.6

volatile uint8_t mode = 0;

void change_clock_freq(uint16_t target_freq_khz);

void main(void) {
    WDTCTL = WDTPW | WDTHOLD;			// Stop watchdog timer
	
    change_clock_freq(1000);            // Set clock to 1MHz (default)

    // Set P1.6 to be a timer module output
	P1DIR |= LED;
	P1SEL0 &= ~LED;
	P1SEL1 |= LED;

	TB0CCR0 = 60000;					// Set Timer0 PWM Period
	TB0CCTL1 = OUTMOD_7;				// Set TB0.1 Waveform Mode - Reset/Set mode
	TB0CCR1 = 30000;					// Set TB0.1 PWM toggle point
	TB0CTL = TBSSEL_2 + ID__8 + MC_1;	// Timer Clock -> SMCLK, Input divider = 8, Mode -> Up Count
	// This gives ~2Hz at a 1MHz clock, ~50Hz at 24MHz

	// Set SW as input with pullup resistor
    P2DIR &= ~SW;						// Set SW pin -> Input
    P2REN |= SW;						// Enable Resistor for SW pin
    P2OUT |= SW;						// Select Pull Up for SW pin

    P2IES &= ~SW;						// Select Interrupt on Rising Edge

    PM5CTL0 &= ~LOCKLPM5;               // Unlock GPIO

    P2IE |= SW;							// Enable Interrupt on SW pin

    __bis_SR_register(LPM0_bits + GIE);	// Enter LPM0 and Enable CPU Interrupt

}

#pragma vector=PORT2_VECTOR
__interrupt void Port_2(void) {
	switch(mode) {
		case 0:
			change_clock_freq(1000);    // Set DCO to 1MHz
			break;
		case 1:
		    change_clock_freq(4000);	// Set DCO to 4MHz
			break;
		case 2:
		    change_clock_freq(12000);	// Set DCO to 12MHz
			break;
		case 3:
		    change_clock_freq(24000);	// Set DCO to 24MHz
			break;
		default:
			break;
	}
	mode++;
	if(mode > 3){
		mode = 0;
	}
	P2IFG &= ~SW;                       // Clear interrupt flag
}

// Change the DCOCLK frequency using FLL stabilisation. See section 3.2.5 and 3.2.5.1 in the user manual.
// Takes a target frequency between 33kHz and 24,000 kHz. It clamps the value between these.

void change_clock_freq(uint16_t target_freq_khz) {
    if (target_freq_khz > 24000) {
        target_freq_khz = 24000;
    }
    else if (target_freq_khz < 33) {
        target_freq_khz = 33;
    }

    // The FLL ensures that the clock frequency (relative to the ref clock) remains constant even as voltage/temp varies.
    // The FLL can use either REFOCLK or the external XT1CLK as its reference clock. We assume 32768Hz in either case.
    const uint16_t fllRefFreq_hz = 32768;

    // Disable FLL. This is needed to prevent the FLL from acting as we make modifications to the clock setup
    __bis_SR_register(SCG0);

    // Clear DCO bits. These will be set by the FLL after it's re-enabled.
    CSCTL0 &= ~(DCO0 | DCO1 | DCO2 | DCO3 | DCO4 | DCO5 | DCO6 | DCO7 | DCO8);

    // Tell the FLL how many multiples of the ref clock we want
    CSCTL2 = ((((uint32_t)target_freq_khz)*1000 / fllRefFreq_hz) - 1);

    // Set coarse frequency regime
    CSCTL1 &= ~DCORSEL_7; // Clear all DCORSEL bits
    if (target_freq_khz <= 1500) {
        CSCTL1 |= DCORSEL_0;
    }
    else if (target_freq_khz <= 3000) {
        CSCTL1 |= DCORSEL_1;
    }
    else if (target_freq_khz <= 6000) {
        CSCTL1 |= DCORSEL_2;
    }
    else if (target_freq_khz <= 10000) {
        CSCTL1 |= DCORSEL_3;
    }
    else if (target_freq_khz <= 14000) {
        CSCTL1 |= DCORSEL_4;
    }
    else if (target_freq_khz <= 18000) {
        CSCTL1 |= DCORSEL_5;
    }
    else if (target_freq_khz <= 22000) {
        CSCTL1 |= DCORSEL_6;
    }
    else if (target_freq_khz <= 24000) {
        CSCTL1 |= DCORSEL_7;
    }

    // The FRAM on the MSP430 can only operate at 8MHz, so if the CPU goes faster we have to add some delays.
    // See section 5.3 of the MSP430FR2355 datasheet for details
    if (target_freq_khz > 16000) {
        FRCTL0 = FRCTLPW | NWAITS_2;
    }
    else if (target_freq_khz > 8000) {
        FRCTL0 = FRCTLPW | NWAITS_1;
    }
    else {
        FRCTL0 = FRCTLPW | NWAITS_0;
    }

    // Re-enable FLL
    __bic_SR_register(SCG0);

    // The FLL will take time to stabilise, but we just return now.
}
