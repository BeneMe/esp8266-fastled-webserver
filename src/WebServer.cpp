#include "WebServer.h"
#include <EEPROM.h>





WebServer::WebServer(ESP8266WebServer* espWebServer, Settings& settings, Patterns& patterns, Fields& fields, WebSocketsServer* webSocketsServer) 
:
fsBrowser(),
espWebServer(espWebServer),
settings(settings),
patterns(patterns),
webSocketsServer(webSocketsServer),
fields(fields)
{
    webServerSetup();
}


void WebServer::sendInt(uint8_t value)
{
  sendString(String(value));
}

void WebServer::sendString(String value)
{
  espWebServer->send(200, "text/plain", value);
}


void WebServer::webServerSetup() {
  
  httpUpdateServer.setup(espWebServer);

  Serial.println("initializing web server endpoints");
  Serial.println("adding /power");
  espWebServer->on("/power", HTTP_POST, [&]() {
    String value = espWebServer->arg("value");
    setPower(value.toInt());
    sendInt(value.toInt());
  });

  //list directory
  Serial.println("adding /list");
  espWebServer->on("/list", HTTP_GET, [&]() {
    Serial.println("handle /list");
    fsBrowser.handleFileList(*espWebServer);
  });

  //load editor
  Serial.println("adding GET /edit");
  espWebServer->on("/edit", HTTP_GET, [&]() {
    if (!fsBrowser.handleFileRead("/edit.htm", *espWebServer)) espWebServer->send(404, "text/plain", "FileNotFound");
  });
  //create file
  Serial.println("adding PUT /edit");
  espWebServer->on("/edit", HTTP_PUT, [&]() {
    fsBrowser.handleFileCreate(*espWebServer);
  });
  //delete file
  Serial.println("adding DELETE /edit");
  espWebServer->on("/edit", HTTP_DELETE, [&]() {
    fsBrowser.handleFileDelete(*espWebServer);
  });
  //first callback is called after the request has ended with all parsed arguments
  //second callback handles file uploads at that location
  Serial.println("adding POST /edit");
  espWebServer->on("/edit", HTTP_POST, [&]() {
    espWebServer->send(200, "text/plain", "");
  }, [&]() {
    fsBrowser.handleFileUpload(*espWebServer);
  });

  addFieldsEndpoints();

  Serial.println("adding static /");
  espWebServer->serveStatic("/", SPIFFS, "/", "max-age=86400");


  
  espWebServer->onNotFound(std::bind(&WebServer::handleNotFound, this));

  espWebServer->begin();
  Serial.println("Checking index.htm / index.htm.gz...");
  if (!fsBrowser.existsOrZipped("/index.htm")) {
    Serial.println("FAILED: could not find index.htm");
  }

  Serial.println("HTTP web server started");

}


