#include <unity.h>
#include <vector>
#include <Wire.h>
#include <Arduino.h>
#include <InertialSensor.h>

InertialSensor sensor(HZ_7p5);

void test_setup()
{
    sensor.begin();
    Serial.println("TEST");
    TEST_ASSERT(sensor.isEnabled() == true);
}

void test_buffer_clear()
{
    sensor.clearBuffer();

    InertiaReading *readings = sensor.getBuffer();
    TEST_ASSERT_EQUAL(0, sensor.getBufferPtr());

    float mean = 0.0f;
    for (int i = 0; i < 250; i++)
    {
        TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, readings[i].accelX);
        TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, readings[i].accelY);
        TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, readings[i].accelZ);

        TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, readings[i].gyroX);
        TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, readings[i].gyroY);
        TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, readings[i].gyroZ);
    }
}

void test_buffer_at_HZ_7p5()
{
    sensor.clearBuffer();
    sensor.updatePowerMode(HZ_7p5);

    // Collect 1 second of samples
    int start = millis();
    while (millis() - start < 1000)
    {
        int now = millis();
        sensor.update(now);
    }

    // check pointer index. At 7.5hz it should be 7-8 samples in 1 sec
    int idx = sensor.getBufferPtr();
    TEST_ASSERT(idx > 0);
}

void test_buffer_at_HZ_15()
{
    sensor.clearBuffer();
    sensor.updatePowerMode(HZ_15);

    int start = millis();
    while (millis() - start < 1000)
    {
        int now = millis();
        sensor.update(now);
    }

    int idx = sensor.getBufferPtr();
    TEST_ASSERT(idx > 0);
}

void test_buffer_at_HZ_60()
{
    sensor.clearBuffer();
    sensor.updatePowerMode(HZ_60);

    int start = millis();
    while (millis() - start < 1000)
    {
        int now = millis();
        sensor.update(now);
    }

    int idx = sensor.getBufferPtr();
    TEST_ASSERT(idx > 0);
}

void test_buffer_at_HZ_120()
{
    sensor.clearBuffer();
    sensor.updatePowerMode(HZ_120);

    int start = millis();
    while (millis() - start < 1000)
    {
        int now = millis();
        sensor.update(now);
    }

    int idx = sensor.getBufferPtr();
    TEST_ASSERT(idx > 0);
}

void test_buffer_at_HZ_240()
{   
    sensor.clearBuffer();
    sensor.updatePowerMode(HZ_240);

    int start = millis();
    while (millis() - start < 1000)
    {
        int now = millis();
        sensor.update(now);
    }

    int idx = sensor.getBufferPtr();
    TEST_ASSERT(idx > 0);
}

void setup()
{   
    Wire.begin();
    Serial.begin(115200);
    delay(2000);

    UNITY_BEGIN();
    RUN_TEST(test_setup);
    RUN_TEST(test_buffer_clear);
    RUN_TEST(test_buffer_at_HZ_7p5);
    RUN_TEST(test_buffer_at_HZ_15);
    RUN_TEST(test_buffer_at_HZ_60);
    RUN_TEST(test_buffer_at_HZ_120);
    RUN_TEST(test_buffer_at_HZ_240);
    UNITY_END();
}

void loop()
{
}