#include "WebServer.h"
#include <EEPROM.h>

WebServer::WebServer(Fields& fields, Settings& settings, Patterns& patterns) 
: webServer(80), fields(fields), settings(settings), patterns(patterns), fsBrowser(FSBrowser())
{
}


void WebServer::sendInt(uint8_t value)
{
  sendString(String(value));
}

void WebServer::sendString(String value)
{
  webServer.send(200, "text/plain", value);
}


void WebServer::broadcastInt(String name, uint8_t value)
{
  String json = "{\"name\":\"" + name + "\",\"value\":" + String(value) + "}";
  webSocketsServer.broadcastTXT(json);
}

void WebServer::broadcastString(String name, String value)
{
  String json = "{\"name\":\"" + name + "\",\"value\":\"" + String(value) + "\"}";
  webSocketsServer.broadcastTXT(json);
}

void WebServer::webServerSetup() {
  
  httpUpdateServer.setup(&webServer);
  webServer.on("/all", HTTP_GET, [this]() {
    String json = fields.getFieldsJson();
    webServer.send(200, "text/json", json);
  });

  webServer.on("/fieldValue", HTTP_GET, [this]() {
    String name = webServer.arg("name");
    String value = fields.getFieldValue(name);
    webServer.send(200, "text/json", value);
  });

  webServer.on("/fieldValue", HTTP_POST, [this]() {
    String name = webServer.arg("name");
    String value = webServer.arg("value");
    String newValue = fields.setFieldValue(name, value);
    webServer.send(200, "text/json", newValue);
  });

  webServer.on("/power", HTTP_POST, [this]() {
    String value = webServer.arg("value");
    setPower(value.toInt());
    sendInt(value.toInt());
  });

  webServer.on("/cooling", HTTP_POST, [this]() {
    String value = webServer.arg("value");
    settings.cooling = value.toInt();
    broadcastInt("cooling", settings.cooling);
    sendInt(settings.cooling);
  });

  webServer.on("/sparking", HTTP_POST, [this]() {
    String value = webServer.arg("value");
    settings.sparking = value.toInt();
    broadcastInt("sparking", settings.sparking);
    sendInt(settings.sparking);
  });

  webServer.on("/speed", HTTP_POST, [this]() {
    String value = webServer.arg("value");
    settings.speed = value.toInt();
    broadcastInt("speed", settings.speed);
    sendInt(settings.speed);
  });

  webServer.on("/twinkleSpeed", HTTP_POST, [this]() {
    String value = webServer.arg("value");
    settings.twinkleSpeed = value.toInt();
    if(settings.twinkleSpeed < 0) settings.twinkleSpeed = 0;
    else if (settings.twinkleSpeed > 8) settings.twinkleSpeed = 8;
    broadcastInt("twinkleSpeed", settings.twinkleSpeed);
    sendInt(settings.twinkleSpeed);
  });

  webServer.on("/twinkleDensity", HTTP_POST, [this]() {
    String value = webServer.arg("value");
    settings.twinkleDensity = value.toInt();
    if(settings.twinkleDensity < 0) settings.twinkleDensity = 0;
    else if (settings.twinkleDensity > 8) settings.twinkleDensity = 8;
    broadcastInt("twinkleDensity", settings.twinkleDensity);
    sendInt(settings.twinkleDensity);
  });

  webServer.on("/solidColor", HTTP_POST, [this]() {
    String r = webServer.arg("r");
    String g = webServer.arg("g");
    String b = webServer.arg("b");
    setSolidColor(r.toInt(), g.toInt(), b.toInt());
    sendString(String(settings.solidColor.r) + "," + String(settings.solidColor.g) + "," + String(settings.solidColor.b));
  });

  webServer.on("/pattern", HTTP_POST, [this]() {
    String value = webServer.arg("value");
    setPattern(value.toInt());
    sendInt(settings.currentPatternIndex);
  });

  webServer.on("/patternName", HTTP_POST, [this]() {
    String value = webServer.arg("value");
    setPatternName(value);
    sendInt(settings.currentPatternIndex);
  });

  webServer.on("/palette", HTTP_POST, [this]() {
    String value = webServer.arg("value");
    setPalette(value.toInt());
    sendInt(settings.currentPaletteIndex);
  });

  webServer.on("/paletteName", HTTP_POST, [this]() {
    String value = webServer.arg("value");
    setPaletteName(value);
    sendInt(settings.currentPaletteIndex);
  });

  webServer.on("/brightness", HTTP_POST, [this]() {
    String value = webServer.arg("value");
    setBrightness(value.toInt());
    sendInt(settings.brightness);
  });

  webServer.on("/autoplay", HTTP_POST, [this]() {
    String value = webServer.arg("value");
    setAutoplay(value.toInt());
    sendInt(settings.autoplay);
  });

  webServer.on("/autoplayDuration", HTTP_POST, [this]() {
    String value = webServer.arg("value");
    setAutoplayDuration(value.toInt());
    sendInt(settings.autoplayDuration);
  });

  //list directory
  webServer.on("/list", HTTP_GET, [this]() {
    fsBrowser.handleFileList(webServer);
  });

  //load editor
  webServer.on("/edit", HTTP_GET, [this]() {
    if (!fsBrowser.handleFileRead("/edit.htm", webServer)) webServer.send(404, "text/plain", "FileNotFound");
  });
  //create file
  webServer.on("/edit", HTTP_PUT, [this]() {
    fsBrowser.handleFileCreate(webServer);
  });
  //delete file
  webServer.on("/edit", HTTP_DELETE, [this]() {
    fsBrowser.handleFileDelete(webServer);
  });
  //first callback is called after the request has ended with all parsed arguments
  //second callback handles file uploads at that location
  webServer.on("/edit", HTTP_POST, [this]() {
    webServer.send(200, "text/plain", "");
  }, [this]() {
    fsBrowser.handleFileUpload(webServer);
  });

  webServer.serveStatic("/", SPIFFS, "/", "max-age=86400");

  webServer.begin();
  Serial.println("HTTP web server started");

  webSocketsServer.begin();
  webSocketsServer.onEvent([this](uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
    webSocketEvent(num, type, payload, length);
    });
  Serial.println("Web socket server started");
  
}


