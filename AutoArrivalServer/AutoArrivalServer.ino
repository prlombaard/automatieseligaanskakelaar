/*
    This sketch shows how to use WiFi event handlers.

    In this example, ESP8266 works in AP mode.
    Three event handlers are demonstrated:
    - station connects to the ESP8266 AP
    - station disconnects from the ESP8266 AP
    - ESP8266 AP receives a probe request from a station

    Written by Markus Sattler, 2015-12-29.
    Updated for new event handlers by Ivan Grokhotkov, 2017-02-02.
    This example is released into public domain,
    or, at your option, CC0 licensed.
*/

#include <ESP8266WiFi.h>
#include <stdio.h>

const char* ssid     = "ap-ssid";
const char* password = "ap-password";
const char* sw_version = "SW Ver 1.1";
//const unsigned long ON_DURATION = 10000;  //10seconds
const unsigned long ON_DURATION = 300000;  //5 minutes
const unsigned long HARDWARE_TEST_CYCLE_DURATION = 5000;  // 5seconds ON and OFF

#define PIN_LIGHT LED_BUILTIN
//#define NODE_MCU_PLATFORM 1
#ifdef NODE_MCU_PLATFORM
#define PIN_RELAY D4
#endif
#ifndef NODE_MCU_PLATFORM
#define PIN_RELAY D7
#endif

String IPHONEMAC = String("dc:a9:04:d8:b2:dc");
String LAPTOPMAC = String("d4:6a:6a:4b:7b:e3");
String NODEMCUMAC = String("5c:cf:7f:29:c0:d9");

String lastMacString;

bool somethingConnected = false;
bool somethingDisconnected = false;
//bool arrived = false;
//bool disembarked = true;
bool lightOn = false;
//bool parked = false;
//bool busyParking = false;

unsigned long arrive_time = 0;
//unsigned long leave_time = 0;

WiFiEventHandler stationConnectedHandler;
WiFiEventHandler stationDisconnectedHandler;
WiFiEventHandler probeRequestPrintHandler;
WiFiEventHandler probeRequestBlinkHandler;

bool blinkFlag;

byte hardware_test() {
  // Test connected hardware
  Serial.println("Testing hardware::START");
  #ifndef NODE_MCU_PLATFORM 
  digitalWrite(PIN_LIGHT, LOW);
  digitalWrite(PIN_RELAY, LOW);
  delay(HARDWARE_TEST_CYCLE_DURATION);
  digitalWrite(PIN_LIGHT, HIGH);
  digitalWrite(PIN_RELAY, HIGH);
  delay(HARDWARE_TEST_CYCLE_DURATION);
  digitalWrite(PIN_LIGHT, LOW);
  digitalWrite(PIN_RELAY, LOW);
  delay(HARDWARE_TEST_CYCLE_DURATION);
  Serial.println("Testing hardware::END");
  #endif
  return 1;
}

void setup() {
  Serial.begin(115200);
  pinMode(PIN_LIGHT, OUTPUT);
  digitalWrite(PIN_LIGHT, HIGH);
  pinMode(PIN_RELAY, OUTPUT);
  digitalWrite(PIN_RELAY, LOW);

  // Don't save WiFi configuration in flash - optional
  WiFi.persistent(false);

  // Set up an access point
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password);

  // Register event handlers.
  // Callback functions will be called as long as these handler objects exist.
  // Call "onStationConnected" each time a station connects
  stationConnectedHandler = WiFi.onSoftAPModeStationConnected(&onStationConnected);
  // Call "onStationDisconnected" each time a station disconnects
  stationDisconnectedHandler = WiFi.onSoftAPModeStationDisconnected(&onStationDisconnected);
  // Call "onProbeRequestPrint" and "onProbeRequestBlink" each time
  // a probe request is received.
  // Former will print MAC address of the station and RSSI to Serial,
  // latter will blink an LED.
  probeRequestPrintHandler = WiFi.onSoftAPModeProbeRequestReceived(&onProbeRequestPrint);
  probeRequestBlinkHandler = WiFi.onSoftAPModeProbeRequestReceived(&onProbeRequestBlink);

  Serial.println("");
  Serial.println("");
  Serial.println("");
  Serial.println("");
  Serial.println(__FILE__);
  #ifdef NODE_MCU_PLATFORM
  Serial.println("This is a NODEMCU PLATFORM");
  #endif
  Serial.println(sw_version);
