// BMParallax3Wire.h
#ifndef __BMParallax3Wire_h__
#define __BMParallax3Wire_h__

  // https://www.parallax.com/sites/default/files/downloads/27979-Parallax-Serial-LCDs-Product-Guide-v3.1.pdf

  #include "Arduino.h"
  #include <SoftwareSerial.h>
  #include "lib/BMParallax3Wire_characters.h"

  #define BMPARALLAX3WIRE_CMD_BACKLIGHT_OFF 18
  #define BMPARALLAX3WIRE_CMD_BACKLIGHT_ON 17
  // clear is decimal 12 hex 0x0C
  #define BMPARALLAX3WIRE_CMD_CLEAR 12
  // per manual, 5 ms delay after calling clear
  #define BMPARALLAX3WIRE_CMD_CLEAR_DELAY 5
  #define BMPARALLAX3WIRE_CMD_POWER_OFF 21

  #define BMPARALLAX3WIRE_COORDS_FIRST_INDEX 128
  #define BMPARALLAX3WIRE_COORDS_MAX_ROWS 4
  #define BMPARALLAX3WIRE_COORDS_MAX_COLS 20

  // index of the startup mode
  #define BMPARALLAX3WIRE_MODE_DEFAULT 3

  #define BMPARALLAX3WIRE_NOTE_CHAR_SIZE 4
  #define BMPARALLAX3WIRE_NOTE_COUNT 13
  #define BMPARALLAX3WIRE_NOTE_FIRST_INDEX 220

  #define BMPARALLAX3WIRE_SCALE_COUNT 5
  #define BMPARALLAX3WIRE_SCALE_DEFAULT 4
  #define BMPARALLAX3WIRE_SCALE_FIRST_INDEX 215

  // the hardware is not avilable to write right away
  #define BMPARALLAX3WIRE_WARMUP_DELAY 10

  // some human-readable labels for the
  // regular custom character indexes
  #define PLX_CHAR_A 0
  #define PLX_CHAR_B 1
  #define PLX_CHAR_C 2
  #define PLX_CHAR_D 3
  #define PLX_CHAR_E 4
  #define PLX_CHAR_F 5
  #define PLX_CHAR_G 6
  #define PLX_CHAR_H 7

  // some human-readable labels for the
  // arrow custom character indexes
  // must use load_arrows() first
  #define PLX_ARROW_UP    0
  #define PLX_ARROW_DOWN  1
  #define PLX_ARROW_LEFT  2
  #define PLX_ARROW_RIGHT 3
  #define PLX_ARROW_NEG_UP    4
  #define PLX_ARROW_NEG_DOWN  5
  #define PLX_ARROW_NEG_LEFT  6
  #define PLX_ARROW_NEG_RIGHT 7

  struct modeFlags {
    char designation[4];
    bool blink;
    bool cursor;
    uint8_t byte;
  };

  class BMParallax3Wire : public SoftwareSerial {
    modeFlags currMode;
    bool backlightLit;

    int find_beat_index(int noteLength);
    int find_note_index(char *note);
    int find_scale_index(int scale);

    public:
      BMParallax3Wire::BMParallax3Wire(int rx, int tx);
      void begin(long baud);

      bool backlight();
      bool backlight(int setLight);
      void clear();
      void load_arrows(); // load arrow characters
      modeFlags mode();
      modeFlags mode(bool blink, bool cursor);
      void off();
      void play_note(char *note); // assumes default scale
      void play_note(char *note, int scale);
      // set the length of a note 1 for whole, 2 for half, 4 for quarter, etc. 
      bool set_beat(int noteLength);
      bool set_cursor(int row, int col);
  };

#endif
