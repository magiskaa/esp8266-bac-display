#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <Arduino_JSON.h>
#include "secrets.h"

float bac = 0.0;
float previous = 0.0;
int bac_rounded_int;

bool initialized = false;
String res;

unsigned long lastBacFetch = 0;
const unsigned long bacInterval = 15000;

bool stableButtonState = HIGH;
bool lastButtonState = HIGH;

const unsigned long debounceMs = 500;
const unsigned long pressCooldownMs = 5000;

unsigned long lastDebounceTime = 0;
unsigned long lastAcceptedPressTime = 0;

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
  
  /* FOR DEBUGGING
  static bool prevRaw = HIGH;
  if (buttonState != prevRaw) {
    Serial.print("RAW button changed to: ");
    Serial.println(buttonState == LOW ? "LOW" : "HIGH");
    prevRaw = buttonState;
  } */

  if (buttonState != lastButtonState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceMs) {
    if (buttonState != stableButtonState) {
      stableButtonState = buttonState;

      if (stableButtonState == LOW && millis() - lastAcceptedPressTime > pressCooldownMs) {
        Serial.println("ACCEPTED BUTTON PRESS");
        lastAcceptedPressTime = millis();
        updateBac(SERVER_NAME_DRINK);
      }
    }
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