//  Serial.print("iPhone MAC: ");
//  Serial.println(IPHONEMAC);
//  Serial.print("Laptop MAC: ");
//  Serial.println(LAPTOPMAC);
  if (hardware_test()) {
    Serial.println("Hardware tested OK");
  }
  else
  {
    Serial.println("!!!Hardware tested NOT OK!!!!!!");
  }
  Serial.println("SETUP COMPLETE");
  print_States();
}

void onStationConnected(const WiFiEventSoftAPModeStationConnected& evt) {
  Serial.print("Station connected: ");
  Serial.println(macToString(evt.mac));
  somethingConnected = true;
}

void onStationDisconnected(const WiFiEventSoftAPModeStationDisconnected& evt) {
  Serial.print("Station disconnected: ");
  Serial.println(macToString(evt.mac));
  somethingDisconnected = true;
}

void onProbeRequestPrint(const WiFiEventSoftAPModeProbeRequestReceived& evt) {
  Serial.print("Probe request from: ");
  Serial.print(macToString(evt.mac));
  Serial.print(" RSSI: ");
  Serial.println(evt.rssi);
}

void onProbeRequestBlink(const WiFiEventSoftAPModeProbeRequestReceived&) {
  // We can't use "delay" or other blocking functions in the event handler.
  // Therefore we set a flag here and then check it inside "loop" function.
  blinkFlag = true;
}

void loop() {
  unsigned long the_time = millis();
  if (the_time > 10000 && probeRequestPrintHandler) {
    // After 10 seconds, disable the probe request event handler which prints.
    // Other three event handlers remain active.
    Serial.println("Not printing probe requests any more (LED should still blink)");
    probeRequestPrintHandler = WiFiEventHandler();
  }
  #ifdef NODE_MCU_PLATFORM
  if (blinkFlag) {
    blinkFlag = false;
    digitalWrite(PIN_LIGHT, LOW);
    delay(100);
    digitalWrite(PIN_LIGHT, HIGH);
  }
  #endif
  #ifndef NODE_MCU_PLATFORM
  if (blinkFlag) {
    blinkFlag = false;
    digitalWrite(PIN_LIGHT, HIGH);
    delay(100);
    digitalWrite(PIN_LIGHT, LOW);
  }
  #endif
  if (somethingConnected) {
    Serial.println("Something Connected");
    Serial.println(lastMacString);
    if ((LAPTOPMAC.equals(lastMacString)) || (NODEMCUMAC.equals(lastMacString))) {
      arrive_time = the_time;
      lightOn = true;
      Serial.println("Laptop connected!!!");
      Serial.print("Switching light of in ");
      Serial.print((int)(ON_DURATION / 1000));
      Serial.print(" seconds, ");
      Serial.print((int)(ON_DURATION / 1000 / 60));
      Serial.println(" minutes");
      Serial.println("millis():");
      Serial.println(arrive_time);
    }
    somethingConnected = false;
  }

  if (somethingDisconnected) {
    Serial.println("Something Disconnected");
    Serial.println(lastMacString);
    somethingDisconnected = false;
  }

  if ((lightOn) && ((the_time - arrive_time) > ON_DURATION)) {
    Serial.println("Parked...Switching light off");
    lightOn = false;
    print_States();
  }

  #ifdef NODE_MCU_PLATFORM
  if (lightOn) {
    digitalWrite(PIN_LIGHT, LOW);
    digitalWrite(PIN_RELAY, HIGH);
  }
  else
  {
    digitalWrite(PIN_LIGHT, HIGH);
    digitalWrite(PIN_RELAY, LOW);
  }
  #endif
  #ifndef NODE_MCU_PLATFORM
  if (lightOn) {
    digitalWrite(PIN_LIGHT, HIGH);
    digitalWrite(PIN_RELAY, HIGH);
  }
  else
  {
    digitalWrite(PIN_LIGHT, LOW);
    digitalWrite(PIN_RELAY, LOW);
  }
  #endif

delay(10);
}

String toBool(bool val) {
  if (val) {
    return String("TRUE");
  }
  else
  {
    return String("FALSE");
  }
}

void print_States() {
  Serial.print("lightOn     ");
  Serial.println(toBool(lightOn));
}

String macToString(const unsigned char* mac) {
  char buf[20];
  snprintf(buf, sizeof(buf), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
  lastMacString = String(buf);

  return String(buf);
}
