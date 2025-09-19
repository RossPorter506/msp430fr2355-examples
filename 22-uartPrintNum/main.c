#include <msp430.h>
#include <stdint.h>

void print(char text[])
{
    uint16_t i = 0;
    while (text[i] != '\0')
    { // Check for end of string
        while (!(UCA1IFG & UCTXIFG))
            ; // Wait for any ongoing transmissions to finish
        // NOTE: You might be tempted to check for completion by checking
        // UCTXCPTIFG instead, but this doesn't work for UART: See Errata USCI42.
        UCA1TXBUF = text[i];
        i++; // Increment counter
    }
}

void printNumber(uint16_t num)
{
    char buf[6];
    buf[5] = '\0'; // Ensure string is null-terminated
    char *str = &buf[5];

    do
    {
        str--; // Point to next slot in the buffer
        uint16_t m = num;
        num /= 10;
        uint8_t digit = (m - 10 * num); // Extract least significant digit
        char c = digit + '0';           // Convert digit to ASCII
        *str = c;                       // Store character in buffer
    } while (num);

    print(str);
}

void main(void)
{
    WDTCTL = WDTPW + WDTHOLD; // Stop Watchdog

    // Select UART RX / TX function on P4.3 / P4.2 (eUSCI_A1 peripheral)
    // See Table 6-66 in MSP430FR2355 datasheet
    P4SEL0 = BIT3 + BIT2;
    P4SEL1 &= ~(BIT3 + BIT2);

    // Configure UART for 9600 baud with a 1.048576MHz clock
    // See Table 22-5 in the user manual for details
    UCA1CTL1 |= UCSWRST;             // Put UART module into reset mode for configuration
    UCA1CTLW0 |= UCSSEL_2;           // UART Clock -> SMCLK
    UCA1MCTLW_H = 0x22;              // UCBRSx value
    UCA1MCTLW_L = UCBRF_13 + UCOS16; // UCBRFx value, oversampling mode on
    UCA1BRW = 6;                     // UCBRx value
    UCA1CTL1 &= ~UCSWRST;            // Initialise UART module

    PM5CTL0 &= ~LOCKLPM5; // Unlock GPIO

    uint16_t count = 0;
    while (1)
    {
        printNumber(count);
        print("\r\n");
        count++;
        __delay_cycles(10000);
    }
}
