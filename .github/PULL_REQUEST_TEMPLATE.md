## Pull Request Description
<!-- Provide a clear overview of the bugfix or feature introduced -->

## Associated QA Issue
- Closes #<!-- Issue number, e.g., Closes #1 -->

## Type of Change
- [ ] Bug fix (non-breaking change fixing a defect)
- [ ] Electrical safety fix (current limiting / protection)
- [ ] Timing optimization (cooperative non-blocking scheduler)
- [ ] Failsafe / Supervisory integration (Watchdog Timer)

## Verification & Testing Evidence
- **Telemetry Frequency Check:** Loop rate measured at >60 kHz
- **Oscilloscope / Logic Analyzer Check:** Switch debounce verified clean (>50ms holdoff)
- **Current Measurement:** Current strictly clamped below 20mA limit
- **Watchdog Timeout Check:** Simulated lockup recovers in 2.0s
