#include <ESP8266WiFi.h>

const int led = 2;

int ledState = LOW;     
unsigned long previousMillis = 0;
long blinkInterval = 1000;

void blinkLed() {
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= blinkInterval) {
    previousMillis = currentMillis;   

    if (ledState == LOW)
      ledState = HIGH;
    else
      ledState = LOW;

    digitalWrite(led, ledState);
  }
}

void connectToWiFi() {
  WiFi.begin("SSID", "PASSWORD.");

  Serial.println("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.print("Local IP Address: ");
  Serial.println(WiFi.localIP());
}

void setup() {
  Serial.begin(115200);
  connectToWiFi();
  
  pinMode(led, OUTPUT);
}

void loop() {
  blinkLed();
}
