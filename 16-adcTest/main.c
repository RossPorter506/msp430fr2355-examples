#include <msp430.h>
#include <stdint.h>

int32_t adcCount, tempr_c;                      // ADC count and temperature in celcius, global for debugging purposes

void main(void) {
	WDTCTL = WDTPW + WDTHOLD;                 	// Stop watchdog timer

	PMMCTL2 |= TSENSOREN + INTREFEN;            // Enable internal temp sensor and 1.5V reference

    ADCMCTL0 = ADCSREF_1 + ADCINCH_12;          // Use internal 1.5V ref, input channel 12 (internal temp sensor)
    ADCCTL1 = ADCDIV_3;                         // Divide input clock by 4
    ADCCTL2 = ADCRES_2;                         // 12-bit resolution
    ADCIE = ADCIE0;                             // Enable interrupts
    ADCCTL0 = ADCSHT_4 + ADCON + ADCENC;        // sample and hold for 64 clock cycles, enable ADC.
	__delay_cycles(100);						// Wait for reference to settle

	while(1) {
	    ADCCTL0 |= ADCSC;                       // Sampling and conversion start
		__bis_SR_register(CPUOFF + GIE);        // Go into low power mode 0 with interrupts enabled

		adcCount = ADCMEM0;					    // Fetch ADC conversion result

		// The ADC returns a count between 0 and 4095. We know that 0 = 0V and 4096 = Vref = 1.5V
		int16_t v_temp_mv = (((int32_t)adcCount)*1500) / 4096;

		// Figure 21-19 in the user manual shows a linear relationship between temp and voltage.
		// Table 5-10 in the MSP430FR2355 datasheet gives a voltage at 30C plus the slope (2.32 mV/C):
		// temp = m*voltage + c => m = 1/2.32 = 0.431. Plug in temp=30, voltage=788 => c = -309.655
		// Note integer division, so multiply first (beware overflow!), divide last to maximise accuracy
		tempr_c = ((431*((int32_t)v_temp_mv))-309655) / 1000;

		__no_operation();                       // Required for breakpoint
	}
}

#pragma vector=ADC_VECTOR
__interrupt void ADC_ISR (void) {
	__bic_SR_register_on_exit(CPUOFF);        // Clear CPUOFF bit from 0(SR)
}
