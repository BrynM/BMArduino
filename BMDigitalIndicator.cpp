#include "Arduino.h"
#include <BMArduino.h>
#include "BMDigitalIndicator.h"

int digitalIndicatorCount = 0;
char defaultDigitalIndicatorName[] = "BMDigitalIndicator-";

BMDigitalIndicator::BMDigitalIndicator() {
  init("", DIGITALINDICATOR_DEFAULT_PIN);
}
BMDigitalIndicator::BMDigitalIndicator(char *aName) {
  init(aName, DIGITALINDICATOR_DEFAULT_PIN);
}
BMDigitalIndicator::BMDigitalIndicator(char *aName, int pin) {
  init(aName, pin);
}
void BMDigitalIndicator::generate_instance_name(char *intoVar) {
  char buffer[10];
  itoa(instanceIndex, buffer, 10);
  strcpy(intoVar, defaultDigitalIndicatorName);
  strcat(intoVar, buffer);
}
bool BMDigitalIndicator::init(char *aName, int pin) {
  instanceIndex = digitalIndicatorCount;
  digitalIndicatorCount++;

  if(!set_name(aName)) {
    generate_instance_name(instanceName);
  }

  set_digital_pin(pin);
  pinActive = false;
  isLit = false;
}

bool BMDigitalIndicator::activate_pin() {
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

void BMDigitalIndicator::begin() {
  activate_pin();
}
void BMDigitalIndicator::begin(char *aName) {
  set_name(aName);
  begin();
}
void BMDigitalIndicator::begin(char *aName, int pin) {
  set_name(aName);
  set_digital_pin(pin);
  begin();
}

bool BMDigitalIndicator::is_lit() {
  return isLit;
}
bool BMDigitalIndicator::is_lit(bool newStatus) {
  if(newStatus) {
    digitalWrite(digitalPin, HIGH);
    isLit = true;
  } else {
    digitalWrite(digitalPin, LOW);
    isLit = false;
  }

  return isLit;
}

bool BMDigitalIndicator::set_digital_pin(int newDigitalPin) {
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

  digitalPin = DIGITALINDICATOR_DEFAULT_PIN;

  if(newDigitalPin == DIGITALINDICATOR_DEFAULT_PIN) {
    // desired an emptying of the pin
    return true;
  }

  return false;
}

bool BMDigitalIndicator::set_name(char *aName) {
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

