# MSP430 CCS Code Examples
Various Code Examples for the MSP430FR2355.

TODO: These example projects will be adapted from the forked repo, which uses the MSP430G2553 instead. Projects with 'TODO:' below have not yet been adjusted. Projects with 'TEST:' have not yet been tested. Projects with 'DOC:' have been tested to work, but probably need a second pair of eyes to add explanations where necessary. 

TODO: Move all projects to C99 mode

TODO: Renumber projects. Rename bitBangShift to SpiBitBangShift and move it to before the spiShift project

TODO: Explain `__bis_SR_register()` and `__bic_SR_register()` intrinsics, how it's a CPU register and thus doesn't have a memory address that can be written to like memory-mapped peripherals

## List of Examples
| Project Name          | Description   |
| ----------------------|---------------|
| 00-blink              | DOCS: Blink onboard Red LED
| 01-blinkTogether      | DOCS: Blink onboard Red and Green LEDs together
| 02-alternateBlink     | DOCS: Blink onboard Red and Green LEDs in an alternating fashion
| 03-testSwitch         | DOCS: Turn ON/OFF onboard LED using onboard Switch
| 04-switchToggle       | DOCS: Toggle the onboard LED using the onboard Switch
| 05-switchAlternate    | DOCS: Toggle between Red and Green LED using onboard Switch
| 06-switchInterrupt    | DOCS: Toggle LED using external interrupt and low power modes 
| 07-blinkWatchdog      | DOCS: Illustrates the Watchdog Timer timeout and reset 
| 10-timerBlink         | DOCS: Use internal timer and low frequency oscillator to blink LED
| 13-softwarePWM        | DOCS: Implementation of PWM using software time delays
| 14-pwmTest            | DOCS: Fading effect on onboard Green LED using hardware PWM
| 15-timerCapture       | TEST: Uses the Input Capture mode of Timer to measure external signal period
| 16-adcTest            | DOCS: Reads the internal temperature sensor and converts value to Celcius
| 17-dcoTest            | DOCS: Cycles through calibrated DCO frequencies during runtime
| 18-adcPWM             | DOCS: Use ADC input to control intensity of onboard Green LED
| 19-adcPWM-lpm         | DOCS: Implementation of ADC based PWM control using low power modes
| 20-uartEcho           | DOCS: Implements UART link at a baud rate of 9600. Echoes RX char + 1
| 21-helloWorld         | DOCS: Prints the string "Hello World !" on the UART link
| 22-uartPrintNum       | DOCS: Converts an integer value to string and prints it on the UART link
| 23-uartADC            | DOCS: Displays the ADC value on the UART link
| 24-spiShift           | TEST: Use SPI protocol to send data to a shift register
| 25-i2cTempSensor      | TEST: Read temperature values from a LM75 sensor over I2C
| 26-bitBangShift       | TEST: Implements SPI communication with shift register by bit banging GPIO
| 27-helloLCD           | TEST: Connects to a 16x2 LCD using 4-bit mode, generic library .c & .h files
