# Arduinifit
Arduinifit is a firmware to control the iFit Proform Pro 5000 (and probably other) treadmills using the original motor drivers. 

# About
The iFit Proform Pro 5000 is made of 3 primary components:
- Motor hardware (elevation and speed) 
- Motor driver (same driver for both)
- Controller
- Buttons
- Tablet

The tablet is most likely to fail since it receives periodic updates and they want you to "upgrade" (is a second running machine really an upgrade though?) as frequently as possible. The controller board communicates with it over UART, but I haven't been able to decipher its inner protocols. Most of the buttons are wired to it in a matrix, so it will be hard to reuse them without the UART connection. The motor driver is controlled over a few lines:
* Black and White   Ground
* Purple            Incline Sense   - pulses every rotation of the incline screw. Note that this is not absolute positioning. 
* Orange            Incline Up      - turns on when 5v is applied *and* Incline Down is pulled to ground
* Yellow            Incline Down    - turns on when 5v is applied *and* Incline Up is pulled to ground
* Blue              Speed           - PWM at 20hz *exactly* (19.5hz to 20.5hz)
* Green             Tachometer (not installed on this treadmill)
* Red               +12v
* Black             Ground

It should be noted that Incline Sense isn't absolute positioning, so I can only guess that the original tablet kept track of the incline's position and saved it to memory for when it was turned off. The Arduino doesn't have memory, so this won't work for us. For now, just take note when it stops moving. 


# Installation
Open the file in the Arduino IDE and upload to any Arduino. There are no specific pin requirements (such as PWM) so you can change the wiring as needed. 

## Wiring
For now, pinout is written in a comment in the code. I'll move it into here in the future. 

