/*
 * SERVO PULSE WIDTH TESTER FOR ESP32
 * This program controls a servo motor using the LEDC peripheral of the ESP32.
 * It initializes the servo with specified parameters and tests the max and min pulse widths
 *
 * Setup:
 * - Connect the servo to choosen GPIO pin (default GPIO_NUM_4).
 * - Run the program and observe the servo movement.
 * - When movement is observed, note min and max pulse widths.
 *
 * This code is intended for use with ESP32 and ESP-IDF.
 *
 * Author: [Christoffer Kolbe Boye-Hansen]
 * Date: [17/10/2024]
 * License: MIT License
 */

#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "iot_servo.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "unity.h"
#include "sdkconfig.h"

#define SERVO_CH0_PIN GPIO_NUM_4
#define TEST_STEP_US 10  // Step size in microseconds

static void _set_pulse_width(ledc_mode_t speed_mode, uint32_t width_us)
{
    iot_servo_write_angle(speed_mode, 0, (width_us - 500) * 180 / (2500 - 500));
}

void app_main(void) {
    // Configuration for the servo
    servo_config_t servo_cfg_ls = {
        .max_angle = 180,
        .min_width_us = 500,   // Start with a safe minimum pulse width
        .max_width_us = 2500,  // Start with a safe maximum pulse width
        .freq = 50,
        .timer_number = LEDC_TIMER_0,
        .channels = {
            .servo_pin = {
                SERVO_CH0_PIN,
            },
            .ch = {
                LEDC_CHANNEL_0,
            },
        },
        .channel_number = 1,
    };

    TEST_ASSERT(ESP_OK == iot_servo_init(LEDC_LOW_SPEED_MODE, &servo_cfg_ls));

    uint32_t test_pulse_width;
    ESP_LOGI("servo", "Testing minimum pulse width");
    
    // Test minimum pulse width
    for (test_pulse_width = 500; test_pulse_width <= 1500; test_pulse_width += TEST_STEP_US) {
        _set_pulse_width(LEDC_LOW_SPEED_MODE, test_pulse_width);
        ESP_LOGI("servo", "Testing pulse width: %luus", (unsigned long)test_pulse_width);
        vTaskDelay(pdMS_TO_TICKS(500));  // Wait to observe the movement
    }

    ESP_LOGI("servo", "Testing maximum pulse width");
    
    // Test maximum pulse width
    for (test_pulse_width = 2500; test_pulse_width >= 1500; test_pulse_width -= TEST_STEP_US) {
        _set_pulse_width(LEDC_LOW_SPEED_MODE, test_pulse_width);
        ESP_LOGI("servo", "Testing pulse width: %luus", (unsigned long)test_pulse_width);
        vTaskDelay(pdMS_TO_TICKS(500));  // Wait to observe the movement
    }

    // Deinitialize the servo
    iot_servo_deinit(LEDC_LOW_SPEED_MODE);
}
