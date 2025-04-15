#include <msp430.h>

void main(void) {
    // Note we don't disable the watchdog timer in this project.

    P1DIR |= BIT0;          // Set P1.0 to output

    P1OUT ^= BIT0;          // Toggle P1.0

    PM5CTL0 &= ~LOCKLPM5;   // Unlock GPIO
    
    // The code will enter this loop, but then after ~30ms (see watchdog section in user manual) the watchdog will
    // trigger and reset the MCU, restarting execution from the start of main()
    while(1);
}
