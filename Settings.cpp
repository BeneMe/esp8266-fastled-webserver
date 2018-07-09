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

Settings::~Settings() {}



void Settings::loadSettings()
{
  brightness = EEPROM.read(0);

  currentPatternIndex = EEPROM.read(1);
  /*  TODO
  if (currentPatternIndex < 0)
    currentPatternIndex = 0;
  else if (currentPatternIndex >= patterns.patternCount)
    currentPatternIndex = patterns.patternCount - 1;
*/
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

  /* TODO
  if (currentPaletteIndex < 0)
    currentPaletteIndex = 0;
  else if (currentPaletteIndex >= Patterns::paletteCount)
    currentPaletteIndex = Patterns::paletteCount - 1;
    */
}


