// BMArduino.h
#ifndef __BMArduino_h__
#define __BMArduino_h__

  /*
   * These can be uncommented and adjusted.
   * Note that prefixes and suffixes have limited length.
   * The values here were left as the defaults for use as examples.
   */

  //#define BM_DEBUG_LEVEL 1

  //#define BM_ANNOUNCE_SUFFIX_PREFIX_SIZE 8

  //#define BM_ANNOUNCE_MS_PREFIX "@"

  //#define BM_ANNOUNCE_MS_SUFFIX "ms # "

  //#define BM_ANNOUNCE_PREFIX ""

  //#define BM_ANNOUNCE_SUFFIX " # "

  //#define BM_SERIAL_RATE 9600

  //#define BM_SERIAL_TIMEOUT 3500

  //#define BM_VALUE_NEAR_DEFAULT_VARIANCE 10

#endif

/*
 * Don't edit below here
 */

#ifndef __BMArduino_h_BMCore__
#define __BMArduino_h_BMCore__

  #include "lib/BMBoards.h"

  #ifndef BM_ANNOUNCE_MS_PREFIX
    #define BM_ANNOUNCE_MS_PREFIX "@"
  #endif

  #ifndef BM_ANNOUNCE_MS_SUFFIX
    #define BM_ANNOUNCE_MS_SUFFIX "ms # "
  #endif

  #ifndef BM_ANNOUNCE_PREFIX
    #define BM_ANNOUNCE_PREFIX ""
  #endif

  #ifndef BM_ANNOUNCE_SUFFIX
    #define BM_ANNOUNCE_SUFFIX " # "
  #endif

  #ifndef BM_ANNOUNCE_SUFFIX_PREFIX_SIZE
    #define BM_ANNOUNCE_SUFFIX_PREFIX_SIZE 8
  #endif

  #ifndef BM_CLASS_NAME_SIZE
    #define BM_CLASS_NAME_SIZE 36
  #endif

  #ifndef BM_DEBUG_LEVEL
    #define BM_DEBUG_LEVEL 1
  #endif

  #ifndef BM_MAX_PINS
    // could be defined by BMBoards
    #define BM_MAX_PINS 96
  #endif

  #ifndef BM_SERIAL_RATE
    #define BM_SERIAL_RATE 9600
  #endif

  #ifndef BM_SERIAL_TIMEOUT
    #define BM_SERIAL_TIMEOUT 3500
  #endif

  #ifndef BM_VALUE_NEAR_DEFAULT_VARIANCE
    #define BM_VALUE_NEAR_DEFAULT_VARIANCE 10
  #endif

  class BMCore {
    protected:
      int bmUsedPins[BM_MAX_PINS];
      char bmFuncAnnounceMsPrefix[BM_ANNOUNCE_SUFFIX_PREFIX_SIZE];
      char bmFuncAnnounceMsSuffix[BM_ANNOUNCE_SUFFIX_PREFIX_SIZE];
      char bmFuncAnnouncePrefix[BM_ANNOUNCE_SUFFIX_PREFIX_SIZE];
      char bmFuncAnnounceSuffix[BM_ANNOUNCE_SUFFIX_PREFIX_SIZE];
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

  #ifdef BM_DEBUG_LEVEL
    #if BM_DEBUG_LEVEL > 2
      #define BM_DEBUGGING
      #define BM_NOISY_DEBUGGING
      #define BM_VERY_NOISY_DEBUGGING
    #elif BM_DEBUG_LEVEL > 1
      #define BM_DEBUGGING
      #define BM_NOISY_DEBUGGING
    #elif BM_DEBUG_LEVEL > 0
      #define BM_DEBUGGING
    #endif
  #endif
#endif
