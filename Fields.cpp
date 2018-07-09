
#include "Fields.h"
#include "defines.h"
#include "TwinkleFOX.h"

 #define NumberFieldType "Number"
 #define BooleanFieldType "Boolean"
 #define SelectFieldType "Select"
 #define ColorFieldType "Color"
 #define SectionFieldType "Section"


Fields::Fields(Settings& settings, Patterns& patterns) 
  : settings(settings), patterns(patterns)
{
  fieldsMap["power"] = { "power", "Power", BooleanFieldType, 0, 1 };
  fieldsMap["brightness"] = { "brightness", "Brightness", NumberFieldType, 1, 255 };
  
  fieldsMap["pattern"] = { "pattern", "Pattern", SelectFieldType, 0, patterns.patternCount, std::bind(&Fields::getPattern, this), std::bind(&Fields::getPatterns, this) };
  /* TODO
  fieldsMap["palette"] = { "palette", "Palette", SelectFieldType, 0, Patterns::paletteCount, getPalette, getPalettes };
  fieldsMap["speed"] = { "speed", "Speed", NumberFieldType, 1, 255, getSpeed };
  fieldsMap["autoplay"] = { "autoplay", "Autoplay", SectionFieldType };
  fieldsMap["autoplayDuration"] = { "autoplayDuration", "Autoplay Duration", NumberFieldType, 0, 255, getAutoplayDuration };
  fieldsMap["solidColorSection"] = { "solidColor", "Solid Color", SectionFieldType };
  fieldsMap["solidColor"] = { "solidColor", "Color", ColorFieldType, 0, 255, getSolidColor };
  fieldsMap["fire"] = { "fire", "Fire & Water", SectionFieldType };
  fieldsMap["cooling"] = { "cooling", "Cooling", NumberFieldType, 0, 255, getCooling };
  fieldsMap["sparking"] = { "sparking", "Sparking", NumberFieldType, 0, 255, getSparking };
  fieldsMap["twinkles"] = { "twinkles", "Twinkles", SectionFieldType };
  fieldsMap["twinkleSpeed"] = { "twinkleSpeed", "Twinkle Speed", NumberFieldType, 0, 8, getTwinkleSpeed };
  fieldsMap["twinkleDensity"] = { "twinkleDensity", "Twinkle Density", NumberFieldType, 0, 8, getTwinkleDensity };
  */
  fieldCount = fieldsMap.size();
}


String Fields::getPower() {
  return String(settings.power);
}

String Fields::setBrightness(String value) {
 settings.brightness = value.toInt();
 if(settings.brightness < 0) settings.brightness = 0;
 else if (settings.brightness > 255) settings.brightness = 255;
 return String(settings.brightness);
}

String Fields::setPower(String value) {
    settings.power = value.toInt();
    if(settings.power < 0) settings.power = 0;
    else if (settings.power > 1) settings.power = 1;
    return String(settings.power);
  }

String Fields::getBrightness() {
  return String(settings.brightness);
}

String Fields::getPattern() {
  return String(settings.currentPatternIndex);
}

String Fields::getPatterns() {
  String json = "";

  for (uint8_t i = 0; i < patterns.patternCount; i++) {
    json += "\"" + patterns.patternsVector[i].name + "\"";
    if (i < patterns.patternCount - 1)
      json += ",";
  }

  return json;
}

String Fields::getPalette() {
  return String(settings.currentPaletteIndex);
}

String Fields::getPalettes() {
  String json = "";

  for (uint8_t i = 0; i < patterns.paletteCount; i++) {
    json += "\"" + patterns.paletteNames[i] + "\"";
    if (i < patterns.paletteCount - 1)
      json += ",";
  }

  return json;
}

String Fields::getAutoplay() {
  return String(settings.autoplay);
}

String Fields::getAutoplayDuration() {
  return String(settings.autoplayDuration);
}

String Fields::getSolidColor() {
  return String(settings.solidColor.r) + "," + String(settings.solidColor.g) + "," + String(settings.solidColor.b);
}

String Fields::getCooling() {
  return String(settings.cooling);
}

String Fields::getSparking() {
  return String(settings.sparking);
}

String Fields::getSpeed() {
  return String(settings.speed);
}

String Fields::getTwinkleSpeed() {
  return String(settings.twinkleSpeed);
}

String Fields::getTwinkleDensity() {
  return String(settings.twinkleDensity);
}


const String Fields::getFieldsJson() {
  String json = "[";

  for (std::pair<std::string, Field> fieldNode : fieldsMap) {

    Field field = fieldNode.second;

    json += "{\"name\":\"" + field.name + "\",\"label\":\"" + field.label + "\",\"type\":\"" + field.type + "\"";

    if(field.getValue) {
      if (field.type == ColorFieldType || field.type == "String") {
        json += ",\"value\":\"" + field.getValue() + "\"";
      }
      else {
        json += ",\"value\":" + field.getValue();
      }
    }

    if (field.type == NumberFieldType) {
      json += ",\"min\":" + String(field.min);
      json += ",\"max\":" + String(field.max);
    }

    if (field.getOptions) {
      json += ",\"options\":[";
      json += field.getOptions();
      json += "]";
    }

    json += "}";

    //TODO if (i < fieldCount - 1)
    json += ",";
  }

  json += "]";

  return json;
}


Fields::Field Fields::getField(String name) {
  if(fieldsMap.find(name.c_str()) != fieldsMap.end()) {
    return fieldsMap.at(name.c_str());
  }
  return Field();
}

String Fields::getFieldValue(String name) {
  Field field = getField(name);
  if (field.getValue) {
    return field.getValue();
  }
  return String();
/*
   { "autoplayDuration", "Autoplay Duration", Fields::NumberFieldType, 0, 255, getAutoplayDuration },
      { "solidColor", "Solid Color", SectionFieldType },
      { "solidColor", "Color", ColorFieldType, 0, 255, getSolidColor },
      { "fire", "Fire & Water", SectionFieldType },
      { "cooling", "Cooling", NumberFieldType, 0, 255, getCooling },
      { "sparking", "Sparking", NumberFieldType, 0, 255, getSparking },
      { "twinkles", "Twinkles", SectionFieldType },
      { "twinkleSpeed", "Twinkle Speed", NumberFieldType, 0, 8, getTwinkleSpeed },
      { "twinkleDensity", "Twinkle Density", NumberFieldType, 0, 8, getTwinkleDensity },
  */
}

String Fields::setFieldValue(String name, String value) {
  Field field = getField(name);
  if (field.setValue) {
    return field.setValue(value);
  }
  return String();
}
