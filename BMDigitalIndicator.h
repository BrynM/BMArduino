// BMDigitalIndicator.h
#ifndef __BMDigitalIndicator_h__
#define __BMDigitalIndicator_h__

  /*
   * BMDigitalIndicator
   */

  #define DIGITALINDICATOR_DEFAULT_PIN -1

  class BMDigitalIndicator {
    int digitalPin;
    bool isLit;
    int instanceIndex;
    char instanceName[BM_CLASS_NAME_SIZE];
    bool pinActive;

    void generate_instance_name(char *intoVar);
    bool init(char *aName, int pin);

    public:
      BMDigitalIndicator::BMDigitalIndicator();
      BMDigitalIndicator::BMDigitalIndicator(char *aName);
      BMDigitalIndicator::BMDigitalIndicator(char *aName, int newDigitalPin);
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

