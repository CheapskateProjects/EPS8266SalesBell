/*
  ESP8266SalesBell

  This project is wlan connected sales bell that can be ringed remotely. 
  /1 : X1
  /2 : X2
  /3 : X3

  Gpio2 pin is used to activate the bell. You should build your electronics accordingly. 

  created   Feb 2017
  by CheapskateProjects

  ---------------------------
  The MIT License (MIT)

  Copyright (c) 2017 CheapskateProjects

  Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

// WLAN configuration
const char* ssid = "XXX";
const char* password = "YYY";

// Constants that can be changed based on your hardware or preferences
const int delayBetweenRingsMillis = 1000;
const int ringOnTimeMillis = 20;
const int relayPin = 2;

ESP8266WebServer server(80);


// Function for handling the actual ringing.
void ringBell(int times)
{
  String message = "Ringing sales bell!";
  server.send(200, "text/plain", message);
  for(int i = 0; i < times; i++)
  {
    digitalWrite(relayPin, 1);
    delay(ringOnTimeMillis);
    digitalWrite(relayPin, 0);
    delay(delayBetweenRingsMillis);
  }
}

// Basic error message for unknown urls
void handleNotFound()
{
  server.send(404, "text/plain", "File Not Found");
}

// Init output pin
void initOutput()
{
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, 0);
}

// Init wlan connection
void initWLAN()
{
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
  }
}

void setup(void)
{
  initOutput();
  initWLAN();
  server.on("/1", [](){ringBell(1);});
  server.on("/2", [](){ringBell(2);});
  server.on("/3", [](){ringBell(3);});
  server.onNotFound(handleNotFound);
  server.begin();
}

void loop(void)
{
  server.handleClient();
}
