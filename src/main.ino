/**
 * @file main.ino
 * @brief Prototype Baseline LED Controller (Contains Baseline QA Defects #1 to #4)
 * @author Amir Furquani (PRN: 202301070165)
 * @repository https://github.com/Amirfurquani/PM_IA_ACTIVIT_II_-ON-GITHUB.git
 * 
 * Target: ATmega328P / Arduino Uno R3
 * 
 * WARNING - KNOWN BASELINE DEFECTS PRESENT IN THIS COMMIT:
 *  - Issue #1: Blocking delay(500) loops freezing the CPU and missing button presses.
 *  - Issue #2: Floating input pin (plain INPUT mode, no pull-up) and no debouncing.
 *  - Issue #3: Direct LED drive on GPIO Pin 10 with 0 Ohm resistance (drawing >100mA).
 *  - Issue #4: Absence of hardware Watchdog Timer (WDT) resulting in unrecoverable lockups.
 */

#define PIN_STATUS_LED    13
#define PIN_EXTERNAL_LED  10
#define PIN_HEARTBEAT_LED 9
#define PIN_MODE_BUTTON   2

int currentMode = 0;

void setup() {
    Serial.begin(115200);
    Serial.println(F("[BOOT] Baseline Prototype Initialized (Defects Present)"));

    // DEFECT #2: Floating input without pull-up resistor
    pinMode(PIN_MODE_BUTTON, INPUT);

    // DEFECT #3: Output configured for direct drive (No series resistor in hardware)
    pinMode(PIN_STATUS_LED, OUTPUT);
    pinMode(PIN_EXTERNAL_LED, OUTPUT);
    pinMode(PIN_HEARTBEAT_LED, OUTPUT);

    // DEFECT #4: Watchdog timer NOT initialized
}

void loop() {
    // DEFECT #2: Naive digitalRead with switch contact bounce and floating noise
    if (digitalRead(PIN_MODE_BUTTON) == LOW) {
        currentMode = (currentMode + 1) % 4;
        Serial.print(F("Mode switched to: "));
        Serial.println(currentMode);
    }

    // DEFECT #1: Blocking delay() calls freeze the CPU during blinking cycles
    if (currentMode == 0) {
        // Mode 0: Normal blink
        digitalWrite(PIN_EXTERNAL_LED, HIGH);
        digitalWrite(PIN_STATUS_LED, HIGH);
        delay(500); // CPU BLOCKED
        digitalWrite(PIN_EXTERNAL_LED, LOW);
        digitalWrite(PIN_STATUS_LED, LOW);
        delay(500); // CPU BLOCKED
    } else if (currentMode == 1) {
        // Mode 1: Fast alert
        digitalWrite(PIN_EXTERNAL_LED, HIGH);
        delay(100); // CPU BLOCKED
        digitalWrite(PIN_EXTERNAL_LED, LOW);
        delay(100); // CPU BLOCKED
    } else if (currentMode == 2) {
        // Mode 2: SOS
        for (int i = 0; i < 3; i++) {
            digitalWrite(PIN_EXTERNAL_LED, HIGH); delay(150);
            digitalWrite(PIN_EXTERNAL_LED, LOW); delay(150);
        }
        for (int i = 0; i < 3; i++) {
            digitalWrite(PIN_EXTERNAL_LED, HIGH); delay(450);
            digitalWrite(PIN_EXTERNAL_LED, LOW); delay(150);
        }
        for (int i = 0; i < 3; i++) {
            digitalWrite(PIN_EXTERNAL_LED, HIGH); delay(150);
            digitalWrite(PIN_EXTERNAL_LED, LOW); delay(150);
        }
        delay(1000); // CPU BLOCKED for 1 second!
    } else {
        // Mode 3: Simple static ON
        digitalWrite(PIN_EXTERNAL_LED, HIGH);
        delay(500);
    }
}
