IoT Intro
=========

This is a repo to help get setup with the ESP8266 and create an IoT device.

**Mac users**

You need this this driver maybe

[https://github.com/adrianmihalko/ch340g-ch34g-ch34x-mac-os-x-driver](https://github.com/adrianmihalko/ch340g-ch34g-ch34x-mac-os-x-driver)

Protip: use the homebrew version


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

Here the different code:

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

## Serving_Assets

Code for being able to serve assets without defining each route.

The result is being able to add `somefile.js` to `{sketch}/data/assets`, and
have requests to `/assets/somefile.js` work without having to define that route
in the Arduino code.

Here's the new code:

```cpp
void setupServer() {
  // ...

  server.onNotFound([]() {
    if (!handleAssets(server.uri())) {
      server.send(404, "text/plain", "404 Not Found");
    }
  });

  // ...
}
```

First with our other routes, add `onNotFound` route.
This will run on any route that isn't explicitly defined.
It runs a lambda function similar to `() => { ... }` in JavaScript.

When `handleAssets` returns false (the asset doesn't exist), we'll return 404.

```cpp
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
```

`handleAssets` will first check to make sure the request is formatted like
`/assets/:file`, but removing this check will let you serve files at any path.

Next, check that the file actually exists.

If it does, we'll get the content type (code below), open it in read mode,
stream the file, close it then return true.

```cpp
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
```

`getContentType` just checks for what MIME type to use with the reponse.
It just checks what the extension is, and returns the corresponding MIME type.

## WiFiManager

This example shows how you can use the WiFiManager library to not have to hard
code the WiFi credentials.

This will make it so if your ESP8266 isn't connected / can't connect to a
network, it will create an access point that, when connected to, will let you
connect to a network more intuitively.

[https://github.com/tzapu/WiFiManager](https://github.com/tzapu/WiFiManager)

*Note, in the IDE, use build removing wifi settings, else it won't be different*

After installing the WifiManager library, here is all that is done.

```cpp
#include <WiFiManager.h>

void setup() {
  // ...

  WiFiManager wifiManager;
  wifiManager.autoConnect("ESPConnect", "ESPConnectPassword");

  // ...
}
```

Replace `connectToWiFi()` in `setup` with this.
(You can delete the whole function too.)
