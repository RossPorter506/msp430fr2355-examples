#include <msp430.h>
#include <stdint.h>

#define CALADC_15V_30C  *((unsigned int *)0x1A1A)                 // Temperature Sensor Calibration-30 C
                                                                  // See device datasheet for TLV table memory mapping
#define CALADC_15V_85C  *((unsigned int *)0x1A1C)                 // Temperature Sensor Calibration-High Temperature (85 for Industrial, 105 for Extended)

int32_t adcCount, v_temp_mv, tempDegC, calibTempDegC;             // ADC count and temperature in celcius, global for debugging purposes

void main(void) {
	WDTCTL = WDTPW + WDTHOLD;                 	// Stop watchdog timer

	PMMCTL2 |= TSENSOREN + INTREFEN;            // Enable internal temp sensor and 1.5V reference

	// TODO: Mention temp sensor minimum sample time
    ADCMCTL0 |= ADCSREF_1 + ADCINCH_12;         // Use internal 1.5V ref, input channel 12 (internal temp sensor)
    ADCCTL1 |= ADCDIV_3 + ADCSHP;               // Divide input clock by 4
    ADCCTL2 &= ~ADCRES;                         // Clear bits
    ADCCTL2 |= ADCRES_2;                        // 12-bit resolution
    ADCIE |= ADCIE0;                            // Enable interrupts
    ADCCTL0 |= ADCSHT_4 + ADCON;                // sample and hold for 64 clock cycles, enable ADC.
	__delay_cycles(100);						// Wait for reference to settle

	PM5CTL0 &= ~LOCKLPM5;                       // Unlock GPIO

	while(1) {
	    ADCCTL0 |= ADCENC + ADCSC;              // Sampling and conversion start
		__bis_SR_register(CPUOFF + GIE);        // Go into low power mode 0 with interrupts enabled

		adcCount = ADCMEM0;					    // Fetch ADC conversion result

		// The ADC returns a count between 0 and 4095. We know that 0 = 0V and 4096 = Vref = 1.5V
		v_temp_mv = (((int32_t)adcCount)*1500) / 4096;

		// Figure 21-19 in the user manual shows a linear relationship between temp and voltage.
		// Table 5-10 in the MSP430FR2355 datasheet gives a voltage at 30C plus the slope (2.32 mV/C):
		// temp = m*voltage + c => m = 1/2.32 = 0.431. Plug in temp=30, voltage=788 => c = -309.655
		// Note integer division, so multiply first (beware overflow!), divide last to maximise accuracy
		tempDegC = ((431*((int32_t)v_temp_mv))-309655) / 1000;

		// TI also provides some device-specific calibrated values that can be used instead
		calibTempDegC = (adcCount-CALADC_15V_30C)*(85-30)/(CALADC_15V_85C-CALADC_15V_30C)+30;

		__no_operation();                       // Required for breakpoint
	}
}

#pragma vector=ADC_VECTOR
__interrupt void ADC_ISR (void) {
	__bic_SR_register_on_exit(CPUOFF);          // Clear CPUOFF bit from 0(SR)
	ADCIFG &= ~ADCIFG0;                         // Clear interrupt flag
}
