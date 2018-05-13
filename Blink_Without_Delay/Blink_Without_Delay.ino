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

void setup() {
  pinMode(led, OUTPUT);
}

void loop() {
  blinkLed();
}
