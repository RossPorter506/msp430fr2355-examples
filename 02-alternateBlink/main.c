#include <msp430.h>

// Let's use some defines to make our code more readable.
#define RED_LED BIT0
#define GREEN_LED BIT6

void main(void)
{
    WDTCTL = WDTPW | WDTHOLD; // Stop watchdog timer

    // P1.0 (Red LED), P6.6 (Green LED) to outputs
    P1DIR |= RED_LED; // Equivalent of P1DIR |= BIT0; due to the #DEFINE at the top of the program
    P6DIR |= GREEN_LED;

    // Set initial values - red LED on, green LED off.
    P1OUT |= RED_LED;
    P6OUT &= ~GREEN_LED;

    // Unlock GPIO
    PM5CTL0 &= ~LOCKLPM5;

    while (1)
    {
        // Toggle LED values using XOR instead of setting and clearing separately
        P1OUT ^= RED_LED;
        P6OUT ^= GREEN_LED;
        // XOR (exclusive OR) truth table:
        // A  B  |  A XOR B
        // 0  0  |    0
        // 0  1  |    1
        // 1  0  |    1
        // 1  1  |    0
        __delay_cycles(100000); // 100ms delay
    }
}
