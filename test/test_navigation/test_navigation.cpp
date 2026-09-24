#include <unity.h>
#include "input.h"
#include "../../src/input.cpp"

// -----------------------------------------------------------------------------
// Category: Display Navigation Logic (Section 43: 4 Required Tests)
// Forward / reverse transitions and circular wraparound
// -----------------------------------------------------------------------------

// Test 1: Forward step transitions
void test_navigation_forward_transitions(void) {
    TEST_ASSERT_TRUE(nextDisplayMode(DisplayMode::TEMPERATURE) == DisplayMode::HUMIDITY);
    TEST_ASSERT_TRUE(nextDisplayMode(DisplayMode::HUMIDITY) == DisplayMode::LIGHT);
    TEST_ASSERT_TRUE(nextDisplayMode(DisplayMode::LIGHT) == DisplayMode::MOTION);
}

// Test 2: Forward wraparound (MOTION -> TEMPERATURE)
void test_navigation_forward_wraparound(void) {
    TEST_ASSERT_TRUE(nextDisplayMode(DisplayMode::MOTION) == DisplayMode::TEMPERATURE);
}

// Test 3: Reverse step transitions
void test_navigation_reverse_transitions(void) {
    TEST_ASSERT_TRUE(previousDisplayMode(DisplayMode::MOTION) == DisplayMode::LIGHT);
    TEST_ASSERT_TRUE(previousDisplayMode(DisplayMode::LIGHT) == DisplayMode::HUMIDITY);
    TEST_ASSERT_TRUE(previousDisplayMode(DisplayMode::HUMIDITY) == DisplayMode::TEMPERATURE);
}

// Test 4: Reverse wraparound (TEMPERATURE -> MOTION)
void test_navigation_reverse_wraparound(void) {
    TEST_ASSERT_TRUE(previousDisplayMode(DisplayMode::TEMPERATURE) == DisplayMode::MOTION);
}

#if defined(NATIVE) || !defined(ESP_PLATFORM)
int main(void) {
#else
extern "C" void app_main(void) {
#endif
    UNITY_BEGIN();
    RUN_TEST(test_navigation_forward_transitions);
    RUN_TEST(test_navigation_forward_wraparound);
    RUN_TEST(test_navigation_reverse_transitions);
    RUN_TEST(test_navigation_reverse_wraparound);
    UNITY_END();
#if defined(NATIVE) || !defined(ESP_PLATFORM)
    return 0;
#endif
}
