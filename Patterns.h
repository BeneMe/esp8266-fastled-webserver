#ifndef __PATTERNS_H__
#define __PATTERNS_H__

#include <stdint.h>
#include <FastLED.h>
#include "Settings.h"
#include <list>
#include "TwinkleFOX.h"
#include "Twinkles.h"

// Forward declarations of an array of cpt-city gradient palettes, and
// a count of how many there are.  The actual color palette definitions
// are at the bottom of this file.
extern const TProgmemRGBGradientPalettePtr gGradientPalettes[];
extern const uint8_t gGradientPaletteCount;



class Patterns {


typedef std::function<void ()> Pattern;
typedef Pattern PatternList[];
class PatternAndName {
  public: 
  Pattern pattern;
  String name;
};

  
typedef struct {
  CRGBPalette16 palette;
   String name;
 } PaletteAndName;
typedef PaletteAndName PaletteAndNameList[];


  private:
  Settings& settings;
  CRGBPalette16 IceColors_p = CRGBPalette16(CRGB::Black, CRGB::Blue, CRGB::Aqua, CRGB::White);
  TwinkleFox twinkleFox;
  Twinkles twinkles;


  public:
  Patterns(Settings& settings);
  void pride();
  void colorWaves();


  void rainbow();
  void rainbowWithGlitter();
  void rainbowSolid();
  void confetti();
  void sinelon();
  void bpm();
  void juggle();
  void fire();
  void water();
  void canvas();

  void showSolidColor();

  void addGlitter( uint8_t chanceOfGlitter);
  void radialPaletteShift();
  void heatMap(CRGBPalette16 palette, bool up);
  void colorwaves( CRGB* ledarray, uint16_t numleds, CRGBPalette16& palette);

  uint8_t beatsaw8( accum88 beats_per_minute, uint8_t lowest = 0, uint8_t highest = 255,
                  uint32_t timebase = 0, uint8_t phase_offset = 0);

// List of patterns to cycle through.  Each is defined as a separate function below.

//Patterns::PatternAndName patterns[];
std::vector<PatternAndName> patternsVector;

uint8_t patternCount;

std::vector<CRGBPalette16> palettes;

uint8_t paletteCount;

std::vector<String> paletteNames;
};
#endif
