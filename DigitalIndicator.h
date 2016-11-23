// DigitalIndicator.h
#ifndef __DigitalIndicator_h__
#define __DigitalIndicator_h__

  /*
   * DigitalIndicator
   */

  #define DIGITALINDICATOR_DEFAULT_PIN -1

  class DigitalIndicator {
    int digitalPin;
    bool isLit;
    int instanceIndex;
    char instanceName[BM_CLASS_NAME_SIZE];
    bool pinActive;

    void generate_instance_name(char *intoVar);
    bool init(char *aName, int pin);

    public:
      DigitalIndicator::DigitalIndicator();
      DigitalIndicator::DigitalIndicator(char *aName);
      DigitalIndicator::DigitalIndicator(char *aName, int newDigitalPin);
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

