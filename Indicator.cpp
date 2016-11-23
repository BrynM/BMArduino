#include "Arduino.h"
#include <BMArduino.h>
#include "Indicator.h"

/*
  class Indicator {
    uint8_t digitalPin;
    bool isLit;
    int instanceIndex;
    char instanceName[BM_CLASS_NAME_SIZE];
    bool pinActive;

    void generate_instance_name(char *intoVar);
    bool init(char *aName, int pin, double knownVal);

    public:
      Indicator::Indicator();
      Indicator::Indicator(uint8_t newDigitalPin);
      void begin();
      void begin(char *aName);
      void begin(char *aName, uint8_t pin);
      bool is_lit();
      bool is_lit(bool newStatus);
      bool set_name(char *aName);
      bool set_digital_pin(int newDigitalPin);
  };
*/

int indicatorCount = 0;
char defaultIndicatorName[] = "Indicator-";

Indicator::Indicator() {
  init("", INDICATOR_DEFAULT_PIN);
}
Indicator::Indicator(char *aName) {
  init(aName, INDICATOR_DEFAULT_PIN);
}
Indicator::Indicator(char *aName, int pin) {
  init(aName, pin);
}
void Indicator::generate_instance_name(char *intoVar) {
  char buffer[10];
  itoa(instanceIndex, buffer, 10);
  strcpy(intoVar, defaultIndicatorName);
  strcat(intoVar, buffer);
}
bool Indicator::init(char *aName, int pin) {
  instanceIndex = indicatorCount;
  indicatorCount++;

  if(!set_name(aName)) {
    generate_instance_name(instanceName);
  }

  set_digital_pin(pin);
  pinActive = false;
  isLit = false;
}

bool Indicator::activate_pin() {
  if(pinActive) {
    #ifdef BM_DEBUGGING
      CLASS_MSG(instanceName);
      Serial.print(F("Digital pin "));
      Serial.print(digitalPin);
      Serial.println(F(" already active."));
    #endif

    return pinActive;
  }

  if(digitalPin > -1) {
    #ifdef BM_DEBUGGING
      CLASS_MSG(instanceName);
      Serial.print(F("Activating digital pin "));
      Serial.println(digitalPin);
    #endif

    pinActive = BM.pin_activate(digitalPin, OUTPUT);

    return pinActive;
  }

  #ifdef BM_DEBUGGING
    CLASS_MSG(instanceName);
    Serial.print(F("Digital pin "));
    Serial.print(digitalPin);
    Serial.println(F(" has not been set and cannot be activated."));
  #endif

  return false;
}

void Indicator::begin() {
  activate_pin();
}
void Indicator::begin(char *aName) {
  set_name(aName);
  begin();
}
void Indicator::begin(char *aName, int pin) {
  set_name(aName);
  set_digital_pin(pin);
  begin();
}

bool Indicator::is_lit() {
  return isLit;
}
bool Indicator::is_lit(bool newStatus) {
  if(newStatus) {
    digitalWrite(digitalPin, HIGH);
    isLit = true;
  } else {
    digitalWrite(digitalPin, LOW);
    isLit = false;
  }

  return isLit;
}

bool Indicator::set_digital_pin(int newDigitalPin) {
  if(newDigitalPin > -1) {
    if(newDigitalPin == digitalPin) {
      #ifdef BM_DEBUGGING
        CLASS_MSG(instanceName);
        Serial.print(F("Digital pin "));
        Serial.print(digitalPin);
        Serial.println(F(" unchanged."));
      #endif

      return true;
    }

    digitalPin = newDigitalPin;

    #ifdef BM_DEBUGGING
      CLASS_MSG(instanceName);
      Serial.print(F("Digital pin set to "));
      Serial.println(digitalPin);
    #endif

    return true;
  } 

  digitalPin = INDICATOR_DEFAULT_PIN;

  if(newDigitalPin == INDICATOR_DEFAULT_PIN) {
    // desired an emptying of the pin
    return true;
  }

  return false;
}

bool Indicator::set_name(char *aName) {
  if(strlen(aName) != 0) {
    #ifdef BM_DEBUGGING
      CLASS_MSG(instanceName);
      Serial.print(F("Changing name to \""));
      Serial.print(aName);
      Serial.println(F("\""));
    #endif

    strcpy(instanceName, aName);
    return true;
  }

  return false;
}

