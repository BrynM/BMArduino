// BMParallax3Wire_characters.h
#ifndef __BMParallax3Wire_characters_h__
#define __BMParallax3Wire_characters_h__

  /*
   * Arrow characters
   */
  const PROGMEM uint8_t parallaxCharArrowUp[8] = {
    B00000000,
    B00000100,
    B00001110,
    B00010101,
    B00000100,
    B00000100,
    B00000000,
    B00000000
  };
  const PROGMEM uint8_t parallaxCharArrowDown[8] = {
    B00000000,
    B00000100,
    B00000100,
    B00010101,
    B00001110,
    B00000100,
    B00000000,
    B00000000
  };
  const PROGMEM uint8_t parallaxCharArrowLeft[8] = {
    B00000000,
    B00000100,
    B00001000,
    B00011111,
    B00001000,
    B00000100,
    B00000000,
    B00000000
  };
  const PROGMEM uint8_t parallaxCharArrowRight[8] = {
    B00000000,
    B00000100,
    B00000010,
    B00011111,
    B00000010,
    B00000100,
    B00000000,
    B00000000
  };

  /*
   * Inverted arrow characters
   */
  const PROGMEM uint8_t parallaxCharArrowUpInverted[8] = {
    B00011111,
    B00011011,
    B00010001,
    B00001010,
    B00011011,
    B00011011,
    B00011111,
    B00000000
  };
  const PROGMEM uint8_t parallaxCharArrowDownInverted[8] = {
    B00011111,
    B00011011,
    B00011011,
    B00001010,
    B00010001,
    B00011011,
    B00011111,
    B00000000
  };
  const PROGMEM uint8_t parallaxCharArrowLeftInverted[8] = {
    B00011111,
    B00011011,
    B00010111,
    B00000000,
    B00010111,
    B00011011,
    B00011111,
    B00000000
  };
  const PROGMEM uint8_t parallaxCharArrowRightInverted[8] = {
    B00011111,
    B00011011,
    B00011101,
    B00000000,
    B00011101,
    B00011011,
    B00011111,
    B00000000
  };

#endif
