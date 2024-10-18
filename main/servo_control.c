#include "servo_control.h"
#include "iot_servo.h"
#include "driver/gpio.h"
#include "unity.h"
#include "sdkconfig.h"

void configure_servo(void) {
    servo_config_t servo_cfg_ls = {
        .max_angle = 180,
        .min_width_us = 870,   // Lower bound for 0 degrees
        .max_width_us = 2150,  // Upper bound for 180 degrees
        .freq = 50,
        .timer_number = LEDC_TIMER_0,
        .channels = {
            .servo_pin = {
                SERVO_CH0_PIN,
                SERVO_CH1_PIN,
            },
            .ch = {
                LEDC_CHANNEL_0,
                LEDC_CHANNEL_1,
            },
        },
        .channel_number = 2,
    };
    TEST_ASSERT(ESP_OK == iot_servo_init(LEDC_LOW_SPEED_MODE, &servo_cfg_ls));
}

void _set_angle(float angle1, float angle2) {
    iot_servo_write_angle(LEDC_LOW_SPEED_MODE, 0, angle1); // Set angle for Servo 1
    iot_servo_write_angle(LEDC_LOW_SPEED_MODE, 1, angle2); // Set angle for Servo 2
}