IoT Intro
=========

This is a repo to help get setup with the ESP8266 and create an IoT device.

# Order

The Arduino IDE doesn't allow sketches to start with a number.
So here's the order the example code progresses, and a bit about what they do.

1. Blink - Gets your Arduino blinking to make sure it works
2. Blink_Without_Delay - Blink, but without using `delay` because it's blocking
3. Connect_to_WiFi - Connect to your WiFi
4. API - Build a blink API
5. Serving_HTML - Serve a page that uses the API

# Links

Links to documentation for things used

[ESP8266 Arduino](https://github.com/esp8266/Arduino)

[ArduinoJson](https://github.com/bblanchon/ArduinoJson)

[ESP8266 file system doc](http://esp8266.github.io/Arduino/versions/2.0.0/doc/filesystem.html)

# Bonus Code

In the Bonus directory, you can find examples that build off Serving_HTML.
They are examples for things that might be useful to you, and do not build off
of each other.

## Local_Domain

Example of using mDNS to setup a .local domain for the ESP8266.

Here the different code.

```cpp
#include <ESP8266mDNS.h>

void setup() {
  // ...
  MDNS.begin("iotblink");
  // ...
}
```

All this does is include the mDNS library, and setup the domain name.
The server should now be able to be accessed at `iotblink.local`.

I read some things that indicated this won't work for Android, but I wasn't able
to try.
