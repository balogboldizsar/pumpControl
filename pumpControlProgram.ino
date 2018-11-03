#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ArduinoOTA.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <FS.h> //file system wrapper
#include <WebSocketsServer.h>
#include "pumpControlProgramHeader.h"

/*__________________________________________________________SETUP__________________________________________________________*/

void setup() {  
  Serial.begin(115200);        // Start the Serial communication to send messages to the computer
  delay(10);
  Serial.println("\r\n");
  
  startWiFi();                 // Start a Wi-Fi access point, and try to connect to some given access points. Then wait for either an AP or STA connection
  startOTA();                  // Start the OTA service
  startSPIFFS();               // Start the SPIFFS and list all contents
  startWebSocket();            // Start a WebSocket server
  startMDNS();                 // Start the mDNS responder
  startServer();               // Start a HTTP server with a file read handler and an upload handler
  
  pinMode(flowRateSensorPin, INPUT_PULLUP); //initializes the flowRateSensorPin as pullup input
  attachInterrupt(digitalPinToInterrupt(flowRateSensorPin), flowRateInterrupt, FALLING); //an interrupt is attached needed to measure the flow rate

}

/*__________________________________________________________LOOP__________________________________________________________*/

void loop() {
  webSocket.loop();                           // constantly check for websocket events
  
  //schedule measurements and WebSocket messages
  if (millis() > lastPressureTime + pressureMeasureFreq) {	
    measurePressure();
    lastPressureTime = millis();
  }
  if (millis() > lastPressureMessageTime + messageFreq) {
    measurementsToWebpage();
    lastPressureMessageTime = millis();
  }
  if (millis() > lastFlowRateTime + flowRateMeasureFreq) {
    measureFlowRate();
    lastFlowRateTime = millis();
  }
  if (millis() > lastFlowRateMessageTime + messageFreq) {
    measurementsToWebpage();
    lastFlowRateMessageTime = millis();
  }
  
  server.handleClient();                      // run the server
  ArduinoOTA.handle();                        // listen for OTA events
}
