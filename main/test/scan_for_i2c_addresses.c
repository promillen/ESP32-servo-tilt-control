/*
 * I2C SCANNER/PROBER FOR ESP32
 * This program scans the I2C bus for connected devices and displays their addresses via serial output.
 *
 * Code based on example from UncleRus: https://github.com/UncleRus/esp-idf-i2cscan/blob/main/main/main.c 
 * and upgraded to newest I2C driver
 * 
 * Setup:
 * - Connect I2C devices and change SDA and SCL pins.
 * - Ensure external pull-up resistors are present if internal pull-ups are disabled.
 * 
 * This code is intended for use with ESP32 and ESP-IDF.
 * 
 * Author: [Christoffer Kolbe Boye-Hansen]
 * Date: [17/10/2024]
 * License: MIT License
 */

#include <stdio.h>
#include "driver/i2c_master.h"
#include "esp_log.h"
#include "esp_err.h"
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#define I2C_MASTER_SCL_IO 7           // GPIO for SCL
#define I2C_MASTER_SDA_IO 6           // GPIO for SDA
#define I2C_MASTER_PORT I2C_NUM_0     // I2C port
#define I2C_MASTER_FREQ_HZ 100000     // I2C frequency
#define I2C_MASTER_TIMEOUT_MS 1000    // Timeout for I2C transactions
#define SCAN_INTERVAL_MS 10000        // Interval between scans

#ifndef APP_CPU_NUM
#define APP_CPU_NUM PRO_CPU_NUM
#endif

static const char *TAG = "I2C_Probe";

void task(void *ignore) {
    i2c_master_bus_config_t i2c_mst_config = {
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .i2c_port = I2C_MASTER_PORT,
        .scl_io_num = I2C_MASTER_SCL_IO,
        .sda_io_num = I2C_MASTER_SDA_IO,
        .glitch_ignore_cnt = 7,
        .flags.enable_internal_pullup = false,
    };

    i2c_master_bus_handle_t bus_handle;

    esp_err_t ret = i2c_new_master_bus(&i2c_mst_config, &bus_handle);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to create I2C master bus: %s", esp_err_to_name(ret));
        vTaskDelete(NULL);  // Delete the task if bus creation fails
    }

    vTaskDelay(pdMS_TO_TICKS(100));
    while (1) {
        printf("     0  1  2  3  4  5  6  7  8  9  a  b  c  d  e  f\n");
        printf("00:         ");

        // Loop through all possible I2C addresses (0x03 to 0x77)
        for (uint8_t address = 0x03; address < 0x78; address++) {
            esp_err_t err = i2c_master_probe(bus_handle, address, I2C_MASTER_TIMEOUT_MS);

            if (address % 16 == 0) {
                printf("\n%.2x:", address);
            }

            if (err == ESP_OK) {
                printf(" %.2x", address);
            } else if (err == ESP_ERR_TIMEOUT) {
                printf("  T");
            } else if (err == ESP_ERR_INVALID_RESPONSE) {
                printf("  I");
            } else {
                printf(" --");
            }
        }

        printf("\n\n");
        vTaskDelay(pdMS_TO_TICKS(SCAN_INTERVAL_MS));
    }

    // Clean up (optional, but generally good practice)
    i2c_del_master_bus(bus_handle);
}

void app_main() {
    xTaskCreatePinnedToCore(task, TAG, configMINIMAL_STACK_SIZE * 8, NULL, 5, NULL, APP_CPU_NUM);
}
