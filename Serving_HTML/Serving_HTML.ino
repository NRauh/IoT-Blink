#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ArduinoJson.h>
#include <FS.h>

ESP8266WebServer server(80);
const int led = 2;

bool ledBlinking = false;

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
  WiFi.begin("SSID", "PASSWORD");

  Serial.println("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.print("Local IP Address: ");
  Serial.println(WiFi.localIP());
}

void handleIndex() {
  File index = SPIFFS.open("/index.html", "r");
  if (!index) {
    server.send(500, "text/html", "Unable to serve page\n");
    return;
  }

  server.streamFile(index, "text/html");
  index.close();
}

void handleBlink() {
  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(server.arg("plain"));
 
  if (root.containsKey("blink")) {
    ledBlinking = root["blink"];
  }

  if (root.containsKey("blinkRate")) {
    blinkInterval = root["blinkRate"];
  }

  server.send(200, "text/plain", "I have blinked\n");
}

void setupServer() {
  server.on("/", HTTP_GET, handleIndex);
  server.on("/blink", HTTP_PATCH, handleBlink);
  server.begin();
}

void setup() {
  SPIFFS.begin();
  Serial.begin(115200);
  connectToWiFi();
  
  pinMode(led, OUTPUT);

  setupServer();
}

void loop() {
  server.handleClient();

  if (ledBlinking) {
    blinkLed();
  }
}
