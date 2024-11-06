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
    uint8_t reg_addr = OUTX_L_XL;
    uint8_t accel_data[4];
    float filtered_x = 0;
    float filtered_y = 0;

    while (1) {
        // Read both X and Y axes in a single transaction
        esp_err_t err = i2c_master_transmit_receive(dev_handle, &reg_addr, 1, accel_data, 4, -1);
        if (err != ESP_OK) {
            ESP_LOGE(TAG, "Failed to read accelerometer: %s", esp_err_to_name(err));
            vTaskDelay(pdMS_TO_TICKS(100)); // Wait before retry
            continue;
        }
        
        // Combine high and low bytes for each axis
        int16_t x = (int16_t)((accel_data[1] << 8) | accel_data[0]);
        int16_t y = (int16_t)((accel_data[3] << 8) | accel_data[2]);
        
        // Convert raw values to g
        float x_g = x / ACCEL_SENSITIVITY;
        float y_g = y / ACCEL_SENSITIVITY;

        // Apply moving average filter
        filtered_x = FILTER_ALPHA * x_g + (1 - FILTER_ALPHA) * filtered_x;
        filtered_y = FILTER_ALPHA * y_g + (1 - FILTER_ALPHA) * filtered_y;

        // Map accelerometer tilt to servo angles (0-180 degrees)
        float angle1 = (filtered_x + 1.0f) * SERVO_MAP_SCALE;
        float angle2 = (filtered_y + 1.0f) * SERVO_MAP_SCALE;

        // Clamp angles to valid range
        angle1 = fminf(fmaxf(angle1, 0.0f), 180.0f);
        angle2 = fminf(fmaxf(angle2, 0.0f), 180.0f);

        // For debugging
        ESP_LOGD(TAG, "Angles: %.2f, %.2f", angle1, angle2);

        // Set servo angles
        _set_angle(angle1, angle2);

        vTaskDelay(pdMS_TO_TICKS(10));
    }
}
