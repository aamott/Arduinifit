/****************************************************************
*   __    ____  ____  __  __  ____  _  _  ____  ____  ____  ____ 
*  /__\  (  _ \(  _ \(  )(  )(_  _)( \( )(_  _)( ___)(_  _)(_  _)
* /(__)\  )   / )(_) ))(__)(  _)(_  )  (  _)(_  )__)  _)(_   )(  
*(__)(__)(_)\_)(____/(______)(____)(_)\_)(____)(__)  (____) (__) 
* Arduino Treadmill Firmware
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
****************************************************************/


// outputs
#define speedPin 11
#define inclineUpPin 9
#define inclineDownPin 10
// inputs
#define inclineSensePin 8
#define potPin A2
#define inclineUpBtn 4
#define inclineDownBtn 3

#define maxDutyCycle 0.35
int potValue = 0;


// Timing
const int hertz = 20;
const int totalMillisPerLoop = 1000/hertz;


void setup() {
  // Outputs
  pinMode(speedPin, OUTPUT);
  TCCR2B = TCCR2B & B11111000 | B00000110;
  pinMode(inclineUpPin, OUTPUT);
  pinMode(inclineDownPin, OUTPUT); 

  // Inputs
  pinMode(inclineSensePin, INPUT_PULLUP); // not currently used
  pinMode(potPin, INPUT);
  pinMode(inclineUpBtn, INPUT_PULLUP);
  pinMode(inclineDownBtn, INPUT_PULLUP);
}


void loop() {
  // Incline
  // if up button and not down
  // incline Up Pressed (low on pressed, high on open)
  if (!digitalRead(inclineUpBtn)) {
    //    set up pin, disable down pin
    digitalWrite(inclineUpPin, true);
    digitalWrite(inclineDownPin, false);
  }
  // incline Down pressed
  else if (!digitalRead(inclineDownBtn)) {
    //    set up pin, disable down pin
    digitalWrite(inclineUpPin, false);
    digitalWrite(inclineDownPin, true);
  }
  else {
    digitalWrite(inclineUpPin, false);
    digitalWrite(inclineDownPin, false);
  }

  // manage speed
  int potValue = analogRead(potPin);
  if (potValue > 40) {
    int onMillis = map(potValue, 0, 1023, 5, totalMillisPerLoop * maxDutyCycle);
    digitalWrite(speedPin, HIGH);
    delay(onMillis);
    digitalWrite(speedPin, LOW);
    delay(totalMillisPerLoop - onMillis);
  }
}