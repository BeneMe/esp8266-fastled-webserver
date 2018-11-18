#ifndef WEBSERVER_H
#define WEBSERVER_H

#include <ESP8266WebServer.h>
#include <WebSocketsServer.h>
#include <ESP8266HTTPUpdateServer.h>
#include "Fields.h"
#include "FSBrowser.h"
#include "Settings.h"
#include "TwinkleFOX.h"
#include "Patterns.h"


class WebServer {
  private:
    Patterns patterns;
    Settings settings;
    FSBrowser fsBrowser;
  public:
    explicit WebServer(Fields &fields, Settings &settings, Patterns& patterns);
    ~WebServer() = default;
    WebServer& operator=(const WebServer&) = delete;
    
    
    ESP8266HTTPUpdateServer httpUpdateServer;
    WebSocketsServer webSocketsServer = WebSocketsServer(81);
    Fields fields;
    
    
    
    void webServerSetup();
    void handleClient();
    void sendInt(uint8_t value);
    void sendString(String value);
    void broadcastInt(String name, uint8_t value);
    void broadcastString(String name, String value);
    void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length);
    
    void setPower(uint8_t value);
    void setAutoplay(uint8_t value);
    void setAutoplayDuration(uint8_t value);
    void setSolidColor(CRGB color);
    void setSolidColor(uint8_t r, uint8_t g, uint8_t b);
    void adjustPattern(bool up);
    void setPattern(uint8_t value);
    void setPatternName(String name);
    void setPalette(uint8_t value);
    void setPaletteName(String name);
    void setBrightness(uint8_t value);
};

#endif