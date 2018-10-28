

#include "Looper.h"


Looper::Looper() :
    settings(),
    patterns(settings),
    fields(settings, patterns),
    webServer(fields, settings, patterns),
    wifiMgr(settings, webServer),
    apMode(false)
 {
    gTargetPalette = gGradientPalettes[0];
}

Looper::~Looper() {}

// scale the brightness of all pixels down
void Looper::dimAll(byte value)
{
  for (int i = 0; i < NUM_LEDS; i++) {
    settings.leds[i].nscale8(value);
  }
}

void Looper::setup() {
  Serial.begin(115200);
  delay(100);
  Serial.setDebugOutput(true);

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

  SPIFFS.begin();
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
  //initializeWiFi();

  if (apMode)
  {
    WiFi.mode(WIFI_AP);

    // Do a little work to get a unique-ish name. Append the
    // last two bytes of the MAC (HEX'd) to "Thing-":
    uint8_t mac[WL_MAC_ADDR_LENGTH];
    WiFi.softAPmacAddress(mac);
    String macID = String(mac[WL_MAC_ADDR_LENGTH - 2], HEX) +
                   String(mac[WL_MAC_ADDR_LENGTH - 1], HEX);
    macID.toUpperCase();
    String AP_NameString = "ESP8266 Thing " + macID;

    char AP_NameChar[AP_NameString.length() + 1];
    memset(AP_NameChar, 0, AP_NameString.length() + 1);

    for (int i = 0; i < AP_NameString.length(); i++)
      AP_NameChar[i] = AP_NameString.charAt(i);

    WiFi.softAP(AP_NameChar, wifiMgr.WiFiAPPSK);

    Serial.printf("Connect to Wi-Fi access point: %s\n", AP_NameChar);
    Serial.println("and open http://192.168.4.1 in your browser");
  }
  else
  {
    WiFi.mode(WIFI_STA);
    Serial.printf("Connecting to %s\n", wifiMgr.ssid);
    if (String(WiFi.SSID()) != String(wifiMgr.ssid)) {
      WiFi.begin(wifiMgr.ssid, wifiMgr.password);
    }

    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }

    Serial.print("Connected! Open http://");
    Serial.print(WiFi.localIP());
    Serial.println(" in your browser");
  }

  //TODO checkWiFi();

  webServer.webServerSetup();

  settings.autoPlayTimeout = millis() + (settings.autoplayDuration * 1000);
}


void Looper::loop() {
  // Add entropy to random number generator; we use a lot of it.
  random16_add_entropy(random(65535));

  //TODO
  /*
  EVERY_N_SECONDS(10) {
     checkWiFi();
  }*/

//  dnsServer.processNextRequest();
  webServer.webSocketsServer.loop();
  webServer.webServer.handleClient();

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
  patterns.patternsVector[settings.currentPatternIndex].pattern();

  FastLED.show();

  // insert a delay to keep the framerate modest
  //FastLED.delay(1000 / FRAMES_PER_SECOND);
  }
}


void Looper::adjustBrightness(bool up)
{
  if (up && brightnessIndex < brightnessCount - 1)
    brightnessIndex++;
  else if (!up && brightnessIndex > 0)
    brightnessIndex--;

  settings.brightness = brightnessMap[brightnessIndex];

  FastLED.setBrightness(settings.brightness);

  EEPROM.write(0, settings.brightness);
  EEPROM.commit();

  webServer.broadcastInt("brightness", settings.brightness);
}