#include "WebSocketHandler.h"
#include "FastLED.h"

WebSocketHandler::WebSocketHandler(WebSocketsServer* webSocket, Settings& settings)
  : webSocket(webSocket),
  settings(settings)
{

}


void WebSocketHandler::setup() {
    Serial.printf("Opening websocket");
    webSocket->begin();
    webSocket->onEvent( [&](uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
      webSocketEvent(num, type, payload, length);
    });

}

void WebSocketHandler::loop() {
  webSocket->loop();
}


//std::function<void (uint8_t num, WStype_t type, uint8_t * payload, size_t length)
void WebSocketHandler::webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
  switch(type) {
    case WStype_DISCONNECTED:
        Serial.printf("[%u] Disconnected!\r\n", num);
        settings.leds[0] = CRGB::Yellow;
        FastLED.show();
      break;

    case WStype_TEXT:
    {
        Serial.printf("[%u] got Text: %s\n", num, payload);

        String value = ((const char*)&payload[0]);
        int pos = value.indexOf(':');
        String cmd = value.substring(0, pos);
        int val = value.substring(pos + 1).toInt();

        if (cmd == String("BRIGHTNESS")) {
          // Set matrix brightness
          FastLED.setBrightness(val);
        }
    }
      break;

    case WStype_CONNECTED:
    {
        IPAddress ip = webSocket->remoteIP(num);
        Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\r\n", num, ip[0], ip[1], ip[2], ip[3], payload);
        settings.leds[0] = CRGB::Blue;
        FastLED.show();
    }
      break;

    case WStype_BIN:
      // Serial.printf("[%u] get binary length: %u\n", num, length);
      for (int i = 0; i < length; i += 3) {
        settings.leds[i / 3].setRGB(payload[i], payload[(i + 1)], payload[(i + 2)]);
      }
      FastLED.show();
      break;

    default:
      Serial.printf("Invalid WStype [%d]\r\n", type);
      // Show status LED
      
      settings.leds[0] = CRGB::Red;
      FastLED.show();
      break;
  }
}