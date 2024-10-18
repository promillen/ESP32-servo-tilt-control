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

static void _set_angle(ledc_mode_t speed_mode, float angle)
{
    iot_servo_write_angle(speed_mode, 0, angle);
}

void app_main(void) {
    // Configuration for the servo
    servo_config_t servo_cfg_ls = {
        .max_angle = 180,
        .min_width_us = 870,   // Lower bound for 0 degrees
        .max_width_us = 2150,  // Upper bound for 180 degrees
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

#ifdef CONFIG_IDF_TARGET_ESP32
    // Configuration for high-speed mode (ESP32 only)
    servo_config_t servo_cfg_hs = {
        .max_angle = 180,
        .min_width_us = 900,
        .max_width_us = 2100,
        .freq = 100,  // High-speed mode uses 100 Hz PWM
        .timer_number = LEDC_TIMER_0,
        .channels = {
            .servo_pin = {
                SERVO_CH0_PIN,  // Same pin for high-speed mode in this example
            },
            .ch = {
                LEDC_CHANNEL_0,
            },
        },
        .channel_number = 1,
    };
    TEST_ASSERT(ESP_OK == iot_servo_init(LEDC_HIGH_SPEED_MODE, &servo_cfg_hs));
#endif

    float angle_ls, angle_hs;
    
    // Sweep from 0 to 180 degrees with finer control (0.1 degree steps)
    // for (float i = 0; i <= 180; i += 0.5) {
    //     _set_angle(LEDC_LOW_SPEED_MODE, i);  // Set angle in low-speed mode

#ifdef CONFIG_IDF_TARGET_ESP32
        _set_angle(LEDC_HIGH_SPEED_MODE, (180 - i));  // Set angle in high-speed mode
#endif

    vTaskDelay(pdMS_TO_TICKS(5));  // Reduced delay for smoother movement

    iot_servo_read_angle(LEDC_LOW_SPEED_MODE, 0, &angle_ls);

#ifdef CONFIG_IDF_TARGET_ESP32
    iot_servo_read_angle(LEDC_HIGH_SPEED_MODE, 0, &angle_hs);
    ESP_LOGI("servo", "[%.1f|%.2f], [%.1f|%.2f]", i, angle_ls, (180 - i), angle_hs);
#else
    ESP_LOGI("servo", "[%.1f|%.2f]", i, angle_ls);
#endif
    }

    // Deinitialize the servo
    iot_servo_deinit(LEDC_LOW_SPEED_MODE);
#ifdef CONFIG_IDF_TARGET_ESP32
    iot_servo_deinit(LEDC_HIGH_SPEED_MODE);
#endif
}