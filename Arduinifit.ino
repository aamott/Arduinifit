/****************************************************************
 *   __    ____  ____  __  __  ____  _  _  ____  ____  ____  ____
 *  /__\  (  _ \(  _ \(  )(  )(_  _)( \( )(_  _)( ___)(_  _)(_  _)
 * /(__)\  )   / )(_) ))(__)(  _)(_  )  (  _)(_  )__)  _)(_   )(
 *(__)(__)(_)\_)(____/(______)(____)(_)\_)(____)(__)  (____) (__)
 * Arduino Treadmill Firmware. Implements a new control panel for
 * the ProForm Pro 5000 and other similar treadmills. Controls
 * speed and incline.
 *
 * Pinout of the ProForm Pro 5000
 * -------------------------------------------------
 * Treadmill   Arduino   Use
 * -------------------------------------------------
 * Purple      D8        Incline Sense
 * Orange      D9        Incline Up
 * Yellow      D10       Incline Down
 * Blue        D11       Speed
 * Green       --        Tachometer (not installed)
 * Red         VIN       +12v
 * Black       Gnd       Ground
 *
 * Controls pinout
 * -------------------------------------------------
 * Black       Gnd       Ground
 * Yellow      A2        Set Speed
 * Blue        D3        Incline Down
 * Green       D4        Incline Up
 *
 * Code Explanation
 * --------------------------------------------------
 * The main loop runs at 20hz (configurable), checking for inputs along the way.
 * Incline is controlled by the incline up and down buttons, speed is controlled by
 * the potentiometer.
 ****************************************************************/

#include "config.h"

#ifdef inclineBtnInvert
#define invert !
#else
#define invert
#endif

const int totalMillisPerLoop = 1000 / hertz;  // how long each pwm cycle should last. Duty cycle is defined by how much of this time the pin is on or off.
unsigned long onTime = 0;
bool speedPinState = LOW;
unsigned long currentTime;
unsigned long previousTime = 0;
unsigned long previousOnTime = 0;
bool safetyTriggered;

void setup() {
  // Outputs
  pinMode(speedPin, OUTPUT);
  pinMode(inclineUpPin, OUTPUT);
  pinMode(inclineDownPin, OUTPUT);

  // Inputs
  // pinMode(inclineSensePin, INPUT_PULLUP); // incline not implemented yet
  pinMode(speedPotPin, INPUT);
  pinMode(inclineUpBtn, INPUT_PULLUP);
  pinMode(inclineDownBtn, INPUT_PULLUP);

  safetyTriggered = false;
}

void loop() {
  currentTime = millis();

  if ((safetyKeyPin == -1 || digitalRead(safetyKeyPin) == HIGH ) && !safetyTriggered) {
    // Safety key is not installed or is not pressed.
    // Run the treadmill normally
    runTreadmill();
  } else if (safetyTriggered) {  // safety has been triggered
    // require speed to be set to 0 to restart
    if (analogRead(speedPotPin) < speedAnalogThreshold) {
      // reset safety
      safetyTriggered = false;
    }
  } else {
    // Safety key is installed and is pressed.
    // Stop the treadmill
    digitalWrite(speedPin, LOW);
    digitalWrite(inclineUpPin, false);
    digitalWrite(inclineDownPin, false);
    safetyTriggered = true;
  }
}

/***********************
 * runTreadmill
 * main treadmilll loop
*/
void runTreadmill() {
  // Incline control - when buttons are pressed, state is low
  if (invert digitalRead(inclineUpBtn) && invert !digitalRead(inclineDownBtn)) {  // Up Pressed, down released
    digitalWrite(inclineUpPin, true);
    digitalWrite(inclineDownPin, false);
  }
  // incline Down pressed
  else if (invert digitalRead(inclineDownBtn) && invert !digitalRead(inclineUpBtn)) {  // down pressed, up released
    digitalWrite(inclineUpPin, false);
    digitalWrite(inclineDownPin, true);
  } else {  // no buttons pressed
    digitalWrite(inclineUpPin, false);
    digitalWrite(inclineDownPin, false);
  }

  // Speed control
  int speedPotValue = analogRead(speedPotPin);

  if (speedPotValue > speedAnalogThreshold) {  // noise cutoff

    // Duty Cycle. A loop should last totalMillisPerLoop milliseconds. The pin should be HIGH for onTime milliseconds.
    if (currentTime - previousTime <= onTime && speedPinState == LOW) {
      digitalWrite(speedPin, HIGH);
      speedPinState = HIGH;
    } else if (currentTime - previousTime > onTime && speedPinState == HIGH) {
      digitalWrite(speedPin, LOW);
      speedPinState = LOW;
    } else if (currentTime - previousTime >= totalMillisPerLoop) {  // reset loop
      digitalWrite(speedPin, LOW);
      speedPinState = LOW;
      previousTime = currentTime;

      // update Pot once per duty cycle
      onTime = map(speedPotValue, 0, 1023, 5, totalMillisPerLoop * maxDutyCycle);
    }
  }
}