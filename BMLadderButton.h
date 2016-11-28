// BMLadderButton.h
#ifndef __BMLadderButton_h__
#define __BMLadderButton_h__

  #define BMLADDERBUTTON_DEFAULT_PIN_COOLDOWN_MS 50
  #define BMLADDERBUTTON_EMPTY_PIN -1
  #define BMLADDERBUTTON_MAX_TRIGGERS 12
  #define BM_CLASS_NAME_SIZE 36

  struct bmLadderButtonTrigger {
    unsigned long cooldown;
    uint32_t value;
    uint32_t variance;
    void (*callback)(uint32_t readValue, bmLadderButtonTrigger trigger);
    unsigned long lastTriggered;
  };

  class BMLadderButton {
    int analogPin;
    int cooldownMsPin;
    char instanceName[BM_CLASS_NAME_SIZE];
    int ladderButtonIndex;
    unsigned long lastPinCheck;
    bool pinActive;
    uint32_t pinValue;
    int triggerCount;
    bmLadderButtonTrigger triggers[BMLADDERBUTTON_MAX_TRIGGERS];

    void generate_instance_name(char *intoVar);
    bool init_members(int onPin, int withCooldownMs);
    bool init_members(char *lbName, int onPin, int withCooldownMs);
    bool pin_active(bool setTo);
    uint32_t read_pin_raw();
    void trigger_value_change(uint32_t newValue);

    public:
        BMLadderButton::BMLadderButton();
        BMLadderButton::BMLadderButton(char *lbName);
        BMLadderButton::BMLadderButton(int onPin);
        BMLadderButton::BMLadderButton(char *lbName, int onPin);
        bool activate();
        bool add_trigger(bmLadderButtonTrigger trigger);
        void clear_triggers();
        bool pin_active();
        void poll();
        bool set_analog_pin(int newAnalogPin);
        bool set_cooldown_ms(int newCoolMs);
        bool set_name(char *lbName);
        bool setup();
        bool setup(char *lbName);
        bool setup(int onPin);
        bool setup(char *lbName, int onPin);
  };

#endif

