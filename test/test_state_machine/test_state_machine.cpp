#include <unity.h>
#include "system_state.h"
#include "../../src/system_state.cpp"

// -----------------------------------------------------------------------------
// Category: System State Machine Logic (Section 43: 4 Required Tests)
// Inactivity timeout (15000 ms) and motion-based wakeup evaluation
// -----------------------------------------------------------------------------

// Test 1: ACTIVE state with no timeout (< 15000 ms elapsed, no motion) -> remains ACTIVE
void test_state_active_no_timeout(void) {
    SystemState result = evaluateSystemState(SystemState::ACTIVE, false, 5000, INACTIVITY_TIMEOUT_MS);
    TEST_ASSERT_TRUE(result == SystemState::ACTIVE);
}

// Test 2: ACTIVE state with timeout (>= 15000 ms elapsed, no motion) -> transitions to INACTIVE
void test_state_active_timeout(void) {
    SystemState result = evaluateSystemState(SystemState::ACTIVE, false, 15000, INACTIVITY_TIMEOUT_MS);
    TEST_ASSERT_TRUE(result == SystemState::INACTIVE);

    SystemState resultExceeded = evaluateSystemState(SystemState::ACTIVE, false, 20000, INACTIVITY_TIMEOUT_MS);
    TEST_ASSERT_TRUE(resultExceeded == SystemState::INACTIVE);
}

// Test 3: INACTIVE state with no motion -> remains INACTIVE
void test_state_inactive_no_motion(void) {
    SystemState result = evaluateSystemState(SystemState::INACTIVE, false, 30000, INACTIVITY_TIMEOUT_MS);
    TEST_ASSERT_TRUE(result == SystemState::INACTIVE);
}

// Test 4: INACTIVE state with motion detected -> transitions to ACTIVE
void test_state_inactive_motion(void) {
    SystemState result = evaluateSystemState(SystemState::INACTIVE, true, 30000, INACTIVITY_TIMEOUT_MS);
    TEST_ASSERT_TRUE(result == SystemState::ACTIVE);
}

#if defined(NATIVE) || !defined(ESP_PLATFORM)
int main(void) {
#else
extern "C" void app_main(void) {
#endif
    UNITY_BEGIN();
    RUN_TEST(test_state_active_no_timeout);
    RUN_TEST(test_state_active_timeout);
    RUN_TEST(test_state_inactive_no_motion);
    RUN_TEST(test_state_inactive_motion);
    UNITY_END();
#if defined(NATIVE) || !defined(ESP_PLATFORM)
    return 0;
#endif
}
