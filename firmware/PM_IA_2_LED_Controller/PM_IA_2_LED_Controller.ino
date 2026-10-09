/**
 * @file main.ino
 * @brief Fault-Tolerant, Non-Blocking Smart LED Embedded Controller
 * @project PM-IA Activity 2: GitHub-Based QA Documentation and Problem Solving
 * @course Project Management (2307476T) - Department of E&TC Engineering, MIT AOE
 * @author Amir Furquani (https://github.com/Amirfurquani)
 * @repository https://github.com/Amirfurquani/PM_IA_ACTIVIT_II_-ON-GITHUB.git
 * 
 * Hardware Target: Microchip ATmega328P / Arduino Uno R3
 * Clock Speed: 16 MHz
 * 
 * Key Architectural Highlights (Addressing QA Issues #1 to #4):
 *  1. Non-blocking cooperative multitasking via millis() (Eliminates blocking delay())
 *  2. Hardware-protected and software-debounced input handling (Active-Low INPUT_PULLUP)
 *  3. Current-limited output protection on GPIO pins (220 Ohm series resistance)
 *  4. Hardware Watchdog Timer (AVR WDT) enabled with 2.0s timeout to survive EMI lockups
 */

#include <avr/wdt.h>
#include "config.h"
#include "button_handler.h"
#include "led_controller.h"

// Hardware Interface Objects
ButtonHandler modeButton(PIN_MODE_BUTTON, BUTTON_DEBOUNCE_DELAY_MS);
LEDController ledEngine(PIN_STATUS_LED, PIN_EXTERNAL_LED, PIN_HEARTBEAT_LED);

// Performance & Telemetry Tracking
unsigned long loopCounter = 0;
unsigned long lastTelemetryTime = 0;
const unsigned long TELEMETRY_INTERVAL_MS = 2000;

// Helper to convert enum state to human-readable string
const char* getStateName(SystemState state) {
    switch (state) {
        case STATE_NORMAL_BLINK:   return "NORMAL_BLINK (1.0 Hz)";
        case STATE_FAST_ALERT:     return "FAST_ALERT (5.0 Hz)";
        case STATE_SOS_BEACON:     return "SOS_BEACON (Morse Code)";
        case STATE_BREATHING_PWM:  return "BREATHING_PWM (Sine Fading)";
        default:                   return "UNKNOWN_STATE";
    }
}

void printSystemBanner() {
    Serial.println(F("\n======================================================="));
    Serial.println(F("  SMART FAULT-TOLERANT EMBEDDED LED CONTROLLER         "));
    Serial.println(F("  Project Management Activity 2 - QA & Git Workflow    "));
    Serial.println(F("  Author: Amir Furquani | PRN: 202301070165            "));
    Serial.println(F("  Repo: Amirfurquani/PM_IA_ACTIVIT_II_-ON-GITHUB       "));
    Serial.println(F("======================================================="));
    Serial.println(F("[INFO] Architecture: Non-Blocking FSM Engine"));
    Serial.println(F("[INFO] Watchdog Timer: Enabled (Timeout: 2000 ms)"));
    Serial.println(F("[INFO] Debounce Filter: 50 ms Active-Low"));
    Serial.println(F("[INFO] GPIO Pinout:"));
    Serial.println(F("       - Status LED:     Pin 13 (Built-in)"));
    Serial.println(F("       - External LED:   Pin 10 (PWM via 220R)"));
    Serial.println(F("       - Mode Button:    Pin 2  (Internal Pull-up)"));
    Serial.println(F("       - Heartbeat LED:  Pin 9  (PWM Pulse)"));
    Serial.println(F("-------------------------------------------------------"));
    Serial.println(F("Interactive Commands:"));
    Serial.println(F("  'm' or 'M' -> Switch to Next State"));
    Serial.println(F("  'r' or 'R' -> Reset to Default Normal Blink"));
    Serial.println(F("  's' or 'S' -> Print System Telemetry"));
    Serial.println(F("  'w' or 'W' -> Trigger Intentional Lockup (Test Watchdog)"));
    Serial.println(F("=======================================================\n"));
}

void setup() {
    // Initialize serial communication
    Serial.begin(SERIAL_BAUD_RATE);
    while (!Serial && millis() < 1000); // Allow USB-UART bridge to settle

    printSystemBanner();

    // Initialize hardware peripherals
    modeButton.begin();
    ledEngine.begin();

    // Enable AVR Hardware Watchdog Timer with a 2-second timeout window
    // (Resolves QA Issue #4: System lockup in unhandled electrical transient)
    wdt_enable(WDTO_2S);
    Serial.println(F("[BOOT] AVR Watchdog Timer armed (2000ms). System stable."));
    Serial.print(F("[BOOT] Initial State: "));
    Serial.println(getStateName(ledEngine.getState()));
}

void processSerialCommands() {
    if (Serial.available() > 0) {
        char cmd = Serial.read();
        switch (cmd) {
            case 'm':
            case 'M':
                ledEngine.nextState();
                Serial.print(F("[CMD] Manual Mode Switch -> "));
                Serial.println(getStateName(ledEngine.getState()));
                break;
            case 'r':
            case 'R':
                ledEngine.setState(STATE_NORMAL_BLINK);
                Serial.println(F("[CMD] Reset to STATE_NORMAL_BLINK"));
                break;
            case 's':
            case 'S':
                Serial.print(F("[STATUS] State: "));
                Serial.print(getStateName(ledEngine.getState()));
                Serial.print(F(" | Uptime: "));
                Serial.print(millis() / 1000);
                Serial.println(F("s"));
                break;
            case 'w':
            case 'W':
                Serial.println(F("[TEST] Simulating infinite lockup... Watchdog should trigger reset in 2s!"));
                while (true) {
                    // Infinite loop without wdt_reset() to verify Watchdog recovery
                }
                break;
            default:
                break;
        }
    }
}

void loop() {
    // 1. Kick the Watchdog Timer to notify system health
    wdt_reset();

    // 2. Poll & update input buttons (non-blocking)
    ButtonEvent btnEvent = modeButton.update();
    if (btnEvent == EVENT_SHORT_PRESS) {
        ledEngine.nextState();
        Serial.print(F("[BUTTON] Short Press Detected! Transitioning to: "));
        Serial.println(getStateName(ledEngine.getState()));
    } else if (btnEvent == EVENT_LONG_PRESS) {
        ledEngine.setState(STATE_NORMAL_BLINK);
        Serial.println(F("[BUTTON] Long Press (>1.5s) Detected! Resetting to: STATE_NORMAL_BLINK"));
    }

    // 3. Process remote UART control commands
    processSerialCommands();

    // 4. Update LED State Machine & Heartbeat
    ledEngine.update();

    // 5. Compute real-time loop metrics & telemetry
    loopCounter++;
    unsigned long currentMillis = millis();
    if (currentMillis - lastTelemetryTime >= TELEMETRY_INTERVAL_MS) {
        float loopsPerSec = (float)loopCounter / ((currentMillis - lastTelemetryTime) / 1000.0f);
        Serial.print(F("[TELEMETRY] Loop Frequency: "));
        Serial.print(loopsPerSec, 0);
        Serial.print(F(" Hz | Active State: "));
        Serial.print(getStateName(ledEngine.getState()));
        Serial.print(F(" | Millis: "));
        Serial.println(currentMillis);

        loopCounter = 0;
        lastTelemetryTime = currentMillis;
    }
}
