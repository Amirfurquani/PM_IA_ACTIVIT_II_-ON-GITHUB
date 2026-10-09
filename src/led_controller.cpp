/**
 * @file led_controller.cpp
 * @brief Implementation of non-blocking LED pattern generator and state machine.
 * @author Amir Furquani (https://github.com/Amirfurquani)
 * @repository https://github.com/Amirfurquani/PM_IA_ACTIVIT_II_-ON-GITHUB.git
 */

#include "led_controller.h"

// SOS Morse code element timing: dot = 150ms, dash = 450ms, intra-element gap = 150ms
// Sequence: S ( . . . ) -> gap -> O ( - - - ) -> gap -> S ( . . . ) -> word pause
struct SOSElement {
    bool state;         // HIGH for ON, LOW for OFF
    uint16_t duration;  // milliseconds
};

static const SOSElement SOS_PATTERN[] = {
    // S: . . .
    {HIGH, 150}, {LOW, 150},
    {HIGH, 150}, {LOW, 150},
    {HIGH, 150}, {LOW, 450}, // Letter gap
    // O: - - -
    {HIGH, 450}, {LOW, 150},
    {HIGH, 450}, {LOW, 150},
    {HIGH, 450}, {LOW, 450}, // Letter gap
    // S: . . .
    {HIGH, 150}, {LOW, 150},
    {HIGH, 150}, {LOW, 150},
    {HIGH, 150}, {LOW, 1200} // End of message word pause
};
static const uint8_t SOS_STEP_COUNT = sizeof(SOS_PATTERN) / sizeof(SOS_PATTERN[0]);

LEDController::LEDController(uint8_t statusPin, uint8_t externalPin, uint8_t heartbeatPin)
    : _statusPin(statusPin),
      _externalPin(externalPin),
      _heartbeatPin(heartbeatPin),
      _currentState(STATE_NORMAL_BLINK),
      _lastToggleTime(0),
      _lastHeartbeatTime(0),
      _statusLedState(LOW),
      _heartbeatLedState(LOW),
      _sosStep(0),
      _sosStepStartTime(0),
      _brightness(0),
      _fadeAmount(5),
      _lastFadeTime(0) {}

void LEDController::begin() {
    pinMode(_statusPin, OUTPUT);
    pinMode(_externalPin, OUTPUT);
    pinMode(_heartbeatPin, OUTPUT);

    digitalWrite(_statusPin, LOW);
    digitalWrite(_externalPin, LOW);
    digitalWrite(_heartbeatPin, LOW);
}

void LEDController::setState(SystemState newState) {
    if (newState >= STATE_COUNT) {
        newState = STATE_NORMAL_BLINK;
    }
    _currentState = newState;

    // Reset state-dependent timers and states
    _lastToggleTime = millis();
    _statusLedState = LOW;
    _sosStep = 0;
    _sosStepStartTime = millis();
    _brightness = 0;
    _fadeAmount = 5;

    // Ensure external PWM LED is turned off upon state change
    analogWrite(_externalPin, 0);
    digitalWrite(_statusPin, LOW);
}

SystemState LEDController::getState() const {
    return _currentState;
}

void LEDController::nextState() {
    SystemState next = static_cast<SystemState>((_currentState + 1) % STATE_COUNT);
    setState(next);
}

void LEDController::update() {
    unsigned long currentMillis = millis();

    // Constant background heartbeat execution
    updateHeartbeat(currentMillis);

    // Operational mode dispatch
    switch (_currentState) {
        case STATE_NORMAL_BLINK:
            updateNormalBlink(currentMillis);
            break;
        case STATE_FAST_ALERT:
            updateFastAlert(currentMillis);
            break;
        case STATE_SOS_BEACON:
            updateSOS(currentMillis);
            break;
        case STATE_BREATHING_PWM:
            updateBreathing(currentMillis);
            break;
        default:
            setState(STATE_NORMAL_BLINK);
            break;
    }
}

void LEDController::updateNormalBlink(unsigned long currentMillis) {
    const unsigned long BLINK_INTERVAL = 500; // 500ms ON, 500ms OFF (1.0 Hz)
    if (currentMillis - _lastToggleTime >= BLINK_INTERVAL) {
        _lastToggleTime = currentMillis;
        _statusLedState = !_statusLedState;
        digitalWrite(_statusPin, _statusLedState);
        digitalWrite(_externalPin, _statusLedState);
    }
}

void LEDController::updateFastAlert(unsigned long currentMillis) {
    const unsigned long FAST_INTERVAL = 100; // 100ms ON, 100ms OFF (5.0 Hz)
    if (currentMillis - _lastToggleTime >= FAST_INTERVAL) {
        _lastToggleTime = currentMillis;
        _statusLedState = !_statusLedState;
        digitalWrite(_statusPin, _statusLedState);
        digitalWrite(_externalPin, _statusLedState);
    }
}

void LEDController::updateSOS(unsigned long currentMillis) {
    if (currentMillis - _sosStepStartTime >= SOS_PATTERN[_sosStep].duration) {
        _sosStep = (_sosStep + 1) % SOS_STEP_COUNT;
        _sosStepStartTime = currentMillis;
        digitalWrite(_statusPin, SOS_PATTERN[_sosStep].state);
        digitalWrite(_externalPin, SOS_PATTERN[_sosStep].state);
    }
}

void LEDController::updateBreathing(unsigned long currentMillis) {
    const unsigned long FADE_INTERVAL = 20; // Update step every 20ms
    if (currentMillis - _lastFadeTime >= FADE_INTERVAL) {
        _lastFadeTime = currentMillis;
        _brightness += _fadeAmount;

        if (_brightness <= 0 || _brightness >= 255) {
            _fadeAmount = -_fadeAmount;
        }

        _brightness = constrain(_brightness, 0, 255);
        analogWrite(_externalPin, _brightness);
        digitalWrite(_statusPin, (_brightness > 127) ? HIGH : LOW);
    }
}

void LEDController::updateHeartbeat(unsigned long currentMillis) {
    const unsigned long PULSE_DURATION = 50;  // 50ms quick pulse
    if (!_heartbeatLedState) {
        if (currentMillis - _lastHeartbeatTime >= HEARTBEAT_PULSE_MS) {
            _lastHeartbeatTime = currentMillis;
            _heartbeatLedState = true;
            digitalWrite(_heartbeatPin, HIGH);
        }
    } else {
        if (currentMillis - _lastHeartbeatTime >= PULSE_DURATION) {
            _heartbeatLedState = false;
            digitalWrite(_heartbeatPin, LOW);
        }
    }
}
