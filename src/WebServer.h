#ifndef WEBSERVER_H
#define WEBSERVER_H


#include <ESP8266WebServer.h>
#include <ESP8266HTTPUpdateServer.h>
#include "FSBrowser.h"
#include <FastLED.h>
#include "WifiConfig.h"
#include "Settings.h"
#include "Patterns.h"
#include "Fields.h"
#include "WebSocketsServer.h"

class WebServer {
  private:
    FSBrowser fsBrowser;
    ESP8266WebServer* espWebServer;
    Settings& settings;
    Patterns& patterns;
    WebSocketsServer* webSocketsServer;
    Fields& fields;
    String apiPath = "/api";
    
  public:
    explicit WebServer(ESP8266WebServer* espWebServer, Settings& settings, Patterns& patterns, Fields& fields, WebSocketsServer* webSocketsServer);
    ~WebServer() = default;
    
    
    ESP8266HTTPUpdateServer httpUpdateServer;
    
    
    
    void webServerSetup();
    void loop();
    void sendInt(uint8_t value);
    void sendString(String value);
    void broadcastInt(String name, uint8_t value);
    void broadcastString(String name, String value);
    void handleNotFound();
    
    void setPower(uint8_t value);
    void setPin(uint8_t pin, uint8_t value);
    void adjustPattern(bool up);
    void adjustBrightness(bool up);
    void setAutoplay(uint8_t value);
    void setAutoplayDuration(uint8_t value);
    void setSolidColor(CRGB color);
    void setSolidColor(uint8_t r, uint8_t g, uint8_t b);
    void setPattern(uint8_t value);
    void setPatternName(String name);
    void setPalette(uint8_t value);
    void setPaletteName(String name);
    void setBrightness(uint8_t value);

    void addFieldsEndpoints();
};

#endif