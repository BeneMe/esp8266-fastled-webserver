#include "WifiMgr.h"

WifiMgr::WifiMgr(Settings &settings, WebServer& webServer)
: settings(settings), webServer(webServer) {

}

template <typename Generic> void WifiMgr::debugPrintln(Generic text) {
  if (DEBUG_WIFI) {
    Serial.print("*WiFi: ");
    Serial.println(text);
  }
}

void WifiMgr::startAp() {
  // WiFi.disconnect();

  // apMode = true;

  //  WiFi.mode(WIFI_AP_STA);
  // debugPrintln("SET AP STA");

  String AP_NameString = "ESP8266-";
  AP_NameString += String(ESP.getChipId(), HEX);

  char AP_NameChar[AP_NameString.length() + 1];
  memset(AP_NameChar, 0, AP_NameString.length() + 1);

  for (unsigned int i = 0; i < AP_NameString.length(); i++)
    AP_NameChar[i] = AP_NameString.charAt(i);

  debugPrintln("Starting soft AP");

  if (WiFiAPPSK != NULL) {
    debugPrintln(WiFi.softAP(AP_NameChar, WiFiAPPSK) ? "ready" : "failed");
  } else {
    debugPrintln(WiFi.softAP(AP_NameChar) ? "ready" : "failed");
  }

  debugPrintln("Connect to Wi-Fi access point: ");
  debugPrintln(AP_NameChar);

  delay(500); // Without delay I've seen the IP address blank
  debugPrintln("AP IP address: ");
  debugPrintln(WiFi.softAPIP());

  //  dnsServer.setErrorReplyCode(DNSReplyCode::NoError);
  //  dnsServer.start(DNS_PORT, "*", WiFi.softAPIP());
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
  WiFi.mode(WIFI_AP_STA);

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

  webServer.webServer.on("/wifi", HTTP_POST, [this]() {
    String ssid = webServer.webServer.arg("ssid");
    String password = webServer.webServer.arg("password");
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

    webServer.webServer.sendHeader("Location", "/wifi.htm");
    webServer.webServer.send(303);
  });

  webServer.webServer.on("/wifi", HTTP_GET, [this]() {
    String json = getWiFiJson();
    webServer.webServer.send(200, "application/json", json);
  });
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

