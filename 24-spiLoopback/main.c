#include <msp430fr2355.h>

// To have the loopback you need to connect P1.6 to P1.7, this connects the MOSI to the MISO 
// A red LED on P1.0 means that the SPI did not recieve the byte/packet transmitted. 
// A singular green LED on P6.6 just indicates that the code is uploaded  

void main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // Stop watchdog timer

    P1DIR |= BIT0; // P1.0 red LED output
    P6DIR |= BIT6; // P6.6 green LED output
    P1OUT &= ~BIT0; // Red LED -> OFF
    P6OUT |= BIT6; // Green LED -> ON

    P1SEL0 |= BIT5 | BIT6 | BIT7;      // P1.5=CLK, P1.6=MISO, P1.7=MOSI
    P1SEL1 &= ~(BIT5 | BIT6 | BIT7);

    UCA0CTLW0 = UCSWRST;               // Put eUSCI in reset
    UCA0CTLW0 |= UCSSEL_2 | UCMST | UCSYNC | UCCKPL | UCMSB; // SMCLK, master, sync, clock polarity high, MSB first
    UCA0BR0 = 0x02;                    // Clock divider
    UCA0BR1 = 0;
    UCA0CTLW0 &= ~UCSWRST;            // Release for operation
    PM5CTL0 &= ~LOCKLPM5;             // Unlock GPIO

    while (1) {
        UCA0TXBUF = 0x67;             // Send test byte

        while (!(UCA0IFG & UCRXIFG)); // Wait for packet
        if (UCA0RXBUF == 0x67) {
            // SPI working 
            P1OUT &= ~BIT0; // Red LED is OFF
        } else {
            // SPI not working
             P1OUT |= BIT0; // Red LED is ON
        }

        __delay_cycles(100000);
    }
}
