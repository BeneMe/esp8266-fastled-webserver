#include "WifiMgr.h"

extern ESP8266WebServer espWebServer;


WifiMgr::WifiMgr(Settings &settings)
: settings(settings) {

}

template <typename Generic> void WifiMgr::debugPrintln(Generic text) {
  if (DEBUG_WIFI) {
    Serial.print("*WiFi: ");
    Serial.println(text);
  }
}

String WifiMgr::getWiFiJson() {
  String hostname = String(HOSTNAME);
  hostname += String(ESP.getChipId(), HEX);

  String json = "{";

  json += "\"status\":\"" + String(WiFi.status()) + "\"";
  json += ",\"localIP\":\"" + WiFi.localIP().toString() + "\"";
  json += ",\"softAPIP\":\"" + WiFi.softAPIP().toString() + "\"";
  json += ",\"hostname\":\"" + hostname + "\"";
  json += ",\"ssid\":\"" + WiFi.SSID() + "\"";
  json += ",\"rssi\":\"" + String(WiFi.RSSI()) + "\"";

  //  json += ",\"networks\":[";
  //  byte ssidCount = WiFi.scanNetworks();
  //  for (byte i = 0; i < ssidCount; i++) {
  //    if (i > 0)
  //      json += ",";
  //
  //    json += "{\"name\":\"" + WiFi.SSID(i) + "\",\"rssi\":\"" + String(WiFi.RSSI(i)) + "\"}";
  //  }
  //
  //  json += "]";

  json += "}";

  return json;
}

void WifiMgr::initializeWiFi() {
  WiFi.mode(WIFI_STA);
  Serial.printf("Connecting to %s\n", ssid);
  if (String(WiFi.SSID()) != String(ssid)) {
    WiFi.begin(ssid, password);
  }

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    if(settings.leds[75].r == 255) {
      settings.leds[75].r = 0;
    } else {
      settings.leds[75].r = 255;
    }
    settings.leds[75].g = 0;
    settings.leds[75].b = 0;
    Serial.print(".");
    FastLED.show();
  }

  Serial.print("Connected! Open http://");
  Serial.print(WiFi.localIP());
  Serial.println(" in your browser");
  
  /*WiFi.mode(WIFI_AP_STA);

  // Set Hostname.
  String hostname = String(HOSTNAME);
  hostname += String(ESP.getChipId(), HEX);
  WiFi.hostname(hostname);

  // Print hostname.
  Serial.println("Hostname: " + hostname);

  char hostnameChar[hostname.length() + 1];
  memset(hostnameChar, 0, hostname.length() + 1);

  for (uint8_t i = 0; i < hostname.length(); i++)
    hostnameChar[i] = hostname.charAt(i);

  //  MDNS.begin(hostnameChar);

  // Add service to MDNS-SD
  //  MDNS.addService("http", "tcp", 80);

  // attempt to connect; should it fail, fall back to AP mode
  //  WiFi.mode(WIFI_STA);

  String stored_ssid = WiFi.SSID();
  if (stored_ssid != NULL && stored_ssid != "") {
    debugPrintln("Connecting to stored SSID:");
    debugPrintln(stored_ssid);
    WiFi.begin();
  } else {
    debugPrintln("No stored SSID");
  }

  startAp();

  espWebServer.on("/wifi", HTTP_POST, [this]() {
    String ssid = espWebServer.arg("ssid");
    String password = espWebServer.arg("password");
    // String mode = webServer.arg("mode");

    char ssidChars[50];
    ssid.toCharArray(ssidChars, 50);

    char passwordChars[50];
    password.toCharArray(passwordChars, 50);

    debugPrintln("Connecting to new SSID:");
    debugPrintln(ssid);

    // dnsServer.stop();
    // WiFi.softAPdisconnect(true);

    // apMode = false;
    // WiFi.mode(WIFI_STA);
    WiFi.begin(ssidChars, passwordChars);
    // futureTimeout = millis() + connectionTimeout;

    espWebServer.sendHeader("Location", "/wifi.htm");
    espWebServer.send(303);
  });

  espWebServer.on("/wifi", HTTP_GET, [this]() {
    String json = getWiFiJson();
    espWebServer.send(200, "application/json", json);
  }); */
}

void WifiMgr::checkWiFi() {
  //  if (WiFi.status() == WL_CONNECTED) {
  //    debugPrintln("connected");
  //    futureTimeout = millis() + connectionTimeout;
  //    return;
  //  }
  //
  //  if (apMode) {
  //    debugPrintln("Already running in AP mode.");
  //    return;
  //  }
  //
  //  // time to give up on the stored network and switch to ap mode?
  //  if (futureTimeout != 0 && millis() < futureTimeout) {
  //    return;
  //  }
  //
  //  debugPrintln("Switching to AP mode, timeout elapsed: ");
  //  debugPrintln(connectionTimeout);
  //
  //  startApMode();
}

