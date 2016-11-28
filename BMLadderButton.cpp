#include "Arduino.h"
#include <BMArduino.h>
#include "BMLadderButton.h"

int ladderButtonCount = 0;

BMLadderButton::BMLadderButton() {
  init_members(BMLADDERBUTTON_EMPTY_PIN, BMLADDERBUTTON_DEFAULT_PIN_COOLDOWN_MS);
}
BMLadderButton::BMLadderButton(char *lbName) {
  init_members(lbName, BMLADDERBUTTON_EMPTY_PIN, BMLADDERBUTTON_DEFAULT_PIN_COOLDOWN_MS);
}
BMLadderButton::BMLadderButton(int onPin) {
  init_members(onPin, BMLADDERBUTTON_DEFAULT_PIN_COOLDOWN_MS);
}
BMLadderButton::BMLadderButton(char *lbName, int onPin) {
  init_members(lbName, onPin, BMLADDERBUTTON_DEFAULT_PIN_COOLDOWN_MS);
}
void BMLadderButton::generate_instance_name(char *intoVar) {
  itoa(ladderButtonCount, intoVar, 10);
  strcat(intoVar, " BMLadderButton");
}
bool BMLadderButton::init_members(int onPin, int withCooldownMs) {
  lastPinCheck = millis();
  pinValue = 0;

  char dummyBuff[BM_CLASS_NAME_SIZE];

  return init_members(dummyBuff, onPin, withCooldownMs);
}
bool BMLadderButton::init_members(char *lbName, int onPin, int withCooldownMs) {
  bool failed = false;
  lastPinCheck = millis();
  pinValue = 0;

  set_name(lbName);

  ladderButtonIndex = ladderButtonCount;
  ladderButtonCount++;

  if(!set_analog_pin(onPin) || !set_cooldown_ms(withCooldownMs)) {
    failed = true;
  }

  #ifdef BM_DEBUGGING
    CLASS_MSG(instanceName);
    if(failed) {
      Serial.println(F("FAILED initialization."));
    } else {
      Serial.println(F("Finished initialization."));
    }
  #endif

  return failed;
}

bool BMLadderButton::activate() {
  if(!BM.pin_activate(analogPin, INPUT)) {
    pin_active(false);

    return false;
  }

  pin_active(true);

  return true;
}

bool BMLadderButton::add_trigger(bmLadderButtonTrigger trigger) {
  if(triggerCount < BMLADDERBUTTON_MAX_TRIGGERS) {
    triggers[triggerCount] = (bmLadderButtonTrigger){
      trigger.cooldown,
      trigger.value,
      trigger.variance,
      trigger.callback
    };
    triggerCount++;

    #ifdef BM_DEBUGGING
      CLASS_MSG(instanceName);
      Serial.print(F("Added trigger "));
      Serial.print(triggerCount);
      Serial.print(F(" with value "));
      Serial.print(trigger.value);
      Serial.print(F(" variance "));
      Serial.print(trigger.variance);
      Serial.print(F(" cooling "));
      Serial.println(trigger.cooldown);
    #endif

    return true;
  #ifdef BM_DEBUGGING
  } else {
    CLASS_MSG(instanceName);
    Serial.print(F("Cannot add trigger "));
    Serial.print(triggerCount);
    Serial.print(F(". Max exceeded "));
    Serial.println(BMLADDERBUTTON_MAX_TRIGGERS);
  #endif
  }

  return false;
}

void BMLadderButton::clear_triggers() {
  // just reset our counter
  triggerCount = 0;
}

bool BMLadderButton::pin_active() {
  return pinActive;
}
bool BMLadderButton::pin_active(bool setTo) {
  if(setTo) {
    pinActive = true;
  } else {
    pinActive = false;
  }

  #ifdef BM_DEBUGGING
    CLASS_MSG(instanceName);
    Serial.print(F("Set pin "));
    Serial.print(analogPin);
    Serial.print(F(" active state to "));
    Serial.println(pinActive);
  #endif

  return pinActive;
}

void BMLadderButton::poll() {
  // for use in a loop()
  int rawValue = read_pin_raw();
}

uint32_t BMLadderButton::read_pin_raw() {
  if(analogPin < 0) {
    #ifdef BM_DEBUGGING
      CLASS_MSG(instanceName);
      Serial.print(F("Pin "));
      Serial.print(analogPin);
      Serial.print(F(" is invalid (too low)."));
    #endif

    return -1;
  }

  if(!pin_active()) {
    #ifdef BM_NOISY_DEBUGGING
      CLASS_MSG(instanceName);
      Serial.print(F("Pin "));
      Serial.print(analogPin);
      Serial.print(F(" is not active ("));
      Serial.print(pinActive);
      Serial.println(F(")."));
    #endif

    return -1;
  }

  unsigned long elapsed = millis() - lastPinCheck;

  if(elapsed < cooldownMsPin) {
    #ifdef BM_VERY_NOISY_DEBUGGING
      CLASS_MSG(instanceName);
      Serial.print(F("Pin "));
      Serial.print(analogPin);
      Serial.print(F(" is still cooling down ("));
      Serial.print(elapsed);
      Serial.print(F(" < "));
      Serial.print(cooldownMsPin);
      Serial.println(F(")."));
    #endif

    return pinValue;
  }

  uint32_t pinCurr = analogRead(analogPin);

  if(!BM.value_is_near(pinCurr, pinValue)) {
    #ifdef BM_NOISY_DEBUGGING
      CLASS_MSG(instanceName);
      Serial.print(F("Reading pin "));
      Serial.print(analogPin);
      Serial.print(F(" Was: "));
      Serial.print(pinValue);
    #endif

    pinValue = pinCurr;
  
    #ifdef BM_NOISY_DEBUGGING
      Serial.print(F(" Is: "));
      Serial.println(pinValue);
    #endif

    trigger_value_change(pinValue);

  #ifdef BM_VERY_NOISY_DEBUGGING
  } else {
    CLASS_MSG(instanceName);
    Serial.print(F("Pin "));
    Serial.print(analogPin);
    Serial.print(F(" has not changed value ("));
    Serial.print(pinValue);
    Serial.println(F(")."));
  #endif
  }

  lastPinCheck = millis();

  return pinValue;
}

