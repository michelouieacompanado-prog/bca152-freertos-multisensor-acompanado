#include <unity.h>
#include "alarm.h"
#include "../../src/alarm.cpp"

// -----------------------------------------------------------------------------
// Category: Temperature Alarm Logic (Section 43: 5 Required Tests)
// Thresholds: LOW = 18.0 °C, HIGH = 30.0 °C
// -----------------------------------------------------------------------------

// Test 1: Below lower threshold (< 18.0 °C -> LOW_TEMPERATURE)
void test_temperature_below_lower_threshold(void) {
    TEST_ASSERT_TRUE(evaluateTemperature(17.9f) == AlarmState::LOW_TEMPERATURE);
    TEST_ASSERT_TRUE(evaluateTemperature(10.0f) == AlarmState::LOW_TEMPERATURE);
    TEST_ASSERT_TRUE(evaluateTemperature(-5.0f) == AlarmState::LOW_TEMPERATURE);
}

// Test 2: Exactly lower threshold (== 18.0 °C -> NORMAL)
void test_temperature_exactly_lower_threshold(void) {
    TEST_ASSERT_TRUE(evaluateTemperature(18.0f) == AlarmState::NORMAL);
}

// Test 3: Normal operating value (18.0 °C < T < 30.0 °C -> NORMAL)
void test_temperature_normal_value(void) {
    TEST_ASSERT_TRUE(evaluateTemperature(22.0f) == AlarmState::NORMAL);
    TEST_ASSERT_TRUE(evaluateTemperature(25.4f) == AlarmState::NORMAL);
    TEST_ASSERT_TRUE(evaluateTemperature(29.9f) == AlarmState::NORMAL);
}

// Test 4: Exactly upper threshold (== 30.0 °C -> NORMAL)
void test_temperature_exactly_upper_threshold(void) {
    TEST_ASSERT_TRUE(evaluateTemperature(30.0f) == AlarmState::NORMAL);
}

// Test 5: Above upper threshold (> 30.0 °C -> HIGH_TEMPERATURE)
void test_temperature_above_upper_threshold(void) {
    TEST_ASSERT_TRUE(evaluateTemperature(30.1f) == AlarmState::HIGH_TEMPERATURE);
    TEST_ASSERT_TRUE(evaluateTemperature(35.0f) == AlarmState::HIGH_TEMPERATURE);
    TEST_ASSERT_TRUE(evaluateTemperature(60.0f) == AlarmState::HIGH_TEMPERATURE);
}

extern "C" void app_main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_temperature_below_lower_threshold);
    RUN_TEST(test_temperature_exactly_lower_threshold);
    RUN_TEST(test_temperature_normal_value);
    RUN_TEST(test_temperature_exactly_upper_threshold);
    RUN_TEST(test_temperature_above_upper_threshold);
    UNITY_END();
}