void WebServer::addFieldsEndpoints() {

  Serial.println("adding POST /io");
  espWebServer->on("/io", HTTP_POST, [&]() {
      String pin = espWebServer->arg("pin");
      String value = espWebServer->arg("value");
      Serial.println("Setting pin " + pin + " to " + value);
      uint8_t pin_int = pin.toInt();
      uint8_t value_int = value.toInt();
      setPin(pin_int, value_int);
      espWebServer->send(200, "text/plain", "Set gpio pin " + pin + " to " + value);
  });

  espWebServer->on("/all", HTTP_GET, [&]() { 
    String json = fields.getFieldsJson();
    espWebServer->send(200, "text/json", json);
  });

  espWebServer->on("/fieldValue", HTTP_GET, [&]() {
    String name = espWebServer->arg("name");
    String value = fields.getFieldValue(name);
    espWebServer->send(200, "text/json", value);
  });

  espWebServer->on("/fieldValue", HTTP_POST, [&]() {
    String name = espWebServer->arg("name");
    String value = espWebServer->arg("value");
    String newValue = fields.setFieldValue(name, value);
    espWebServer->send(200, "text/json", newValue);
  });

  espWebServer->on("/power", HTTP_POST, [&]() {
    String value = espWebServer->arg("value");
    setPower(value.toInt());
    sendInt(settings.power);
  });

  espWebServer->on("/cooling", HTTP_POST, [&]() {
    String value = espWebServer->arg("value");
    settings.cooling = value.toInt();
    broadcastInt("cooling", settings.cooling);
    sendInt(settings.cooling);
  });

  espWebServer->on("/sparking", HTTP_POST, [&]() {
    String value = espWebServer->arg("value");
    settings.sparking = value.toInt();
    broadcastInt("sparking", settings.sparking);
    sendInt(settings.sparking);
  });

  espWebServer->on("/speed", HTTP_POST, [&]() {
    String value = espWebServer->arg("value");
    settings.speed = value.toInt();
    broadcastInt("speed", settings.speed);
    sendInt(settings.speed);
  });

  espWebServer->on("/twinkleSpeed", HTTP_POST, [&]() {
    String value = espWebServer->arg("value");
    settings.twinkleSpeed = value.toInt();
    if (settings.twinkleSpeed < 0) settings.twinkleSpeed = 0;
    else if (settings.twinkleSpeed > 8) settings.twinkleSpeed = 8;
    broadcastInt("twinkleSpeed", settings.twinkleSpeed);
    sendInt(settings.twinkleSpeed);
  });

  espWebServer->on("/twinkleDensity", HTTP_POST, [&]() {
    String value = espWebServer->arg("value");
    settings.twinkleDensity = value.toInt();
    if (settings.twinkleDensity < 0) settings.twinkleDensity = 0;
    else if (settings.twinkleDensity > 8) settings.twinkleDensity = 8;
    broadcastInt("twinkleDensity", settings.twinkleDensity);
    sendInt(settings.twinkleDensity);
  });

  espWebServer->on("/solidColor", HTTP_POST, [&]() {
    String r = espWebServer->arg("r");
    String g = espWebServer->arg("g");
    String b = espWebServer->arg("b");
    setSolidColor(r.toInt(), g.toInt(), b.toInt());
    sendString(String(settings.solidColor.r) + "," + String(settings.solidColor.g) + "," + String(settings.solidColor.b));
  });

  espWebServer->on("/pattern", HTTP_POST, [&]() {
    String value = espWebServer->arg("value");
    setPattern(value.toInt());
    sendInt(settings.currentPatternIndex);
  });

  espWebServer->on("/patternName", HTTP_POST, [&]() {
    String value = espWebServer->arg("value");
    setPatternName(value);
    sendInt(settings.currentPatternIndex);
  });

  espWebServer->on("/palette", HTTP_POST, [&]() {
    String value = espWebServer->arg("value");
    setPalette(value.toInt());
    sendInt(settings.currentPaletteIndex);
  });

  espWebServer->on("/paletteName", HTTP_POST, [&]() {
    String value = espWebServer->arg("value");
    setPaletteName(value);
    sendInt(settings.currentPaletteIndex);
  });

  espWebServer->on("/brightness", HTTP_POST, [&]() {
    String value = espWebServer->arg("value");
    setBrightness(value.toInt());
    sendInt(settings.brightness);
  });

  espWebServer->on("/autoplay", HTTP_POST, [&]() {
    String value = espWebServer->arg("value");
    setAutoplay(value.toInt());
    sendInt(settings.autoplay);
  });

  espWebServer->on("/autoplayDuration", HTTP_POST, [&]() {
    String value = espWebServer->arg("value");
    setAutoplayDuration(value.toInt());
    sendInt(settings.autoplayDuration);
  });
}

void WebServer::handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += espWebServer->uri();
  message += "\nMethod: ";
  message += (espWebServer->method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += espWebServer->args();
  message += "\n";

  for (uint8_t i = 0; i < espWebServer->args(); i++) {
    message += " " + espWebServer->argName(i) + ": " + espWebServer->arg(i) + "\n";
  }

  espWebServer->send(404, "text/plain", message);
}

void WebServer::setPin(uint8_t pin, uint8_t value){
  digitalWrite(pin, value);
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

  broadcastString("color", String(settings.solidColor.r) + "," + String(settings.solidColor.g) + "," + String(settings.solidColor.b));
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
  for (uint8_t i = 0; i < patterns.patternCount; i++) {
    if (patterns.patternsVector[i].name == name) {
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
  for (uint8_t i = 0; i < patterns.paletteCount; i++) {
    if (patterns.paletteNames[i] == name) {
      setPalette(i);
      break;
    }
  }
}

void WebServer::adjustBrightness(bool up)
{
  if (up && settings.brightnessIndex < settings.brightnessCount - 1)
    settings.brightnessIndex++;
  else if (!up && settings.brightnessIndex > 0)
    settings.brightnessIndex--;

  settings.brightness = settings.brightnessMap[settings.brightnessIndex];

  FastLED.setBrightness(settings.brightness);

  EEPROM.write(0, settings.brightness);
  EEPROM.commit();

  broadcastInt("brightness", settings.brightness);
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


void WebServer::broadcastInt(String name, uint8_t value)
{
  String json = "{\"name\":\"" + name + "\",\"value\":" + String(value) + "}";
  webSocketsServer->broadcastTXT(json);
}

void WebServer::broadcastString(String name, String value)
{
  String json = "{\"name\":\"" + name + "\",\"value\":\"" + String(value) + "\"}";
  webSocketsServer->broadcastTXT(json);
}

void WebServer::loop() {
  espWebServer->handleClient();
}

