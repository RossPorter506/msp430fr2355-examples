//***************************************************************************************
//  MSP430 Blink the LED Demo - Software Toggle P1.0
//
//  Description; Toggle P1.0 by xor'ing P1.0 inside of a software loop.
//  ACLK = n/a, MCLK = SMCLK = default DCO
//
//                MSP430x5xx
//             -----------------
//         /|\|              XIN|-
//          | |                 |
//          --|RST          XOUT|-
//            |                 |
//            |             P1.0|-->LED
//
//  J. Stevenson
//  Texas Instruments, Inc
//  July 2011
//  Built with Code Composer Studio v5
//***************************************************************************************

#include <msp430.h>				
#include <stdint.h>

void main(void) {
    // Stop the watchdog timer. We discuss the watchdog in a later example.
    WDTCTL = WDTPW | WDTHOLD;

	// To power the LED we must set pin P1.0 into output pin mode
    // P1DIR is a register that contains the pin direction info for each pin in port 1
    // Each of the pins is represented as a single bit in the register.
    // When a pin's bit is zero, the pin is an input, otherwise it's an output.
    // See PxDIR in the 'MSP430FR4xx and MSP430FR2xx Family User's Guide' for more info.
    // P1DIR = 0b00000000
    //                  ^--- P1.0 direction
    //           ^---------- P1.7 direction
    // Non-destructively write to the least significant bit in the register.
	P1DIR |= 0b00000001; // For learning purposes we write the bit location manually, but later we will use the BITX macros for this.

	//    P1DIR |= 0b00000001
	// => P1DIR = P1DIR | 0b00000001
	// => P1DIR = 0bXXXXXXXX | 0b00000001 (for some unknown bit values X)
	//   0bXXXXXXXX
	// | 0b00000001
	// = 0bXXXXXXX1

	// Once we have configured the GPIO how we want, we can power the GPIO pins by clearing the LOCKLPM5 bit.
	// This is a feature unique to the MSP430 to minimise power consumption. See section 8.3.1 in the user manual for more info.
	PM5CTL0 &= ~LOCKLPM5;

	while(1) {
	    // To control whether an output pin in port 1 is high or low we use P1OUT
	    // Like P1DIR, each bit in P1OUT controls the output level of each pin in port 1.
	    // Changing PxOUT has no effect unless the pin has been configured as an output!!
	    // See PxOUT in the 'MSP430FR4xx and MSP430FR2xx Family User's Guide' for more info.
	    // Toggle P1.0 using exclusive-OR
	    P1OUT ^= 1; // Note that 0b00000001 = 1

        // A crude software-based delay loop.
	    // In later examples we will use a special delay function.
		volatile uint16_t i = 0;	// volatile to prevent compiler optimization
		while(i < 50000) {
            i++;
        }
	}
}
