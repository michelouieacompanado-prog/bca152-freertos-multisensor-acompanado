# Wokwi Functional Verification & Fault Experiments Record

This document records the empirical functional verification results and deliberate FreeRTOS fault experiment findings for the **Real-Time Multisensor Room Monitoring System** (BCA152 Laboratory Activity No. 1).

---

## 1. Functional Verification Records (Part XVI)

| Test ID | Input / Stimulus | Expected Result | Actual Observed Result | Result |
| :--- | :--- | :--- | :--- | :---: |
| **FT-01** | Change DHT22 temperature to 28.0 °C | OLED display updates temperature reading to 28.0 °C | OLED page `TEMPERATURE` updated to `28.0 C` | **PASS** |
| **FT-02** | Change DHT22 humidity to 75.0 % | OLED display updates humidity reading to 75.0 % | OLED page `HUMIDITY` updated to `75.0 %` | **PASS** |
| **FT-03** | Adjust LDR light intensity to 80% | OLED display updates light percentage to 80 % | OLED page `LIGHT` updated to `80 %` | **PASS** |
| **FT-04** | Rotate rotary encoder clockwise | Display navigates to next page (`TEMP` -> `HUM` -> `LIGHT` -> `MOTION`) | Screen transitioned forward with wraparound | **PASS** |
| **FT-05** | Rotate rotary encoder counterclockwise | Display navigates to previous page (`MOTION` -> `LIGHT` -> `HUM` -> `TEMP`) | Screen transitioned backward with wraparound | **PASS** |
| **FT-06** | Set temperature above 30.0 °C (e.g. 32.5 °C) | Alarm buzzer activates, `EVENT_ALARM` bit set, OLED displays `[ALARM]` banner | Buzzer toggled high, serial logged warning, OLED rendered `ROOM MONITOR [ALARM]` | **PASS** |
| **FT-07** | Return temperature to normal (e.g. 24.0 °C) | Alarm buzzer deactivates, `EVENT_ALARM` bit cleared, banner returns to normal | Buzzer turned off, serial logged normal status, OLED banner restored | **PASS** |
| **FT-08** | Trigger PIR motion sensor HIGH | PIR motion detected, `EVENT_MOTION` set, system remains in `ACTIVE` state | Serial logged `[MOTION] PIR Sensor: Motion Detected!`, system state `ACTIVE` | **PASS** |
| **FT-09** | Allow 15 seconds of inactivity without PIR motion | System state machine transitions to `INACTIVE`, OLED blanks/powers off | Serial logged state transition after 15s, OLED power disabled (0xAE) | **PASS** |
| **FT-10** | Trigger PIR motion sensor HIGH while system is `INACTIVE` | Motion restores system state to `ACTIVE`, OLED powers on and resumes rendering | Serial logged restoration, OLED re-enabled (0xAF), display rendering resumed | **PASS** |

---

## 2. Deliberate FreeRTOS Fault Experiments (Part XVII)

### Fault Experiment 1: Remove Blocking Delay
- **Action:** Temporarily removed `vTaskDelayUntil()` from `SensorTask`, causing an infinite unblocked `while(1)` loop.
- **Observation:** The CPU core executing `SensorTask` reached 100% utilization. Lower-priority tasks on the same core (such as `DisplayTask` at Priority 1) were starved of CPU execution time, causing the OLED screen to freeze and fail to update.
- **Root Cause & Technical Reasoning:** In FreeRTOS pre-emptive priority-based scheduling, a higher-priority task (`SensorTask` at Priority 2) that never enters the `Blocked` state monopolizes the processor. Lower-priority ready tasks never receive CPU time slices.
- **Restoration:** Restored `vTaskDelayUntil(&lastWakeTime, pdMS_TO_TICKS(2000))`, returning `SensorTask` to periodic blocking execution.

### Fault Experiment 2: Change Task Priority
- **Action:** Temporarily assigned Priority 5 to `SensorTask` and set its polling rate to 10 ms.
- **Observation:** Interactive user controls (rotary encoder input handled by `InputTask` at Priority 3) became noticeably sluggish and missed step transitions during rapid rotation.
- **Root Cause & Technical Reasoning:** Frequent execution of a high-priority task preempts medium-priority tasks (`InputTask`), delaying ISR/task processing of quadrature signals. Priority assignment must reflect scheduling urgency rather than task importance.
- **Restoration:** Restored `SensorTask` to Priority 2 and `InputTask` to Priority 3.

### Fault Experiment 3: Remove Mutex Protection
- **Action:** Removed `xSemaphoreTake(serialMutex, ...)` and `xSemaphoreGive(serialMutex)` surrounding concurrent `printf` statements in `SensorTask`, `MotionTask`, `InputTask`, and `AlarmTask`.
- **Observation:** Diagnostic serial output lines became garbled and interleaved when multiple tasks attempted to print simultaneously (e.g., `[SENS[MOTION] PIR Sensor: Motion Detected!OR] Temp: 25.4 C`).
- **Root Cause & Technical Reasoning:** UART hardware register buffers are a non-reentrant shared resource. Concurrent access from multiple task contexts without mutual exclusion results in race conditions and corrupted character streams.
- **Restoration:** Re-wrapped all serial diagnostic outputs with `serialMutex`.
