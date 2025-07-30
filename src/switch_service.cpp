#include "switch_service.h"
#include "Arduino.h"
#include "logger.h"

#define LOG_SWITCH_CTX "SWITCH"

LoggerFacade switch_logger(Logger::setup(LOG_SWITCH_CTX));

void
MOSFETSwitch::initialize() {
    pinMode(this->pin, OUTPUT);

    this->write_switch_state();
}

void
MOSFETSwitch::change_switch(SwitchState switch_state) {
    switch_logger.info("changing switch state to:");
    switch_logger.info(switch_state == SWITCH_OFF ? "OFF" : "ON");

    this->state = switch_state;

    this->write_switch_state();
}

void
MOSFETSwitch::toggle_switch() {
    switch_logger.info("toggling switch state from:");
    switch_logger.info(this->state == SWITCH_OFF ? "OFF" : "ON");
    switch_logger.info("into:");
    switch_logger.info(this->state != SWITCH_OFF ? "OFF" : "ON");

    if (this->state == SWITCH_OFF)
        this->state = SWITCH_ON;
    else
        this->state = SWITCH_OFF;

    this->write_switch_state();
}

SwitchState
MOSFETSwitch::get_switch_state() {
    return this->state;
}

int MOSFETSwitch::get_switch_state_as_logic_level() {
    if (this->state == SWITCH_OFF)
        return LOW;
    else
        return HIGH;
}

void MOSFETSwitch::write_switch_state() {
    digitalWrite(this->pin, this->get_switch_state_as_logic_level());
}
void
MOSFETSwitch::test_switch() {
    this->toggle_switch();
    delay(this->test_delay);
    this->toggle_switch();
}
