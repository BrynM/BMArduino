// bpm_utils.h
#ifndef __BMArduino__
#define __BMArduino__

  #include "BMBoards.h"

  #define BM_CLASS_NAME_SIZE 36

  #ifndef BM_MAX_PINS
    // could be defined by BMBoards
    #define BM_MAX_PINS 96
  #endif

  #ifndef BM_DEBUG_LEVEL
    #define BM_DEBUG_LEVEL 1
  #endif

  #ifndef BM_FUNC_ANNOUNCE_PREFIX_SUFFIX_SIZE
    #define BM_FUNC_ANNOUNCE_PREFIX_SUFFIX_SIZE 8
  #endif

  #ifndef BM_FUNC_ANNOUNCE_PREFIX
    #define BM_FUNC_ANNOUNCE_PREFIX "@"
  #endif

  #ifndef BM_FUNC_ANNOUNCE_SUFFIX
    #define BM_FUNC_ANNOUNCE_SUFFIX " # "
  #endif

  #ifndef BM_SERIAL_RATE
    #define BM_SERIAL_RATE 9600
  #endif

  #ifndef BM_SERIAL_TIMEOUT
    #define BM_SERIAL_TIMEOUT 3500
  #endif

  class BMCore {
    protected:
      int bmUsedPins[BM_MAX_PINS];
      char bmFuncAnnouncePrefix[BM_FUNC_ANNOUNCE_PREFIX_SUFFIX_SIZE];
      char bmFuncAnnounceSuffix[BM_FUNC_ANNOUNCE_PREFIX_SUFFIX_SIZE];
      int bmSerialRate;
      int bmSerialTimeout;
      bool bmStartedSerial;

    public:
      BMCore::BMCore();
      void announce();
      void announce(char *funcName);
      void announce(char *className, char *funcName);
      void info();
      bool pin_activate(int pin, int ioMode);
      bool value_is_between(int value, int lowerBound, int upperBound);
      bool value_is_near(int val, int near);
      bool value_is_near(int val, int near, int variance);
      void wait_for_serial();
      void wait_for_serial(bool startSerial);
  };

  extern BMCore BM;

  #define FUNC_MSG() \
    ( \
      (BM.announce(__func__)), \
      (void)0 \
    )

  #define CLASS_MSG(c) \
    ( \
      (BM.announce(c, __func__)), \
      (void)0 \
    )

  #if defined (BM_DEBUG_LEVEL) && BM_DEBUG_LEVEL > 1
    #define BM_DEBUGGING 1
    #define BM_NOISY_DEBUGGING 1
  #elif defined (BM_DEBUG_LEVEL) && BM_DEBUG_LEVEL > 0
    #define BM_DEBUGGING 1
  #endif
#endif
