/**
 * @file button_handler.cpp
 * @brief Implementation of non-blocking button state machine and debouncing.
 * @author Amir Furquani (https://github.com/Amirfurquani)
 * @repository https://github.com/Amirfurquani/PM_IA_ACTIVIT_II_-ON-GITHUB.git
 */

#include "button_handler.h"

ButtonHandler::ButtonHandler(uint8_t pin, uint16_t debounceMs)
    : _pin(pin),
      _debounceMs(debounceMs),
      _lastStableState(HIGH),
      _lastReading(HIGH),
      _lastDebounceTime(0),
      _pressStartTime(0),
      _isPressed(false),
      _longPressEmitted(false) {}

void ButtonHandler::begin() {
    // Enable internal pull-up resistor to prevent floating input (addresses QA Issue #2)
    pinMode(_pin, INPUT_PULLUP);
    _lastStableState = digitalRead(_pin);
    _lastReading = _lastStableState;
}

ButtonEvent ButtonHandler::update() {
    int reading = digitalRead(_pin);
    ButtonEvent event = EVENT_NONE;
    unsigned long currentMillis = millis();

    // Check if mechanical bounce or pin transition occurred
    if (reading != _lastReading) {
        _lastDebounceTime = currentMillis;
    }

    // Verify if reading has persisted longer than debounce threshold
    if ((currentMillis - _lastDebounceTime) > _debounceMs) {
        if (reading != _lastStableState) {
            _lastStableState = reading;

            // Active-LOW logic with INPUT_PULLUP
            if (_lastStableState == LOW) {
                _isPressed = true;
                _pressStartTime = currentMillis;
                _longPressEmitted = false;
            } else {
                // Button released
                if (_isPressed && !_longPressEmitted) {
                    event = EVENT_SHORT_PRESS;
                }
                _isPressed = false;
            }
        }
    }

    // Check for continuous hold long press
    if (_isPressed && !_longPressEmitted) {
        if ((currentMillis - _pressStartTime) >= BUTTON_LONG_PRESS_MS) {
            event = EVENT_LONG_PRESS;
            _longPressEmitted = true;
        }
    }

    _lastReading = reading;
    return event;
}