bool BMLadderButton::set_analog_pin(int newAnalogPin) {
  if(newAnalogPin > -1) {
    if(newAnalogPin == analogPin) {
      #ifdef BM_DEBUGGING
        CLASS_MSG(instanceName);
        Serial.print(F("Analog pin "));
        Serial.print(analogPin);
        Serial.println(F(" unchanged."));
      #endif

      return true;
    }

    analogPin = newAnalogPin;

    #ifdef BM_DEBUGGING
      CLASS_MSG(instanceName);
      Serial.print(F("Analog pin set to "));
      Serial.println(analogPin);
    #endif

    return true;
  } 

  analogPin = BMLADDERBUTTON_EMPTY_PIN;

  if(newAnalogPin == BMLADDERBUTTON_EMPTY_PIN) {
    // desired an emptying of the pin
    return true;
  }

  return false;
}

bool BMLadderButton::set_cooldown_ms(int newCoolMs) {
  if(newCoolMs > -1) {
    if(newCoolMs == cooldownMsPin) {
      #ifdef BM_DEBUGGING
        CLASS_MSG(instanceName);
        Serial.print(F("Cooldown on pin "));
        Serial.print(analogPin);
        Serial.print(F(" unchanged "));
        Serial.println(cooldownMsPin);
      #endif
      return false;
    }

    cooldownMsPin = newCoolMs;

    return true;
  }

  cooldownMsPin = BMLADDERBUTTON_DEFAULT_PIN_COOLDOWN_MS;

  #ifdef BM_DEBUGGING
    CLASS_MSG(instanceName);
    Serial.print(F("Cooldown on pin "));
    Serial.print(analogPin);
    Serial.print(F(" fell back to default cooldown milliseconds "));
    Serial.println(cooldownMsPin);
  #endif

  return false;
}

bool BMLadderButton::set_name(char *lbName) {
  if(strcmp (instanceName, lbName) == 0) {
    // already called that
    return true;
  }

  if(strlen(lbName) != 0) {
    strcpy(instanceName, lbName);

    return true;
  }

  generate_instance_name(instanceName);

  return true;
}

bool BMLadderButton::setup() {
  if(!activate()) {
    #ifdef BM_DEBUGGING
      CLASS_MSG(instanceName);
      Serial.print(F("Failed to activate analog pin "));
      Serial.println(analogPin);
    #endif

    return false;
  }

  return true;
}
bool BMLadderButton::setup(char *lbName) {
  set_name(lbName);
  return setup();
}
bool BMLadderButton::setup(int onPin) {
  set_analog_pin(onPin);
  return setup();
}
bool BMLadderButton::setup(char *lbName, int onPin) {
  set_name(lbName);
  set_analog_pin(onPin);
  return setup();
}

void BMLadderButton::trigger_value_change(uint32_t newValue) {
  unsigned long timestamp = millis();
  unsigned long delta;

  if(triggerCount < 1) {
    #ifdef BM_NOISY_DEBUGGING
      CLASS_MSG(instanceName);
      Serial.print(F("No triggers to fire for change "));
      Serial.println(newValue);
    #endif

    return;
  }

  for(int i = 0; i < triggerCount; i++) {
    if(BM.value_is_near(newValue, triggers[i].value, triggers[i].variance)) {
      delta = timestamp - triggers[i].lastTriggered;

      if(triggers[i].lastTriggered < 1 || (delta > triggers[i].cooldown)) {
        #ifdef BM_DEBUGGING
          CLASS_MSG(instanceName);
          Serial.print(F("Matched trigger "));
          Serial.print(triggers[i].value);
          Serial.print(F(" near "));
          Serial.println(newValue);
        #endif

        triggers[i].lastTriggered = timestamp;
        triggers[i].callback(newValue, triggers[i]);
      #ifdef BM_DEBUGGING
      } else {
        CLASS_MSG(instanceName);
        Serial.print(F("Cooling trigger "));
        Serial.print(triggers[i].value);
        Serial.print(F(" near "));
        Serial.print(newValue);
        Serial.print(F(" for "));
        Serial.print(delta);
        Serial.print(F("/"));
        Serial.println(triggers[i].cooldown);
      #endif
      }

    }
  }
}

