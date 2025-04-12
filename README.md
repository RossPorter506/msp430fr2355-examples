# MSP430 CCS Code Examples
Various Code Examples for the MSP430FR2355.
TODO: These example projects will be adapted from the forked repo, which uses the MSP430G2553 instead. Projects with 'TODO:' below have not yet been adjusted. 

## List of Examples
| Project Name          | Description   |
| ----------------------|---------------|
| 00-blink              | Blink onboard Red LED
| 01-blinkTogether      | Blink onboard Red and Green LEDs together
| 02-alternateBlink     | Blink onboard Red and Green LEDs in an alternating fashion
| 03-testSwitch         | TODO: Turn ON/OFF onboard LED using onboard Switch
| 04-switchToggle       | TODO: Toggle the onboard LED using the onboard Switch
| 05-switchAlternate    | TODO: Toggle between Red and Green LED using onboard Switch
| 06-switchInterrupt    | TODO: Toggle LED using external interrupt and low power modes 
| 07-blinkWatchdog      | TODO: Illustrates the Watchdog Timer timeout and reset 
| 10-timerBlink         | TODO: Use internal timer and low frequency oscillator to blink LED
| 13-pwmTest            | TODO: Fading effect on onboard Green LED using hardware PWM
| 14-softwarePWM        | TODO: Implementation of PWM using software time delays
| 15-timerCapture       | TODO: Uses the Input Capture mode of Timer to measure external signal period
| 16-adcTest            | TODO: Reads the internal temperature sensor and converts value to Celcius
| 17-dcoTest            | TODO: Cycles through calibrated DCO frequencies during runtime
| 18-adcPWM             | TODO: Use ADC input to control intensity of onboard Green LED
| 19-adcPWM-lpm         | TODO: Implementation of ADC based PWM control using low power modes
| 20-uartEcho           | TODO: Implements UART link at a baud rate of 9600. Echoes RX char + 1
| 21-helloWorld         | TODO: Prints the string "Hello World !" on the UART link
| 22-uartPrintNum       | TODO: Converts an integer value to string and prints it on the UART link
| 23-uartADC            | TODO: Displays the ADC value on the UART link
| 24-spiShift           | TODO: Use SPI protocol to send data to a shift register
| 25-i2cTempSensor      | TODO: Read temperature values from a LM75 sensor over I2C
| 26-bitBangShift       | TODO: Implements SPI communication with shift register by bit banging GPIO
| 27-helloLCD           | TODO: Connects to a 16x2 LCD using 4-bit mode, generic library .c & .h files
