/**
 * @file main.ino
 * @brief Fault-Tolerant, Non-Blocking Smart LED Embedded Controller
 * @project PM-IA Activity 2: GitHub-Based QA Documentation and Problem Solving
 * @author Amir Furquani (https://github.com/Amirfurquani)
 * @repository https://github.com/Amirfurquani/PM_IA_ACTIVIT_II_-ON-GITHUB.git
 * 
 * Hardware Target: Microchip ATmega328P / Arduino Uno R3
 * 
 * Commit Scope: Fixes Issue #1 (Replaced blocking delay() with non-blocking millis() scheduler)
 */

#include "config.h"
#include "led_controller.h"

// Hardware Interface Objects
LEDController ledEngine(PIN_STATUS_LED, PIN_EXTERNAL_LED, PIN_HEARTBEAT_LED);

// Performance & Telemetry Tracking
unsigned long loopCounter = 0;
unsigned long lastTelemetryTime = 0;
const unsigned long TELEMETRY_INTERVAL_MS = 2000;

const char* getStateName(SystemState state) {
    switch (state) {
        case STATE_NORMAL_BLINK:   return "NORMAL_BLINK (1.0 Hz)";
        case STATE_FAST_ALERT:     return "FAST_ALERT (5.0 Hz)";
        case STATE_SOS_BEACON:     return "SOS_BEACON (Morse Code)";
        case STATE_BREATHING_PWM:  return "BREATHING_PWM (Sine Fading)";
        default:                   return "UNKNOWN_STATE";
    }
}

void setup() {
    Serial.begin(SERIAL_BAUD_RATE);
    while (!Serial && millis() < 1000);

    Serial.println(F("[BOOT] Non-blocking millis() scheduler initialized."));
    ledEngine.begin();

    // Defect #2 still present: plain INPUT, no debouncing
    pinMode(PIN_MODE_BUTTON, INPUT);
}

void loop() {
    // Basic polling of button (Defect #2 still present)
    if (digitalRead(PIN_MODE_BUTTON) == LOW) {
        ledEngine.nextState();
    }

    // Non-blocking LED update (Fixes Issue #1: eliminates blocking delay())
    ledEngine.update();

    // Telemetry tracking
    loopCounter++;
    unsigned long currentMillis = millis();
    if (currentMillis - lastTelemetryTime >= TELEMETRY_INTERVAL_MS) {
        float loopsPerSec = (float)loopCounter / ((currentMillis - lastTelemetryTime) / 1000.0f);
        Serial.print(F("[TELEMETRY] Loop Frequency: "));
        Serial.print(loopsPerSec, 0);
        Serial.print(F(" Hz | Active State: "));
        Serial.println(getStateName(ledEngine.getState()));

        loopCounter = 0;
        lastTelemetryTime = currentMillis;
    }
}