void WebServer::setBrightness(uint8_t value)
{
  if (value > 255)
    value = 255;
  else if (value < 0) value = 0;

  settings.brightness = value;

  FastLED.setBrightness(settings.brightness);

  EEPROM.write(0, settings.brightness);
  EEPROM.commit();

  broadcastInt("brightness", settings.brightness);
}

void WebServer::setPower(uint8_t value)
{
  settings.power = value == 0 ? 0 : 1;

  EEPROM.write(5, settings.power);
  EEPROM.commit();

  broadcastInt("power", settings.power);
}

void WebServer::setAutoplay(uint8_t value)
  {
  settings.autoplay = value == 0 ? 0 : 1;

  EEPROM.write(6, settings.autoplay);
  EEPROM.commit();

  broadcastInt("autoplay", settings.autoplay);
}

void WebServer::setAutoplayDuration(uint8_t value)
{
  settings.autoplayDuration = value;

  EEPROM.write(7, settings.autoplayDuration);
  EEPROM.commit();

  settings.autoPlayTimeout = millis() + (settings.autoplayDuration * 1000);

  broadcastInt("autoplayDuration", settings.autoplayDuration);
}

void WebServer::setSolidColor(CRGB color)
{
  setSolidColor(color.r, color.g, color.b);
}

void WebServer::setSolidColor(uint8_t r, uint8_t g, uint8_t b)
{
  settings.solidColor = CRGB(r, g, b);

  EEPROM.write(2, r);
  EEPROM.write(3, g);
  EEPROM.write(4, b);
  EEPROM.commit();

  setPattern(patterns.patternCount - 1);

  broadcastString("color", String(settings.solidColor.r) +
   "," + String(settings.solidColor.g) + "," + String(settings.solidColor.b));
}

// increase or decrease the current pattern number, and wrap around at the ends
void WebServer::adjustPattern(bool up)
{
  if (up)
    settings.currentPatternIndex++;
  else
    settings.currentPatternIndex--;

  // wrap around at the ends
  if (settings.currentPatternIndex < 0)
    settings.currentPatternIndex = patterns.patternCount - 1;
  if (settings.currentPatternIndex >= patterns.patternCount)
    settings.currentPatternIndex = 0;

  if (settings.autoplay == 0) {
    EEPROM.write(1, settings.currentPatternIndex);
    EEPROM.commit();
  }

  broadcastInt("pattern", settings.currentPatternIndex);
}

void WebServer::setPattern(uint8_t value)
{
  if (value >= patterns.patternCount)
    value = patterns.patternCount - 1;

  settings.currentPatternIndex = value;

  if (settings.autoplay == 0) {
    EEPROM.write(1, settings.currentPatternIndex);
    EEPROM.commit();
  }

  broadcastInt("pattern", settings.currentPatternIndex);
}

void WebServer::setPatternName(String name)
{
  for(uint8_t i = 0; i < patterns.patternCount; i++) {
    if(patterns.patternsVector[i].name == name) {
      setPattern(i);
      break;
    }
  }
}

void WebServer::setPalette(uint8_t value)
{
  if (value >= patterns.paletteCount)
    value = patterns.paletteCount - 1;

  settings.currentPaletteIndex = value;

  EEPROM.write(8, settings.currentPaletteIndex);
  EEPROM.commit();

  broadcastInt("palette", settings.currentPaletteIndex);
}

void WebServer::setPaletteName(String name)
{
  for(uint8_t i = 0; i < patterns.paletteCount; i++) {
    if(patterns.paletteNames[i] == name) {
      setPalette(i);
      break;
    }
  }
}

void WebServer::webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {

  switch (type) {
    case WStype_DISCONNECTED:
      Serial.printf("[%u] Disconnected!\n", num);
      break;

    case WStype_CONNECTED:
      {
        IPAddress ip = webSocketsServer.remoteIP(num);
        Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);

        // send message to client
        // webSocketsServer.sendTXT(num, "Connected");
      }
      break;

    case WStype_TEXT:
      Serial.printf("[%u] get Text: %s\n", num, payload);

      // send message to client
      // webSocketsServer.sendTXT(num, "message here");

      // send data to all connected clients
      // webSocketsServer.broadcastTXT("message here");
      break;

    case WStype_BIN:
      Serial.printf("[%u] get binary length: %u\n", num, length);
      hexdump(payload, length);

      for (int i = 0; i < length; i += 3) {
        settings.leds[i / 3].setRGB(payload[i], payload[(i + 1)], payload[(i + 2)]);
      }
      FastLED.show();
      // send message to client
      // webSocketsServer.sendBIN(num, payload, lenght);
      break;
    default:
      Serial.printf("Unhandled WStype event!");
  }
}
