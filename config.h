/************************************************
* User Config
*/
// Outputs
#define speedPin 11
#define inclineUpPin 9
#define inclineDownPin 10

// Inputs
#define speedPotPin A2
#define inclineUpBtn 4  // Buttons should be high by default, low when pushed
#define inclineDownBtn 3
#define inclineBtnInvert  // Uncomment if the button is low when pressed. Comment if button is high when pressed.
// #define inclineSensePin 8 // switches on/off every step of incline. Not implemented yet.
#define safetyKeyPin -1  // Safety key is a pin that outputs 3-5v when the key is inserted/attached. -1 to disable.

// Max duty cycle defines max treadmill speed
#define maxDutyCycle 0.55
// Speed input minimum threshold is the minimum analog value that will be required to start the treadmill. 0-1023.
#define speedAnalogThreshold 40

// PWM timing
#define hertz 20

/************************************************
* End User Config
*************************************************/