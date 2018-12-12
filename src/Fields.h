/*
   ESP8266 + FastLED + IR Remote: https://github.com/jasoncoon/esp8266-fastled-webserver
   Copyright (C) 2016 Jason Coon

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef _FIELDS_H_
#define _FIELDS_H_

#include "Settings.h"
#include <stdint.h>
#include <map>
#include "Patterns.h"





class Fields {
  
  typedef std::function<String ()> FieldGetter;
  typedef std::function<String (String)> FieldSetter;
  class Field;
  //typedef void (Fields::*FieldSetter)(String);

  class Field {
    public:
      String name;
      String label;
      String type;
      uint8_t min;
      uint8_t max;
      FieldGetter getValue;
      FieldGetter getOptions;
      FieldSetter setValue;
  };

/*
  typedef struct {
    String name;
    String label;
    String type;
    uint8_t min;
    uint8_t max;
    FieldGetter getValue;
    FieldGetter getOptions;
    FieldSetter setValue;
  } Field;*/


  private:
  Settings& settings;
  Patterns& patterns;
  static const String NumberFieldType;
  static const String BooleanFieldType;
  static const String SelectFieldType;
  static const String ColorFieldType;
  static const String SectionFieldType;

  
  public:
  explicit Fields(Settings &settings, Patterns& patterns);
  ~Fields() = default;
  Fields& operator=(const Fields&) = delete;
  
  String setBrightness(String value);
  String setPower(String value);
  String getPower();
  
  //String setBrightness(String value) {
  //  brightness = value.toInt();
  //  if(brightness < 0) brightness = 0;
  //  else if (brightness > 255) brightness = 255;
  //  return String(brightness);
  //}
  
  String getBrightness();
  
  String getPattern();
  
  String getPatterns();
  
  String getPalette();
  
  String getPalettes();
  
  String getAutoplay();
  
  String getAutoplayDuration();
  
  String getSolidColor();
  
  String getCooling();
  
  String getSparking();
  
  String getSpeed();
  
  String getTwinkleSpeed();
  
  String getTwinkleDensity();

  const String getFieldsJson();

  std::map<std::string, Fields::Field> fieldsMap;

  uint8_t fieldCount;

  Field getField(String name);
  String getFieldValue(String name);
  String setFieldValue(String name, String value);
};
#endif
