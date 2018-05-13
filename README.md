IoT Intro
=========

This is a repo to help get setup with the ESP8266 and create an IoT device.

## Order

The Arduino IDE doesn't allow sketches to start with a number.
So here's the order the example code progresses, and a bit about what they do.

1. Blink - Gets your Arduino blinking to make sure it works
2. Blink_Without_Delay - Blink, but without using `delay` because it's blocking
3. Connect_to_WiFi - Connect to your WiFi
4. API - Build a blink API
5. Serving_HTML - Serve a page that uses the API
