#include "accelerometer.h"
#include "i2c_utils.h"
#include "servo_control.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "driver/i2c_master.h"
#include <math.h>

static const char *TAG = "LSM6DS3TR";

void configure_accelerometer(i2c_master_dev_handle_t dev_handle) {
    uint8_t write_buf[2];

    // Set ODR to 52Hz (0100 in bits 3:0 of CTRL1_XL)
    uint8_t ctrl1_xl_value = 0x40;  // 0100 for ODR 52Hz, 00 for +/- 2g full scale
    write_buf[0] = 0x10; // CTRL1_XL register
    write_buf[1] = ctrl1_xl_value;
    ESP_ERROR_CHECK(i2c_master_transmit(dev_handle, write_buf, 2, -1));
}

void accelerometer_task(void *arg) {
    i2c_master_dev_handle_t dev_handle = (i2c_master_dev_handle_t)arg;
    uint8_t reg_addr_x = OUTX_L_XL;
    uint8_t reg_addr_y = OUTY_L_XL;
    uint8_t accel_data[2];
    float filtered_x = 0;
    float filtered_y = 0;
    float alpha = 0.05; // Smoothing factor for exponential moving average (0 < alpha < 1)

    while (1) {
        // Read X-axis data
        ESP_ERROR_CHECK(i2c_master_transmit_receive(dev_handle, &reg_addr_x, 1, accel_data, 2, -1));
        int16_t x = (int16_t)((accel_data[1] << 8) | accel_data[0]);
        float x_g = x / 16384.0; // Convert raw value to g (assuming +/- 2g scale)

        // Apply moving average filter for X-axis
        filtered_x = alpha * x_g + (1 - alpha) * filtered_x;

        // Read Y-axis data
        ESP_ERROR_CHECK(i2c_master_transmit_receive(dev_handle, &reg_addr_y, 1, accel_data, 2, -1));
        int16_t y = (int16_t)((accel_data[1] << 8) | accel_data[0]);
        float y_g = y / 16384.0; // Convert raw value to g

        // Apply moving average filter for Y-axis
        filtered_y = alpha * y_g + (1 - alpha) * filtered_y;

        // Map the tilt of the x-axis to servo angle for Servo 1 (0-180 degrees)
        float angle1 = (filtered_x + 1.0) * 90.0; // Map from [-1, 1] to [0, 180]
        if (angle1 < 0) angle1 = 0;
        if (angle1 > 180) angle1 = 180;

        // Map the tilt of the y-axis to servo angle for Servo 2 (0-180 degrees)
        float angle2 = (filtered_y + 1.0) * 90.0; // Map from [-1, 1] to [0, 180]
        if (angle2 < 0) angle2 = 0;
        if (angle2 > 180) angle2 = 180;

        // ESP_LOGI(TAG, "Servo Angle: %.2f", angle);
        _set_angle(angle1, angle2);

        vTaskDelay(pdMS_TO_TICKS(10));
    }
}
