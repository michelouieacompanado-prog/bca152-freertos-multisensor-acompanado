#ifndef SYSTEM_STATE_H
#define SYSTEM_STATE_H

#include <stdbool.h>
#include <stdint.h>

#define INACTIVITY_TIMEOUT_MS 15000 // 15 seconds (Section 31)

// Section 32: System State Machine
enum class SystemState {
    ACTIVE,
    INACTIVE
};

// Pure hardware-independent state transition evaluation (Section 32 & Section 42)
SystemState evaluateSystemState(SystemState currentState, bool motionDetected, uint32_t elapsedInactiveMs, uint32_t timeoutMs);

void init_system_state(void);
SystemState get_system_state(void);
void StateTask(void *pvParameters);

#endif // SYSTEM_STATE_H
