#ifndef __SWITCH_SERVICE_H__
#define __SWITCH_SERVICE_H__

typedef unsigned switch_dio_num_t;

enum SwitchState { SWITCH_OFF, SWITCH_ON };

class ISwitch {
public:
    virtual void toggle_switch() = 0;
    virtual void change_switch(SwitchState state) = 0;
    virtual void test_switch() = 0;
    virtual SwitchState get_switch_state() = 0;
};

class MOSFETSwitch : public ISwitch {
    SwitchState state = SWITCH_OFF;
    switch_dio_num_t pin;
    unsigned test_delay = 5000;

    int get_switch_state_as_logic_level();
    void write_switch_state();

public:
    explicit MOSFETSwitch(switch_dio_num_t switch_pin_num, SwitchState switch_init_state = SWITCH_OFF) :
        state(switch_init_state), pin(switch_pin_num) {}

    void initialize();

    void toggle_switch() override;
    void change_switch(SwitchState switch_state) override;
    void test_switch() override;
    SwitchState get_switch_state() override;
};

#endif // __SWITCH_SERVICE_H__
