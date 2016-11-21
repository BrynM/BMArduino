#include "Arduino.h"
#include "BMArduino.h"
#include "BMBoards.h"

BMCore::BMCore() {
  strcpy(bmFuncAnnounceMsPrefix, BM_ANNOUNCE_MS_PREFIX);
  strcpy(bmFuncAnnounceMsSuffix, BM_ANNOUNCE_MS_SUFFIX);
  strcpy(bmFuncAnnouncePrefix, BM_ANNOUNCE_PREFIX);
  strcpy(bmFuncAnnounceSuffix, BM_ANNOUNCE_SUFFIX);
  bmSerialRate = BM_SERIAL_RATE;
  bmSerialTimeout = BM_SERIAL_TIMEOUT;
  bmStartedSerial = false;
};

char bmInstanceName[BM_CLASS_NAME_SIZE] = "BM";

void BMCore::announce() {
  #ifdef BM_DEBUGGING
    Serial.print(bmFuncAnnouncePrefix);
    Serial.print(bmFuncAnnounceMsPrefix);
    Serial.print(millis());
    Serial.print(bmFuncAnnounceMsSuffix);
  #endif
}

void BMCore::announce(char *funcName) {
  #ifdef BM_DEBUGGING
    Serial.print(bmFuncAnnouncePrefix);
    Serial.print(bmFuncAnnounceMsPrefix);
    Serial.print(millis());
    Serial.print(bmFuncAnnounceMsSuffix);
    Serial.print(funcName);
    Serial.print("()");
    Serial.print(bmFuncAnnounceSuffix);
  #endif
}

void BMCore::announce(char *className, char *funcName) {
  #ifdef BM_DEBUGGING
    Serial.print(bmFuncAnnouncePrefix);
    Serial.print(bmFuncAnnounceMsPrefix);
    Serial.print(millis());
    Serial.print(bmFuncAnnounceMsSuffix);
    Serial.print(className);
    Serial.print("::");
    Serial.print(funcName);
    Serial.print("()");
    Serial.print(bmFuncAnnounceSuffix);
  #endif
}


void BMCore::info() {
  #ifdef BM_DEBUGGING
    Serial.print("########################################\n");
    Serial.print("# Compiled for ");
    Serial.print(__BM_BOARD__);
    Serial.print(" (");
    Serial.print(BM_MAX_PINS);
    Serial.println("p avail)");
    Serial.print("# ");
    Serial.print(__DATE__);
    Serial.print(" ");
    Serial.print(__TIME__);
    Serial.println();
    Serial.print("# libc ");
    Serial.print(__AVR_LIBC_DATE_STRING__);
    Serial.print(" ");
    Serial.print(__AVR_LIBC_VERSION_STRING__);
    Serial.println();
    Serial.print("########################################\n\n");
  #endif
}

bool BMCore::pin_activate(int pin, int ioMode) {
  int lastIter = 0;

  for(int i = 0; i < BM_MAX_PINS; i++) {
    if(bmUsedPins[i] == '\0') {
      lastIter = i;
      break;
    }

    if(bmUsedPins[i] == pin) {
      #ifdef BM_DEBUGGING
        CLASS_MSG(bmInstanceName);
        Serial.print("Failed pin ");
        Serial.print(pin);
        Serial.print(" slot ");
        Serial.println(i);
      #endif

      return false;
    }
  }

  bmUsedPins[lastIter] = pin;
  pinMode(pin, ioMode);

  #ifdef BM_DEBUGGING
    CLASS_MSG(bmInstanceName);
    Serial.print("Reserved pin ");
    Serial.print(pin);
    Serial.print(" slot ");
    Serial.print(lastIter);
    Serial.print(" (");
    Serial.print(BM_MAX_PINS - (lastIter + 1));
    Serial.println(" remain)");
  #endif

  return true;
}

bool BMCore::value_is_between(int value, int lowerBound, int upperBound) {
  return (value > lowerBound && value < upperBound);
}

bool BMCore::value_is_near(int val, int near) {
  return value_is_near(val, near, 10);
}
bool BMCore::value_is_near(int val, int near, int variance) {
  return (val >= (near - variance) && val <= (near+variance));
}

void BMCore::wait_for_serial() {
  int serialBreak = 0;
  int startMilliS = millis();

  while (!Serial && !serialBreak) {
    if((millis() - startMilliS) >= bmSerialTimeout) {
      serialBreak = 1;
    }
  }

  #ifdef BM_DEBUGGING
    Serial.println();
    CLASS_MSG(bmInstanceName);
    Serial.print("Hardware Serial ");
    if(serialBreak) {
      Serial.print("timed out after ");
      Serial.print(bmSerialTimeout);
      Serial.println("!");
    } else {
      if(bmStartedSerial) {
        Serial.print("begun at ");
        Serial.print(bmSerialRate);
        Serial.println(" baud.");
      } else {
        Serial.print("begun.");
      }
    }
  #endif


}
void BMCore::wait_for_serial(bool startSerial) {
  if(startSerial) {
    Serial.begin(bmSerialRate);
    bmStartedSerial = true;
  }

  return wait_for_serial();
}

BMCore BM;
