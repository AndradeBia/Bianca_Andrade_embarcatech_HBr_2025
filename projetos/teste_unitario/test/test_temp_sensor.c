#include "unity.h"
#include "temp_sensor.h"

void setUp(void) {

}

void tearDown(void) {

}

void test_adc_to_celsius_should_ReturnApprox27CForSpecificVoltage(void) {
    uint16_t adc_val_at_27c = 876;

    float expected_temp = 27.0f;
    float actual_temp = adc_to_celsius(adc_val_at_27c);


    TEST_ASSERT_FLOAT_WITHIN(0.1f, expected_temp, actual_temp);
}

void test_adc_to_celsius_should_ReturnApprox20CForAnotherVoltage(void) {
    uint16_t adc_val_approx_20c = 862;

    float expected_temp = 20.0f;
    float actual_temp = adc_to_celsius(adc_val_approx_20c);

    TEST_ASSERT_FLOAT_WITHIN(0.1f, expected_temp, actual_temp);
}


int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_adc_to_celsius_should_ReturnApprox27CForSpecificVoltage);
    RUN_TEST(test_adc_to_celsius_should_ReturnApprox20CForAnotherVoltage);


    return UNITY_END();
}