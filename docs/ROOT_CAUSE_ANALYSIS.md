# Root Cause Analysis (RCA): Embedded QA Defects

**Author:** Amir Furquani (PRN: 202301070165)  
**Course:** Project Management (2307476T)  
**Academic Year:** 2026 – 2027 | **Semester:** VII | **Division:** B  
**Repository:** [https://github.com/Amirfurquani/PM_IA_ACTIVIT_II_-ON-GITHUB.git](https://github.com/Amirfurquani/PM_IA_ACTIVIT_II_-ON-GITHUB.git)  

---

## 1. Executive Summary

During the development and testing of the Smart Embedded LED Controller on the Microchip ATmega328P (Arduino Uno R3) architecture, four critical defects were isolated. Using structured Root Cause Analysis (RCA)—specifically **5-Why Analysis** and **Fishbone (Ishikawa) Modeling**—the fundamental hardware and firmware root causes were determined and permanently countermeasured.

---

## 2. 5-Why Root Cause Analyses

### Issue #1: Blocking `delay()` Starves CPU & Introduces 1000ms UI Latency
- **Why 1:** Why is the microcontroller completely unresponsive to pushbutton inputs during the blinking pattern?  
  *Because the CPU execution thread is halted inside `delay(500)` calls.*
- **Why 2:** Why is `delay()` being called inside the control loop?  
  *Because the pattern timing relies on naive busy-wait synchronous loops.*
- **Why 3:** Why does busy-waiting prevent button polling?  
  *Because the ATmega328P is a single-core microcontroller with no hardware RTOS thread preemption.*
- **Why 4:** Why was synchronous timing initially implemented?  
  *Because standard tutorial starter code models time through linear blocking delays rather than state machines.*
- **Why 5:** Why was non-blocking timing omitted in initial requirements?  
  *Because temporal constraints and multi-tasking cooperative requirements were not formally budgeted during early prototyping.*
- **Permanent Countermeasure:** Implemented non-blocking timestamp delta arithmetic (`millis() - lastTime >= interval`) driven by an asynchronous Finite State Machine (FSM). Loop frequency increased from 1 Hz to >65 kHz.

---

### Issue #2: Floating Input Pin & Switch Contact Bounce Induces Spurious Triggers
- **Why 1:** Why does the operational mode unexpectedly skip states or cycle wildly on a single physical push?  
  *Because digital input Pin 2 registers dozens of rapid state transitions within a few milliseconds.*
- **Why 2:** Why are there multiple state transitions on a single actuation?  
  *Because physical metal contacts mechanically bounce for 5 to 20 ms upon impact, and the pin floats when open.*
- **Why 3:** Why is the pin floating when the switch is open?  
  *Because the GPIO was configured as plain `INPUT` without a pull-up or pull-down biasing network.*
- **Why 4:** Why is the pin voltage unstable without a biasing network?  
  *High-impedance CMOS inputs pick up ambient electromagnetic interference and stray electrostatic charges.*
- **Why 5:** Why was hardware pull-up or software debouncing missing?  
  *Because hardware impedance stabilization and temporal contact hysteresis were not factored into the initial schematic.*
- **Permanent Countermeasure:** Configured the pin as `INPUT_PULLUP` to engage the internal 20k–50k $\Omega$ pull-up resistor, and implemented a 50ms edge-triggered software debouncer (`ButtonHandler`) with short-press and long-press discrimination.

---

### Issue #3: Direct LED Drive Exceeds ATmega328P Absolute Max GPIO Rating (40mA)
- **Why 1:** Why is the microcontroller GPIO pin experiencing thermal stress and potential long-term gate breakdown?  
  *Because the continuous current drawn through Pin 10 exceeds 100 mA during high-brightness states.*
- **Why 2:** Why is the current exceeding 100 mA?  
  *Because the LED forward voltage ($V_f \approx 2.0\text{V}$) is connected directly across the 5.0V GPIO rail without series impedance.*
- **Why 3:** Why does this exceed the semiconductor limits?  
  *Because the ATmega328P absolute maximum rating is 40 mA per I/O pin (and 20 mA continuous recommended).*
- **Why 4:** Why was no series resistor placed on the breadboard / virtual circuit?  
  *Because rapid prototyping assumed the MCU output buffer impedance would naturally limit current.*
- **Why 5:** Why were electrical component ratings not verified beforehand?  
  *Because electrical safety design rules and component current budgeting were not enforced prior to firmware deployment.*
- **Permanent Countermeasure:** Inserted a 220 $\Omega$ series current-limiting resistor into the circuit schematic:
  $$I = \frac{V_{CC} - V_F}{R} = \frac{5.0\text{V} - 2.0\text{V}}{220\,\Omega} = \frac{3.0\text{V}}{220\,\Omega} \approx 13.63\text{ mA}$$
  This clamps the current safely below the 20 mA continuous limit.

---

### Issue #4: Microcontroller Lockup Due to High-Voltage EMI Without Watchdog
- **Why 1:** Why did the system permanently freeze and halt all LED blinking during inductive switching / transient tests?  
  *Because the microcontroller program counter jumped to an unmapped memory vector or trapped into an infinite stall.*
- **Why 2:** Why did the program counter get corrupted?  
  *Because transient electrical noise (EMI) induced voltage glitches on the internal bus lines.*
- **Why 3:** Why did the microcontroller fail to recover autonomously after the noise event ended?  
  *Because there was no supervisory watchdog circuit monitoring system execution liveness.*
- **Why 4:** Why was the Watchdog Timer not active?  
  *Because the AVR hardware Watchdog Timer is disabled by default on power-up and requires explicit firmware configuration.*
- **Why 5:** Why was supervisory watchdog reset overlooked during initial software design?  
  *Because safety-critical embedded reliability protocols were not integrated into the base application architecture.*
- **Permanent Countermeasure:** Armed the internal AVR Hardware Watchdog Timer (`wdt_enable(WDTO_2S)`) with a 2000ms window, serviced deterministically via `wdt_reset()` in the non-blocking loop, guaranteeing automatic hardware reset upon deadlock.

---

## 3. Fishbone (Ishikawa) Diagram Categorization

The embedded defects are categorized across four engineering branches:
1. **Software & Timing:** Blocking synchronous delays, lack of cooperative task slicing, CPU starvation.
2. **Electrical Safety:** Direct LED drive without impedance budgeting, GPIO overcurrent (>100mA), thermal stress.
3. **Signal Integrity:** Floating CMOS inputs, mechanical contact chatter (5–20ms), absent pull-up biasing.
4. **Reliability & EMI:** Program counter glitching, absence of autonomous supervisory Watchdog recovery.
