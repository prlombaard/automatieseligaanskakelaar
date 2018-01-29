# automatieseligaanskakelaar
ESP8266 project that enables a light or peripheral to be swtiched on automatically when a certain wifi client device comes in range of the server wifi device

# Background / References
This project was inspired by a project listed on hackaday.com (TODO: Add link to that project). The idea is that a specific programmed device such as an ESP8266 is used as an automatic (remote control) for a garage and light to send when a person is disembarking from the home or returning home.

# Requirements
The following hardware is required:
  * 1 x NodeMCU V1.0 (ESP-12F) - used as client (the device that travels in the car)
  * 1 x ESP8266 12F - used as server (the device that is located in the garage, connected to the garage lights)
  * 1 x USB to serial converter
  * 1 x Battery Pack, that will power the NodeMCU
  
# Setup Instructions (client)
The following steps should be taken to program the various ESP8266 MCUs:
  * Install the latest Arduino IDE
  * Run the ArduinoIDE
  * Install via the board manager the ESP core
  * Connect the NodeMCU via USB cable to the PC
  * Load the AutoLightClient.ino
  * Select the NodeMCU 1.0 board
  * Upload the code
  * Disconnect the NodeMCU

# Setup Instructions (server)
The following steps should be taken to program the various ESP8266 MCUs:
  * Run the ArduinoID
  * Wire-up the ESP8266 as indicated in the fritzing diagram
  * Connect the ESP8266 via USB to serial converter
  * Place the ESP8266 into flash mode
  * Load the AutoLightServer.ino
  * Select the NodeMCU 1.0 board
  * Upload the code
  * Disconnect the NodeMCU, and place it into normal boot mode.
  
# Test
Test the setup by following to following steps:
  * Wire up the relay module. **WARNING** be careful you're working with **high voltages!!!!**
  * **Do not connect the relay yet to mains power**
  * Connect the ESP8266 to the relay
  * Provide power to the ESP8266
  * Connect the relay to mains power and to the lights **be careful**
  * Connect the NodeMCU to the battery power pack
