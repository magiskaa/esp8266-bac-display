#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <Arduino_JSON.h>
#include "secrets.h"

#define GREEN 14
#define RED 12

float bac = 0.0;
float previous = 0.0;
bool initialized = false;
int bac_rounded_int;

void setup() {
  Serial.begin(9600);
  
  WiFi.begin(SSID, PASSWORD);
  WiFi.setSleep(false);
  WiFi.setAutoReconnect(true);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(WiFi.localIP());

  pinMode(GREEN, OUTPUT);
  digitalWrite(GREEN, LOW);
  pinMode(RED, OUTPUT);
  digitalWrite(RED, LOW);
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    String url = String(SERVER_NAME) + "?id=" + USER_ID;
    String s = httpGETRequest(url);
    s.trim();
    bac = s.toFloat();
  }

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

  delay(1000);
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
