#include "Arduino.h"
#include "BMArduino.h"
#include "BMParallax3Wire.h"

#define BMPARALLAX3WIRE_MODE_COUNT 4
modeFlags parallax3WireModes[4] = {
  {"bc", true, true, 0x19}, // 25 dec - blinkCursor
  {"bNc", true, false, 0x17}, // 23 dec - blinkNoCursor
  {"Nbc", false, true, 0x18}, // 24 dec - noBlinkCursor
  {"NbNc", false, false, 0x16} // 22 dec - noBlinkNoCursor
};

int bmParallax3WireValidScales[BMPARALLAX3WIRE_SCALE_COUNT] = {
  3,
  4,
  5,
  6,
  7
};

char bmParallax3WireValidNotes[BMPARALLAX3WIRE_NOTE_COUNT][BMPARALLAX3WIRE_NOTE_CHAR_SIZE] = {
  "A",
  "A#",
  "B",
  "C",
  "C#",
  "D",
  "D#",
  "E",
  "F",
  "F#",
  "G",
  "G#",
  // empty for rest/silent beat duration
  ""
};

// for the musical functionality - note length control
#define BMPARALLAX3WIRE_BEAT_FIRST_INDEX 208
// these are reversed so their indexes can be used for addition
int bmParallax3WireValidBeats[7] = {64, 32, 16, 8, 4, 2, 1};

#ifdef BM_DEBUGGING
  char bmParallax3WireClassName[BM_CLASS_NAME_SIZE] = "BMParallax3Wire";
#endif

BMParallax3Wire::BMParallax3Wire(int rx, int tx)
  : SoftwareSerial(rx, tx) {
};
void BMParallax3Wire::begin(long baud) {
  SoftwareSerial::begin(baud);
  #ifdef BM_DEBUGGING
    CLASS_MSG(bmParallax3WireClassName);
    Serial.print(F("Began SoftwareSerial at "));
    Serial.println(baud);
  #endif

  #ifdef BM_NOISY_DEBUGGING
    CLASS_MSG(bmParallax3WireClassName);
    Serial.print(F("Sleeping for hardware "));
    Serial.println(BMPARALLAX3WIRE_WARMUP_DELAY);
  #endif
  delay(BMPARALLAX3WIRE_WARMUP_DELAY); // required by hardware

  backlight(false);

  if(BMPARALLAX3WIRE_MODE_DEFAULT < BMPARALLAX3WIRE_MODE_COUNT) {
    currMode = parallax3WireModes[BMPARALLAX3WIRE_MODE_DEFAULT];
  } else {
    // fall back to first index
    currMode = parallax3WireModes[0];

    #ifdef BM_DEBUGGING
      CLASS_MSG(bmParallax3WireClassName);
      Serial.print(F("Falling back to first indexed mode! "));
      Serial.println(currMode.designation);
    #endif
  }

  mode(currMode.blink, currMode.cursor);
};

bool BMParallax3Wire::backlight() {
  return backlightLit;
}
bool BMParallax3Wire::backlight(int setLight) {
  if(setLight) {
    write(BMPARALLAX3WIRE_CMD_BACKLIGHT_ON);
    backlightLit = true;

    #ifdef BM_DEBUGGING
      CLASS_MSG(bmParallax3WireClassName);
      Serial.println(F("Backlight on."));
    #endif
  } else {
    write(BMPARALLAX3WIRE_CMD_BACKLIGHT_OFF);
    backlightLit = false;

    #ifdef BM_DEBUGGING
      CLASS_MSG(bmParallax3WireClassName);
      Serial.println(F("Backlight off."));
    #endif
  }
}

void BMParallax3Wire::clear() {
  write(BMPARALLAX3WIRE_CMD_CLEAR);
  #ifdef BM_NOISY_DEBUGGING
    CLASS_MSG(bmParallax3WireClassName);
    Serial.println(F("Clear."));
    CLASS_MSG(bmParallax3WireClassName);
    Serial.print(F("Sleeping for clear "));
    Serial.println(BMPARALLAX3WIRE_CMD_CLEAR_DELAY);
  #endif
  delay(BMPARALLAX3WIRE_CMD_CLEAR_DELAY); // required by hardware
}

int BMParallax3Wire::find_beat_index(int noteLength) {
  int beatCount = sizeof(bmParallax3WireValidBeats) / sizeof(int);

  for(int i = 0; i < beatCount; i++) {
    if(bmParallax3WireValidBeats[i] == noteLength) {
      return i;
    }
  }

  return -1;
}

int BMParallax3Wire::find_note_index(int note) {
  for(int i = 0; i < BMPARALLAX3WIRE_NOTE_COUNT; i++) {
    if(strcmp(bmParallax3WireValidNotes[i], note) == 0) {
      return i;
    }
  }

  return -1;
}

int BMParallax3Wire::find_scale_index(int scale) {
  int scaleCount = sizeof(bmParallax3WireValidScales) / sizeof(int);

  for(int i = 0; i < scaleCount; i++) {
    if(bmParallax3WireValidScales[i] == scale) {
      return i;
    }
  }

  return -1;
}

