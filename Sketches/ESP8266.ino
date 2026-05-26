#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <Arduino_JSON.h>
#include "secrets.h"

#define GREEN 14
#define RED 12
#define BUTTON 4

float bac = 0.0;
float previous = 0.0;
bool initialized = false;
int bac_rounded_int;
String res;

bool lastButtonState = HIGH;
unsigned long lastBacFetch = 0;
const unsigned long bacInterval = 3000;

void setup() {
  Serial.begin(9600);
  
  pinMode(GREEN, OUTPUT);
  digitalWrite(GREEN, LOW);
  pinMode(RED, OUTPUT);
  digitalWrite(RED, LOW);
  pinMode(BUTTON, INPUT_PULLUP);
  
  Serial.println("Connecting to wifi...");
  WiFi.begin(SSID, PASSWORD);
  WiFi.setSleep(false);
  WiFi.setAutoReconnect(true);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(WiFi.localIP());
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    return;
  }
  
  bool buttonState = digitalRead(BUTTON);
  if (lastButtonState == HIGH && buttonState == LOW) {
    updateBac(SERVER_NAME_DRINK);
  }
  lastButtonState = buttonState;

  unsigned long now = millis();
  if (now - lastBacFetch >= bacInterval) {
    lastBacFetch = now;
    updateBac(SERVER_NAME_BAC);
  }
}

void updateBac(const String& serverName) {
  String url = String(serverName) + "?id=" + userId;
  String res = httpGETRequest(url);
  res.trim();
  
  bac = res.toFloat();
  bac_rounded_int = (int)(bac * 1000.0 + 0.5);
  
  char buf[6];
  snprintf(buf, sizeof(buf), "%04d", bac_rounded_int);
  Serial.println(buf);
  
  if (!initialized) {
    previous = bac;
    initialized = true;
  } else {
    if (previous < bac) {
      digitalWrite(GREEN, HIGH);
      digitalWrite(RED, LOW);
    } else if (previous > bac) {
      digitalWrite(GREEN, LOW);
      digitalWrite(RED, HIGH);
    }
    previous = bac;
  }
}

String httpGETRequest(const String& url) {
  WiFiClient client;
  HTTPClient http;

  http.setReuse(false);
  http.setTimeout(4000);

  if (!http.begin(client, url)) {
    return "";
  }

  int httpResponseCode = http.GET();

  String payload = "";
  if (httpResponseCode > 0) {
    payload = http.getString();
  } else {
    Serial.printf(
      "GET %s => %d (%s)\n",
      url.c_str(),
      httpResponseCode,
      http.errorToString(httpResponseCode).c_str()
    );
  }

  http.end();
  return payload;
}
