/*
 * Credits to
 * -esp8266-fastled-webserver:
 *    https://github.com/jasoncoon/esp8266-fastled-webserver
 *    Copyright (C) 2015-2016 Jason Coon
 * 
 * -canvas-cast
 *    Owen McAteer
 *    https://github.com/owenmcateer/canvas-cast
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
//#include "Looper.h" 

#include <ESP8266WiFi.h>

//#define RECV_PIN D4
//IRrecv irReceiver(RECV_PIN);

//#include "Commands.h"

//Looper* looper;

void setup(void) {
  Serial.begin(115200);
  delay(100);
  Serial.setDebugOutput(true);
  Serial.print("Starting Setup\n");
  
  //looper = new Looper();
  Serial.print("Looper created\n");
  //looper->setup();
  Serial.print("Setup finished\n");
}

void loop(void) {
  //looper->loop();
}







//void handleIrInput()
//{
//  InputCommand command = readCommand();
//
//  if (command != InputCommand::None) {
//    Serial.print("command: ");
//    Serial.println((int) command);
//  }
//
//  switch (command) {
//    case InputCommand::Up: {
//        adjustPattern(true);
//        break;
//      }
//    case InputCommand::Down: {
//        adjustPattern(false);
//        break;
//      }
//    case InputCommand::Power: {
//        setPower(power == 0 ? 1 : 0);
//        break;
//      }
//    case InputCommand::BrightnessUp: {
//        adjustBrightness(true);
//        break;
//      }
//    case InputCommand::BrightnessDown: {
//        adjustBrightness(false);
//        break;
//      }
//    case InputCommand::PlayMode: { // toggle pause/play
//        setAutoplay(!autoplay);
//        break;
//      }
//
//    // pattern buttons
//
//    case InputCommand::Pattern1: {
//        setPattern(0);
//        break;
//      }
//    case InputCommand::Pattern2: {
//        setPattern(1);
//        break;
//      }
//    case InputCommand::Pattern3: {
//        setPattern(2);
//        break;
//      }
//    case InputCommand::Pattern4: {
//        setPattern(3);
//        break;
//      }
//    case InputCommand::Pattern5: {
//        setPattern(4);
//        break;
//      }
//    case InputCommand::Pattern6: {
//        setPattern(5);
//        break;
//      }
//    case InputCommand::Pattern7: {
//        setPattern(6);
//        break;
//      }
//    case InputCommand::Pattern8: {
//        setPattern(7);
//        break;
//      }
//    case InputCommand::Pattern9: {
//        setPattern(8);
//        break;
//      }
//    case InputCommand::Pattern10: {
//        setPattern(9);
//        break;
//      }
//    case InputCommand::Pattern11: {
//        setPattern(10);
//        break;
//      }
//    case InputCommand::Pattern12: {
//        setPattern(11);
//        break;
//      }
//
//    // custom color adjustment buttons
//
//    case InputCommand::RedUp: {
//        solidColor.red += 8;
//        setSolidColor(solidColor);
//        break;
//      }
//    case InputCommand::RedDown: {
//        solidColor.red -= 8;
//        setSolidColor(solidColor);
//        break;
//      }
//    case InputCommand::GreenUp: {
//        solidColor.green += 8;
//        setSolidColor(solidColor);
//        break;
//      }
//    case InputCommand::GreenDown: {
//        solidColor.green -= 8;
//        setSolidColor(solidColor);
//        break;
//      }
//    case InputCommand::BlueUp: {
//        solidColor.blue += 8;
//        setSolidColor(solidColor);
//        break;
//      }
//    case InputCommand::BlueDown: {
//        solidColor.blue -= 8;
//        setSolidColor(solidColor);
//        break;
//      }
//
//    // color buttons
//
//    case InputCommand::Red: {
//        setSolidColor(CRGB::Red);
//        break;
//      }
//    case InputCommand::RedOrange: {
//        setSolidColor(CRGB::OrangeRed);
//        break;
//      }
//    case InputCommand::Orange: {
//        setSolidColor(CRGB::Orange);
//        break;
//      }
//    case InputCommand::YellowOrange: {
//        setSolidColor(CRGB::Goldenrod);
//        break;
//      }
//    case InputCommand::Yellow: {
//        setSolidColor(CRGB::Yellow);
//        break;
//      }
//
//    case InputCommand::Green: {
//        setSolidColor(CRGB::Green);
//        break;
//      }
//    case InputCommand::Lime: {
//        setSolidColor(CRGB::Lime);
//        break;
//      }
//    case InputCommand::Aqua: {
//        setSolidColor(CRGB::Aqua);
//        break;
//      }
//    case InputCommand::Teal: {
//        setSolidColor(CRGB::Teal);
//        break;
//      }
//    case InputCommand::Navy: {
//        setSolidColor(CRGB::Navy);
//        break;
//      }
//
//    case InputCommand::Blue: {
//        setSolidColor(CRGB::Blue);
//        break;
//      }
//    case InputCommand::RoyalBlue: {
//        setSolidColor(CRGB::RoyalBlue);
//        break;
//      }
//    case InputCommand::Purple: {
//        setSolidColor(CRGB::Purple);
//        break;
//      }
//    case InputCommand::Indigo: {
//        setSolidColor(CRGB::Indigo);
//        break;
//      }
//    case InputCommand::Magenta: {
//        setSolidColor(CRGB::Magenta);
//        break;
//      }
//
//    case InputCommand::White: {
//        setSolidColor(CRGB::White);
//        break;
//      }
//    case InputCommand::Pink: {
//        setSolidColor(CRGB::Pink);
//        break;
//      }
//    case InputCommand::LightPink: {
//        setSolidColor(CRGB::LightPink);
//        break;
//      }
//    case InputCommand::BabyBlue: {
//        setSolidColor(CRGB::CornflowerBlue);
//        break;
//      }
//    case InputCommand::LightBlue: {
//        setSolidColor(CRGB::LightBlue);
//        break;
//      }
//  }
//}




/*
void strandTest()
{
  static uint8_t i = 0;

  EVERY_N_SECONDS(1)
  {
    i++;
    if (i >= NUM_LEDS)
      i = 0;
  }

  fill_solid(settings.leds, NUM_LEDS, CRGB::Black);

  settings.leds[i] = settings.solidColor;
}



// Alternate rendering function just scrolls the current palette
// across the defined LED strip.
void palettetest( CRGB* ledarray, uint16_t numleds, const CRGBPalette16& gCurrentPalette)
{
  static uint8_t startindex = 0;
  startindex--;
  fill_palette( ledarray, numleds, startindex, (256 / NUM_LEDS) + 1, gCurrentPalette, 255, LINEARBLEND);
}
*/