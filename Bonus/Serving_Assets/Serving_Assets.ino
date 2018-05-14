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

String getContentType(String filename) {
  if (filename.endsWith(".html")) {
    return "text/html";
  } else if (filename.endsWith(".css")) {
    return "text/css";
  } else if (filename.endsWith(".js")) {
    return "application/javascript";
  } else if (filename.endsWith(".ico")) {
    return "image/x-icon";
  } else {
    return "text/plain";
  }
}

bool handleAssets(String path) {
  if (!path.startsWith("/assets")) {
    return false;
  }

  if (!SPIFFS.exists(path)) {
    return false;
  }

  String contentType = getContentType(path);
  File file = SPIFFS.open(path, "r");
  server.streamFile(file, contentType);
  file.close();
  return true;
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

  server.onNotFound([]() {
    if (!handleAssets(server.uri())) {
      server.send(404, "text/plain", "404 Not Found");
    }
  });
  
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
