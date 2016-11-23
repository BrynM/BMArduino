// Indicator.h
#ifndef __Indicator_h__
#define __Indicator_h__

  /*
   * Indicator
   */

  #define INDICATOR_DEFAULT_PIN -1

  class Indicator {
    int digitalPin;
    bool isLit;
    int instanceIndex;
    char instanceName[BM_CLASS_NAME_SIZE];
    bool pinActive;

    void generate_instance_name(char *intoVar);
    bool init(char *aName, int pin);

    public:
      Indicator::Indicator();
      Indicator::Indicator(char *aName);
      Indicator::Indicator(char *aName, int newDigitalPin);
      bool activate_pin();
      void begin();
      void begin(char *aName);
      void begin(char *aName, int pin);
      bool is_lit();
      bool is_lit(bool newStatus);
      bool set_name(char *aName);
      bool set_digital_pin(int newDigitalPin);
  };

#endif

