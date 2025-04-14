#include <msp430.h>
#include <stdint.h>

void print(char text[]) {
    uint16_t i = 0;
    while(text[i] != '\0') {            // Check for end of string
        while (!(UCA0IFG & UCTXIFG));   // Wait for any ongoing transmissions to finish
        UCA0TXBUF = text[i];
        i++;                            // Increment counter
    }
}

void printNumber(uint16_t num) {
    char buf[6];
    buf[5] = '\0';                      // Ensure string is null-terminated
    char* str = &buf[4];

    do {
        uint16_t m = num;
        num /= 10;
        uint8_t digit = (m - 10*num);   // Extract least significant digit
        char c = digit + '0';           // Convert digit to ASCII
        *str = c;                       // Store character in buffer
        str--;                          // Point to next slot in the buffer
    } while(num);

    print(str);
}

void main(void)
{
	WDTCTL = WDTPW + WDTHOLD;			        // Stop Watchdog

	// Select UART RX / TX function on P1.6 / P1.7
    // See Table 6-63 in MSP430FR2355 datasheet
    P1SEL0 = BIT6 + BIT7;

    // P1.5 as ADC input
    P1SEL0 = BIT5;
    P1SEL1 = BIT5;

    // Configure UART for 9600 baud with a 1MHz clock
    // See Table 22-5 in the user manual for details
    UCA0CTL1 |= UCSWRST;                        // Put UART module into reset mode for configuration
    UCA0CTLW0 |= UCSSEL_2;                      // UART Clock -> SMCLK
    UCA0MCTLW_H = 0x20;                         // UCBRSx value
    UCA0MCTLW_L = UCBRF_8 + UCOS16;             // UCBRFx value, oversampling mode on
    UCA0BRW = 6;
    UCA0CTL1 &= ~UCSWRST;                       // Initialise UART module

    ADCMCTL0 = ADCSREF_0 + ADCINCH_5;           // VCC as reference, ADC channel 5 as input (P1.5)
    ADCCTL0 = ADCSHT_4 + ADCON + ADCENC;        // sample and hold for 64 clock cycles, enable ADC.

    PM5CTL0 &= ~LOCKLPM5;                       // Unlock GPIO

	while(1) {
		ADCCTL0 |= ADCSC;                       // Sampling and conversion start
		while(ADCCTL1 & ADCBUSY);			    // Wait for conversion to end
		uint16_t adcVal = ADCMEM0;			    // Read ADC Value
		printNumber(adcVal);					// Print value on UART
		print("\r\n");							// Print newline
		__delay_cycles(10000);
	}
}
