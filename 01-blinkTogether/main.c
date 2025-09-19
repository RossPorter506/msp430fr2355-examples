#include <msp430.h>
#include <stdint.h>

void main(void)
{
    WDTCTL = WDTPW | WDTHOLD; // Stop watchdog timer

    // Set pins 1.0 (red LED) and 6.6 (green LED) as outputs.
    // The BITX macros evaluate to the equivalent bit:
    // Inside the msp430.h header file BIT0 is '#define BIT0 (0x0001)', BIT1 is '#define BIT1 (0x0002)' BIT7 is #define BIT7 (0x0080)
    // Which is equivalent to BIT0 = 0b00000001, BIT1 = 0b00000010, BIT7 = 0b10000000, etc.
    // This macro which is provided by Texas Instruments, allows us to write human readable code as having 0x0080 is harder to read than BIT7
    P1DIR |= BIT0;
    P6DIR |= BIT6;

    // Unlock GPIO
    PM5CTL0 &= ~LOCKLPM5;

    while (1)
    {
        P1OUT &= ~BIT0; // Red LED output is set to 0 (OFF) without editing the state of other pins
        P6OUT &= ~BIT6; // Green LED -> OFF

        // Software delay. Delay for 20,000 CPU cycles.
        // Default CPU freq is 1MHz, so this is 100k/1MHz = 100ms
        __delay_cycles(100000);

        P1OUT |= BIT0; // Red LED output is set to 1 (ON) without editing the state of other pins
        P6OUT |= BIT6; // Green LED -> ON

        __delay_cycles(100000);
    }
}
