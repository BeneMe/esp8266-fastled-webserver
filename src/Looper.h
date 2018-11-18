#ifndef LOOPER_H
#define LOOPER_H

#include <FastLED.h>
FASTLED_USING_NAMESPACE

extern "C" {
  #include "user_interface.h"
}

#include <ESP8266WiFi.h>
//#include <ESP8266mDNS.h>

#include <FS.h>
#include <EEPROM.h>
//#include <IRremoteESP8266.h>

#include "defines.h"
#include "TwinkleFOX.h"
#include "WifiMgr.h"
#include "FSBrowser.h"
#include "WebServer.h"
#include "Fields.h"
#include "Settings.h"
#include "Patterns.h"
#include "Twinkles.h"

// Forward declarations of an array of cpt-city gradient palettes, and
// a count of how many there are.  The actual color palette definitions
// are at the bottom of this file.
extern const TProgmemRGBGradientPalettePtr gGradientPalettes[];
extern const uint8_t gGradientPaletteCount;

class Looper {
    private:
    uint8_t brightnessCount = 5;
    uint8_t brightnessMap[5] = { 16, 32, 64, 128, 255 };
    uint8_t brightnessIndex = 0;

    // ten seconds per color palette makes a good demo
    // 20-120 is better for deployment
    uint8_t secondsPerPalette = 10;
    uint8_t speed = 30;
    uint8_t gCurrentPaletteNumber = 0;

    CRGBPalette16 gTargetPalette;

    Settings settings;
    Patterns patterns;
    Fields fields;
    WebServer webServer;
    WifiMgr wifiMgr;
    bool apMode;

    void dimAll(byte value);

public:
    explicit Looper();
    ~Looper() = default;
    Looper& operator=(const Looper&) = delete;

    void setup();
    void loop();
    void adjustBrightness(bool up);
};

#endif