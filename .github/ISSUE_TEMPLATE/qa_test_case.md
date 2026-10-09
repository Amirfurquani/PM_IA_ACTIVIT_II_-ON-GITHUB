---
name: QA Test Case
about: Execute and document verification test cases for embedded quality assurance
title: "[QA-TEST] "
labels: qa-test
assignees: Amirfurquani
---

## Test Identifier
- **Test ID:** TC-
- **Requirement ID:** REQ-

## Test Objectives
<!-- Specific electrical, timing, or functional criteria to verify -->

## Pre-Conditions
- Circuit wiring configured according to `diagram.json`
- Serial console attached at 115,200 baud

## Test Procedure
1. Send serial command or actuate hardware switch
2. Measure response time / oscilloscope trace / current meter
3. Record output telemetry

## Acceptance Criteria
- [ ] Response latency within specified timing bounds (< 1ms)
- [ ] Current consumption within safe limits (< 20mA)
- [ ] Hardware Watchdog resets within window upon failure
