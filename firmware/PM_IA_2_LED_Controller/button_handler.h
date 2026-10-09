/**
 * @file button_handler.h
 * @brief Non-blocking pushbutton input handler with edge-triggered debouncing.
 * @project Smart Multi-Mode LED Controller (Embedded QA Case Study)
 * @author Amir Furquani (https://github.com/Amirfurquani)
 * @repository https://github.com/Amirfurquani/PM_IA_ACTIVIT_II_-ON-GITHUB.git
 */

#ifndef BUTTON_HANDLER_H
#define BUTTON_HANDLER_H

#include "config.h"

enum ButtonEvent {
    EVENT_NONE,
    EVENT_SHORT_PRESS,
    EVENT_LONG_PRESS
};

class ButtonHandler {
public:
    ButtonHandler(uint8_t pin, uint16_t debounceMs = BUTTON_DEBOUNCE_DELAY_MS);
    void begin();
    ButtonEvent update();

private:
    uint8_t _pin;
    uint16_t _debounceMs;
    int _lastStableState;
    int _lastReading;
    unsigned long _lastDebounceTime;
    unsigned long _pressStartTime;
    bool _isPressed;
    bool _longPressEmitted;
};

#endif // BUTTON_HANDLER_H
