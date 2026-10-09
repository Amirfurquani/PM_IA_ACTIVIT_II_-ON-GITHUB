/**
 * @file config.h
 * @brief Hardware pin mapping, safety constants, and timing configurations.
 * @project Smart Multi-Mode LED Controller (Embedded QA Case Study)
 * @author Amir Furquani (https://github.com/Amirfurquani)
 * @repository https://github.com/Amirfurquani/PM_IA_ACTIVIT_II_-ON-GITHUB.git
 */

#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

// ============================================================================
// HARDWARE PIN DEFINITIONS
// ============================================================================
#define PIN_STATUS_LED          13      ///< Onboard status LED
#define PIN_EXTERNAL_LED        10      ///< External High-Brightness LED (PWM-capable)
#define PIN_MODE_BUTTON          2      ///< Mode toggle pushbutton (INT0 capable)
#define PIN_HEARTBEAT_LED        9      ///< Secondary diagnostic heartbeat LED (PWM)

// ============================================================================
// ELECTRICAL & SAFETY LIMITS (ATmega328P Specifications)
// ============================================================================
#define MAX_GPIO_CURRENT_MA     20      ///< Recommended continuous current limit per pin (safe margin below 40mA absolute max)
#define LED_SERIES_RESISTOR_OHM 220     ///< Measured current-limiting resistor value for 5V Vcc & 2.0V Vf LED

// ============================================================================
// TIMING CONSTANTS (Milliseconds)
// ============================================================================
#define BUTTON_DEBOUNCE_DELAY_MS 50     ///< Software debounce settling window
#define BUTTON_LONG_PRESS_MS    1500    ///< Duration threshold for long-press event
#define HEARTBEAT_PULSE_MS      1000    ///< Diagnostic alive-beacon cycle
#define SERIAL_BAUD_RATE        115200  ///< High-speed serial diagnostics

// ============================================================================
// OPERATIONAL MODES (FINITE STATE MACHINE)
// ============================================================================
enum SystemState {
    STATE_NORMAL_BLINK,     ///< 1.0 Hz square wave (500ms ON / 500ms OFF)
    STATE_FAST_ALERT,       ///< 5.0 Hz rapid warning blink (100ms ON / 100ms OFF)
    STATE_SOS_BEACON,       ///< International Morse Code S.O.S. optical beacon
    STATE_BREATHING_PWM,    ///< Smooth sinusoidal brightness modulation
    STATE_COUNT             ///< Total count of operational states
};

#endif // CONFIG_H
