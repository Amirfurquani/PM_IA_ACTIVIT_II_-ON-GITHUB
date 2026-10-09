/**
 * @file led_controller.h
 * @brief Non-blocking FSM-based LED pattern sequencer.
 * @project Smart Multi-Mode LED Controller (Embedded QA Case Study)
 * @author Amir Furquani (https://github.com/Amirfurquani)
 * @repository https://github.com/Amirfurquani/PM_IA_ACTIVIT_II_-ON-GITHUB.git
 */

#ifndef LED_CONTROLLER_H
#define LED_CONTROLLER_H

#include "config.h"

class LEDController {
public:
    LEDController(uint8_t statusPin, uint8_t externalPin, uint8_t heartbeatPin);
    void begin();
    void setState(SystemState newState);
    SystemState getState() const;
    void nextState();
    void update();

private:
    uint8_t _statusPin;
    uint8_t _externalPin;
    uint8_t _heartbeatPin;
    SystemState _currentState;

    // Timing tracking variables
    unsigned long _lastToggleTime;
    unsigned long _lastHeartbeatTime;
    bool _statusLedState;
    bool _heartbeatLedState;

    // S.O.S. pattern sequencer variables
    uint8_t _sosStep;
    unsigned long _sosStepStartTime;

    // Breathing PWM variables
    int _brightness;
    int _fadeAmount;
    unsigned long _lastFadeTime;

    void updateNormalBlink(unsigned long currentMillis);
    void updateFastAlert(unsigned long currentMillis);
    void updateSOS(unsigned long currentMillis);
    void updateBreathing(unsigned long currentMillis);
    void updateHeartbeat(unsigned long currentMillis);
};

#endif // LED_CONTROLLER_H