void BMParallax3Wire::load_arrows() {
  #ifdef BM_DEBUGGING
    CLASS_MSG(bmParallax3WireClassName);
    Serial.println(F("Loading arrow characters."));
  #endif

  for(int i = 0; i < 8; i++) {
    #ifdef BM_NOISY_DEBUGGING
      CLASS_MSG(bmParallax3WireClassName);
      Serial.print(F("Loading arrow character "));
      Serial.println(i);
    #endif

    write(248+i); // begin writing cust char
    for(int c = 0; c < 8; c++) {
      // sorry - indexes get funky in PROGMEM
      switch(i) {
        case 0:
          write(pgm_read_word_near(parallaxCharArrowUp + c));
          break;
        case 1:
          write(pgm_read_word_near(parallaxCharArrowDown + c));
          break;
        case 2:
          write(pgm_read_word_near(parallaxCharArrowLeft + c));
          break;
        case 3:
          write(pgm_read_word_near(parallaxCharArrowRight + c));
          break;
        case 4:
          write(pgm_read_word_near(parallaxCharArrowUpInverted + c));
          break;
        case 5:
          write(pgm_read_word_near(parallaxCharArrowDownInverted + c));
          break;
        case 6:
          write(pgm_read_word_near(parallaxCharArrowLeftInverted + c));
          break;
        case 7:
          write(pgm_read_word_near(parallaxCharArrowRightInverted + c));
          break;
      }
    }
  }
}

modeFlags BMParallax3Wire::mode() {
  // just spit back
  return currMode;
}
modeFlags BMParallax3Wire::mode(bool blink, bool cursor) {
  off();

  for(int i = 0; i < BMPARALLAX3WIRE_MODE_COUNT; i++) {
    if(parallax3WireModes[i].byte > 0) {
      if(parallax3WireModes[i].blink == blink && parallax3WireModes[i].cursor == cursor) {
        currMode = parallax3WireModes[i];
        break;
      }
    }
  }

  if(currMode.byte > 0) {
    #ifdef BM_DEBUGGING
      CLASS_MSG(bmParallax3WireClassName);
      Serial.print(F("Setting LCD mode to "));
      Serial.println(currMode.designation);
    #endif
    write(currMode.byte);
  #ifdef BM_DEBUGGING
  } else {
    CLASS_MSG(bmParallax3WireClassName);
    Serial.print(F("Invalid LCD mode blink: "));
    Serial.print(blink);
    Serial.print(F(" cursor: "));
    Serial.println(cursor);
  #endif
  }

  return currMode;
}

void BMParallax3Wire::off() {
  // often does not physicially turn off... :/
  write(0x15);
}

void BMParallax3Wire::play_note(char *note) {
  play_note(note, BMPARALLAX3WIRE_SCALE_DEFAULT);
}
void BMParallax3Wire::play_note(char *note, int scale) {
  int scaleIndex = find_scale_index(scale);
  int noteIndex = find_note_index(note);

  if(noteIndex < 0 || scaleIndex < 0) {
    #ifdef BM_DEBUGGING
      CLASS_MSG(bmParallax3WireClassName);
      Serial.print(F("Invalid note or scale index. note: "));
      Serial.print(note);
      Serial.print(F(" index: "));
      Serial.print(noteIndex);
      Serial.print(F(" scale: "));
      Serial.print(scale);
      Serial.print(F(" index: "));
      Serial.print(scaleIndex);
    #endif
    return;
  }

  write(BMPARALLAX3WIRE_SCALE_FIRST_INDEX+scaleIndex);
  write(BMPARALLAX3WIRE_NOTE_FIRST_INDEX+noteIndex);
/*
215 D7 Select the 3rd scale (A = 220 Hz)
216 D8 Select the 4th scale (A = 440 Hz)
217 D9 Select the 5th scale (A = 880 Hz)
218 DA Select the 6th scale (A = 1760 Hz)
219 DB Select the 7th scale (A = 3520 Hz)
220 DC Play A note
221 DD Play A# note
222 DE Play B
223 DF Play C
224 E0 Play C#
225 E1 Play D
226 E2 Play D#
227 E3 Play E
228 E4 Play F
229 E5 Play F#
230 E6 Play G
231 E7 Play G#
232 E8 Pause for current note length (no sound) 
*/  
}

// set the length of a note 1 for whole, 2 for half, 4 for quarter, etc. 
bool BMParallax3Wire::set_beat(int noteLength) {
  int beatIndex = find_beat_index(noteLength);
  if(beatIndex < 0) {
    #ifdef BM_DEBUGGING
      CLASS_MSG(bmParallax3WireClassName);
      Serial.print(F("Invalid beat length "));
      Serial.println(noteLength);
    #endif

    return false;
  }

  write(BMPARALLAX3WIRE_BEAT_FIRST_INDEX+beatIndex);

  #ifdef BM_DEBUGGING
    CLASS_MSG(bmParallax3WireClassName);
    Serial.print(F("Set beat length "));
    Serial.print(noteLength);
    Serial.print(F(" index "));
    Serial.println(beatIndex);
  #endif

  return true;
}

bool BMParallax3Wire::set_cursor(int row, int col) {
  if(row >= BMPARALLAX3WIRE_COORDS_MAX_ROWS || col >= BMPARALLAX3WIRE_COORDS_MAX_COLS) {
    // out of range - ignore and bail
    return false;
  }

  int offset = (row * BMPARALLAX3WIRE_COORDS_MAX_COLS) + col;
  write(BMPARALLAX3WIRE_COORDS_FIRST_INDEX + offset);
  #ifdef BM_NOISY_DEBUGGING
    CLASS_MSG(bmParallax3WireClassName);
    Serial.print(F("Set cursor to "));
    Serial.print(row);
    Serial.print(F(", "));
    Serial.println(col);
  #endif

  return true;
}

