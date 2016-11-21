// bpm_utils.h
#ifndef __BMArduino__
#define __BMArduino__

  #include "BMBoards.h"

  #define BM_DEBUG_LEVEL 1
  #define BM_SERIAL_TIMEOUT 3500
  #define BM_SERIAL_RATE 9600
  #define FUNC_ANNOUNCE_PREFIX "@"
  #define FUNC_ANNOUNCE_SUFFIX " # "
  #define BPMBASE_NAME_SIZE 36

  #ifndef BM_MAX_PINS
    // could be defined by BMBoards
    #define BM_MAX_PINS 96
  #endif

  class BMCore {
    protected:
      int bm_used_pins[BM_MAX_PINS];

    public:
      BMCore::BMCore();
      void info();
      void announce();
      void announce(char *funcName);
      void announce(char *className, char *funcName);
      void serial_timestamp();
      void serial_timestamp(char *intoVar);
      bool pin_activate(int pin, int ioMode);
      bool value_is_between(int value, int lowerBound, int upperBound);
      bool value_is_near(int val, int near);
      bool value_is_near(int val, int near, int variance);
      void wait_for_serial();
      void wait_for_serial(bool startSerial);
  };

  extern BMCore BM;

  #define FUNC_MSG(f) \
    ( \
      (BMCore::announce(f)), \
      (void)0 \
    )

  #define CLASS_MSG(c, f) \
    ( \
      (BMCore::announce(c, f)), \
      (void)0 \
    )

  #if defined (BM_DEBUG_LEVEL) && BM_DEBUG_LEVEL > 1
    #define BM_DEBUGGING 1
    #define BM_NOISY_DEBUGGING 1
  #elif defined (BM_DEBUG_LEVEL) && BM_DEBUG_LEVEL > 0
    #define BM_DEBUGGING 1
  #endif
#endif
