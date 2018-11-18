#include "Settings.h"
#include <EEPROM.h>
#include <stdint.h>
#include "Patterns.h"

Settings::Settings()
: leds()
{
  power = 1;
  gCurrentPalette = CRGB( CRGB::Black);
}

void Settings::loadSettings()
{
  brightness = EEPROM.read(0);

  currentPatternIndex = EEPROM.read(1);
  
  if (currentPatternIndex < 0) {
    currentPatternIndex = 0;
  }


  byte r = EEPROM.read(2);
  byte g = EEPROM.read(3);
  byte b = EEPROM.read(4);

  if (r == 0 && g == 0 && b == 0)
  {
  }
  else
  {
    solidColor = CRGB(r, g, b);
  }

  Settings::power = EEPROM.read(5);

  autoplay = EEPROM.read(6);
  autoplayDuration = EEPROM.read(7);

  currentPaletteIndex = EEPROM.read(8);


  if (currentPaletteIndex < 0) {
    currentPaletteIndex = 0;
  }
}


