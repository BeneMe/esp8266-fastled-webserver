

#include "Looper.h"
#include "GradientPalettes.h"
#include "WebSocketsServer.h"

Looper::Looper(ESP8266WebServer* esp8266WebServer, WebSocketsServer* webSocketsServer)
  : settings(),
    patterns(settings),
    fields(settings, patterns),
    webServer(esp8266WebServer, settings, patterns, fields, webSocketsServer),
    webSocketHandler(webSocketsServer, settings),
    wifiMgr(settings),
    apMode(false)
 {
    gTargetPalette = gGradientPalettes[0];
    apMode = false;
}


// scale the brightness of all pixels down
void Looper::dimAll(byte value)
{
  for (int i = 0; i < NUM_LEDS; i++) {
    settings.leds[i].nscale8(value);
  }
}

void Looper::setup() {
  

  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(settings.leds, NUM_LEDS);         // for WS2812 (Neopixel)
  //FastLED.addLeds<LED_TYPE,DATA_PIN,CLK_PIN,COLOR_ORDER>(leds, NUM_LEDS); // for APA102 (Dotstar)
  FastLED.setDither(false);
  FastLED.setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(settings.brightness);
  FastLED.setMaxPowerInVoltsAndMilliamps(5, MILLI_AMPS);
  fill_solid(settings.leds, NUM_LEDS, CRGB::Black);
  FastLED.show();

  EEPROM.begin(512);
  settings.loadSettings();

  FastLED.setBrightness(settings.brightness);

//  irReceiver.enableIRIn(); // Start the receiver

  Serial.println();
  Serial.print( F("Heap: ") ); Serial.println(system_get_free_heap_size());
  Serial.print( F("Boot Vers: ") ); Serial.println(system_get_boot_version());
  Serial.print( F("CPU: ") ); Serial.println(system_get_cpu_freq());
  Serial.print( F("SDK: ") ); Serial.println(system_get_sdk_version());
  Serial.print( F("Chip ID: ") ); Serial.println(system_get_chip_id());
  Serial.print( F("Flash ID: ") ); Serial.println(spi_flash_get_id());
  Serial.print( F("Flash Size: ") ); Serial.println(ESP.getFlashChipRealSize());
  Serial.print( F("Vcc: ") ); Serial.println(ESP.getVcc());
  Serial.println();

  {
    Dir dir = SPIFFS.openDir("/");
    while (dir.next()) {
      String fileName = dir.fileName();
      size_t fileSize = dir.fileSize();
      Serial.printf("FS File: %s, size: %s\n", fileName.c_str(), String(fileSize).c_str());
    }
    Serial.printf("\n");
  }

  //disabled due to https://github.com/jasoncoon/esp8266-fastled-webserver/issues/62
  wifiMgr.initializeWiFi();


  webServer.webServerSetup();
  webSocketHandler.setup();

  settings.autoPlayTimeout = millis() + (settings.autoplayDuration * 1000);
}


void Looper::loop() {
  // Add entropy to random number generator; we use a lot of it.
  random16_add_entropy(random(65535));

 
  EVERY_N_SECONDS(10) {
     wifiMgr.checkWiFi();
  }

//  dnsServer.processNextRequest();
  webServer.loop();
  webSocketHandler.loop();
//  handleIrInput();

  if (settings.power == 0) {
    fill_solid(settings.leds, NUM_LEDS, CRGB::Black);
    FastLED.show();
    // FastLED.delay(15);
    return;
  } else {

  // EVERY_N_SECONDS(10) {
  //   Serial.print( F("Heap: ") ); Serial.println(system_get_free_heap_size());
  // }

  // change to a new cpt-city gradient palette
  EVERY_N_SECONDS( secondsPerPalette ) {
    gCurrentPaletteNumber = addmod8( gCurrentPaletteNumber, 1, gGradientPaletteCount);
    gTargetPalette = gGradientPalettes[ gCurrentPaletteNumber ];
  }

  EVERY_N_MILLISECONDS(40) {
    // slowly blend the current palette to the next
    nblendPaletteTowardPalette( settings.gCurrentPalette, gTargetPalette, 8);
    settings.gHue++;  // slowly cycle the "base color" through the rainbow
  }

  if (settings.autoplay && (millis() > settings.autoPlayTimeout)) {
    webServer.adjustPattern(true);
    settings.autoPlayTimeout = millis() + (settings.autoplayDuration * 1000);
  }

  // Call the current pattern function once, updating the 'leds' array
  if(settings.currentPatternIndex < patterns.patternsVector.size()) {
    patterns.patternsVector[settings.currentPatternIndex].pattern();
  }

  FastLED.show();

  // insert a delay to keep the framerate modest
  //FastLED.delay(1000 / FRAMES_PER_SECOND);
  
  }
}
