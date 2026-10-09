# Quality Assurance (QA) Process & Defect Management Plan

**Project:** Fault-Tolerant Smart Embedded LED Controller  
**Course:** Project Management (2307476T)  
**Academic Year:** 2026 – 2027 | **Semester:** VII | **Division:** B  
**Author:** Amir Furquani (PRN: 202301070165)  
**Repository:** [https://github.com/Amirfurquani/PM_IA_ACTIVIT_II_-ON-GITHUB.git](https://github.com/Amirfurquani/PM_IA_ACTIVIT_II_-ON-GITHUB.git)  

---

## 1. QA Lifecycle Overview

The embedded firmware quality assurance lifecycle follows a closed-loop Agile methodology combining AI-assisted defect identification, static code analysis, virtual hardware-in-the-loop simulation, and Git-based resolution tracking.

```
+------------------+      +-------------------+      +------------------+
| AI Code Review & | ---> |  Issue Logging in | ---> | Root Cause (RCA) |
|  Defect Discovery|      |   GitHub Issues   |      |  5-Why / Fishbone|
+------------------+      +-------------------+      +------------------+
                                                              |
                                                              v
+------------------+      +-------------------+      +------------------+
| Closed / Verified| <--- | Automated Telemetry| <--- |  Branch & Pull   |
|   on Main Branch |      |  & Lab Validation |      | Request Merge    |
+------------------+      +-------------------+      +------------------+
```

---

## 2. Defect Severity & Priority Classification Matrix

| Severity Level | Definition | Impact on Embedded System | SLA / Action Window |
|---|---|---|---|
| **Critical** | Fatal malfunction; system lockup or physical hardware damage risk. | Floating pin triggers random interrupts; excessive GPIO current damaging microcontroller. | Immediate Fix (P1 Blocker) |
| **High** | Core functionality severely degraded; massive response latency. | Synchronous `delay()` loops starving the CPU thread, freezing user input for >1000ms. | Fix in Current Sprint (P1/P2) |
| **Major** | System instability under transient or boundary conditions. | Unsupervised MCU lockup during EMI or power transient without Watchdog auto-reboot. | Fix before Release (P2) |
| **Minor** | Cosmetic or non-blocking diagnostic defect. | Telemetry formatting discrepancies or minor serial log jitter. | Backlog (P3) |

---

## 3. Test Cases & Verification Plan

### Test Case TC-01: CPU Starvation & Loop Frequency Verification
- **Target:** Issue #1
- **Procedure:** Execute main loop while monitoring serial telemetry output frequency over a 2000ms measurement window.
- **Pass Criteria:** Loop frequency must exceed 50,000 Hz; input latency must measure under 1 ms.

### Test Case TC-02: Mechanical Switch Debounce & Pull-Up Integrity
- **Target:** Issue #2
- **Procedure:** Connect digital logic analyzer to Pin 2; rapidly actuate mechanical pushbutton 20 consecutive times.
- **Pass Criteria:** No spurious state increments; exactly 1 transition per physical actuation.

### Test Case TC-03: Current Budgeting & GPIO Electrical Safety
- **Target:** Issue #3
- **Procedure:** Measure continuous DC forward current across Pin 10 and Pin 9 with digital multimeter during active PWM driving.
- **Pass Criteria:** Current must not exceed 20 mA (Calculated: $13.63\text{ mA} \pm 5\%$).

### Test Case TC-04: Watchdog Supervisory Timeout & Recovery
- **Target:** Issue #4
- **Procedure:** Transmit serial command `'w'` to force infinite while-loop lockup without watchdog kicks.
- **Pass Criteria:** AVR hardware Watchdog Timer triggers hard system reset within $2000\text{ ms} \pm 200\text{ ms}$.
