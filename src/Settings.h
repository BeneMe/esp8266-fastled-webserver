

#ifndef __SETTINGS_H__
#define __SETTINGS_H__

#include <stdint.h>
#include <FastLED.h>
#include "defines.h"

class Settings {
public:
  explicit Settings();
  ~Settings() = default;
  Settings& operator=(const Settings&) = delete;

  void loadSettings();
  uint8_t power = 1;
  uint8_t currentPatternIndex = 0; // Index number of which pattern is current
  uint8_t brightness;
  uint8_t autoplay = 0; 
  uint8_t autoplayDuration = 10;
  unsigned long autoPlayTimeout = 0;
  
  uint8_t currentPaletteIndex = 0;
  
  uint8_t gHue = 0; // rotating "base color" used by many of the patterns
    
  // COOLING: How much does the air cool as it rises?
  // Less cooling = taller flames.  More cooling = shorter flames.
  // Default 50, suggested range 20-100
  uint8_t cooling = 49;

  // SPARKING: What chance (out of 255) is there that a new spark will be lit?
  // Higher chance = more roaring fire.  Lower chance = more flickery fire.
  // Default 120, suggested range 50-200.
  uint8_t sparking = 60;

  uint8_t speed = 30;

  CRGB solidColor = CRGB::Blue;

  CRGBPalette16 gCurrentPalette;
  
  CRGB leds[NUM_LEDS +1];

  uint8_t twinkleSpeed = 4;
  uint8_t twinkleDensity = 5;
  CRGB gBackgroundColor = CRGB::Black;
  CRGBPalette16 twinkleFoxPalette;

  uint8_t brightnessCount = 5;
  uint8_t brightnessMap[5] = { 16, 32, 64, 128, 255 };
  uint8_t brightnessIndex = 0;
};
#endif
