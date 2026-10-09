# AI-Assisted Embedded Firmware Engineering & QA Collaboration Log

**Author:** Amir Furquani (PRN: 202301070165)  
**Course:** Project Management (2307476T)  
**Academic Year:** 2026 – 2027 | **Semester:** VII | **Division:** B  
**Repository:** [https://github.com/Amirfurquani/PM_IA_ACTIVIT_II_-ON-GITHUB.git](https://github.com/Amirfurquani/PM_IA_ACTIVIT_II_-ON-GITHUB.git)  

---

## 1. Overview of AI-Based Engineering Workflow

In modern industrial embedded systems engineering, Artificial Intelligence (AI) pair programming and automated LLM-based static analysis significantly accelerate defect discovery, root cause analysis (RCA), and defensive code refactoring. This activity was executed using an **AI-driven Quality Assurance Lifecycle**:

```
+---------------------+     +----------------------+     +---------------------+
|  Baseline Prototype | --> | AI Code Audit Agent  | --> | Defect Extraction & |
|  Embedded C++ Code  |     | (Antigravity AI / LLM) |    | Risk Classification |
+---------------------+     +----------------------+     +---------------------+
                                                                    |
                                                                    v
+---------------------+     +----------------------+     +---------------------+
| Verified Production | <-- | Automated Git Branch | <-- | AI-Generated 5-Why  |
|  Firmware on Main   |     | Fixes & Pull Requests|     | & Fishbone Synthesis|
+---------------------+     +----------------------+     +---------------------+
```

---

## 2. AI Prompting & Diagnostic Sessions

### Prompt Session 1: Baseline Static Code & Electrical Safety Audit
- **Engineer Query:** *"Analyze this initial Arduino LED blinking code and circuit schematic. Identify all latent software and hardware defects that would violate industrial embedded safety standards."*
- **AI Agent Response:**
  1. Identified synchronous blocking `delay(500)` calls halting CPU execution and freezing UI button response.
  2. Detected high-impedance floating input on Pin 2 with missing pull-up biasing and absence of debounce hysteresis.
  3. Calculated unsafe diode forward current on GPIO Pin 10 ($I \approx 120\text{ mA} \gg 40\text{ mA}$ absolute limit).
  4. Flagged missing supervisory watchdog resets (`<avr/wdt.h>`), exposing the system to permanent EMI lockups.

### Prompt Session 2: AI-Generated 5-Why & Fishbone RCA
- **Engineer Query:** *"Generate a formal 5-Why Root Cause Analysis and Ishikawa Fishbone categorization for each of the 4 discovered defects according to industrial QA standards."*
- **AI Agent Response:** Formulated structured 5-Why sequences leading from superficial symptoms to fundamental design-rule oversights, mapping them into four branches (Software & Timing, Electrical Safety, Signal Integrity, Reliability & EMI).

### Prompt Session 3: AI-Assisted Architecture Refactoring
- **Engineer Query:** *"Propose a modular, non-blocking C++ architecture implementing a 4-state Finite State Machine (FSM), 50ms software debouncer, and AVR Watchdog integration."*
- **AI Agent Response:** Designed `LEDController` and `ButtonHandler` modular classes using `millis()` delta math and object-oriented encapsulation, ensuring loop frequency exceeding 60 kHz.

---

## 3. Measurable AI Engineering Impact

| Engineering Dimension | Traditional Manual Approach | AI-Assisted QA Workflow | Improvement Metric |
|---|---|---|---|
| Defect Discovery Time | 4–6 hours (hardware trial & error) | < 10 minutes (static analysis) | ~95% reduction |
| Root Cause Formulation | Informal debugging guesses | Structured 5-Why & Fishbone | 100% formal compliance |
| Code Architecture Quality | Spaghetti monolithic code | Modular FSM & C++ classes | High maintainability |
| Safety & Protection | Often neglected (missing WDT/resistors) | Enforced defensive design | Zero hardware hazard |
