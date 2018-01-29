/*
    This sketch sends data via HTTP GET requests to data.sparkfun.com service.

    You need to get streamId and privateKey at data.sparkfun.com and paste them
    below. Or just customize this script to talk to other HTTP servers.

*/

#include <ESP8266WiFi.h>

const char* ssid     = "ap-ssid";
const char* password = "ap-password";
const char* sw_version = "SW Ver 1.0";
const unsigned long HARDWARE_TEST_CYCLE_DURATION = 2500;  // 2.5 seconds ON and OFF

#define PIN_LIGHT LED_BUILTIN
#define NODE_MCU_PLATFORM 1



bool client_connected_to_server = false;

byte hardware_test() {
  // Test connected hardware
  Serial.println("Testing hardware::START");
#ifdef NODE_MCU_PLATFORM
  digitalWrite(PIN_LIGHT, HIGH);
  //  digitalWrite(PIN_RELAY, LOW);
  delay(HARDWARE_TEST_CYCLE_DURATION);
  digitalWrite(PIN_LIGHT, LOW);
  //  digitalWrite(PIN_RELAY, HIGH);
  delay(HARDWARE_TEST_CYCLE_DURATION);
  digitalWrite(PIN_LIGHT, HIGH);
  //  digitalWrite(PIN_RELAY, LOW);
  delay(HARDWARE_TEST_CYCLE_DURATION);
  Serial.println("Testing hardware::END");
#endif
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

void setup_wifi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(PIN_LIGHT, LOW);
    delay(100);
    digitalWrite(PIN_LIGHT, HIGH);
    delay(100);
    Serial.print(",");
  }

  client_connected_to_server = true;
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}


void setup() {
  Serial.begin(115200);
  delay(250);

  pinMode(PIN_LIGHT, OUTPUT);
  digitalWrite(PIN_LIGHT, HIGH);

  Serial.println();
  Serial.println();
  Serial.println();
  Serial.println();
  Serial.println();

  // We start by connecting to a WiFi network
  setup_wifi();
  if (hardware_test()) {
    Serial.println("Hardware tested OK");
  }
  else
  {
    Serial.println("!!!Hardware tested NOT OK!!!!!!");
  }

  Serial.println(sw_version);

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  /* Explicitly set the ESP8266 to be a WiFi-client, otherwise, it by default,
     would try to act as both a client and an access-point and could cause
     network-issues with your other WiFi-devices on your WiFi-network. */

  Serial.println("setup() COMPLETE");
}

int value = 0;

void loop() {
  Serial.println("Wifi Status:");
  Serial.println(WiFi.status());
  if (WiFi.status() != WL_CONNECTED) {
    // Just disconnect
    Serial.println("Disconnecting from AP");
    WiFi.disconnect();
    Serial.println("Disconnected from AP");
    client_connected_to_server = false;
    Serial.println("Calling set_wifi");
    setup_wifi();  // THIS IS A BLOCKING CALL
    Serial.println("Returned from set_wifi()");
    }
  else
  {
    client_connected_to_server = true;
  }
   delay(500);
  if (client_connected_to_server) {
#ifdef NODE_MCU_PLATFORM
    digitalWrite(PIN_LIGHT, LOW);
    delay(1000);
    digitalWrite(PIN_LIGHT, HIGH);
    delay(1000);
#endif
  }
}
