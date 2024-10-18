#include "i2c_utils.h"
#include "accelerometer.h"
#include "servo_control.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "unity.h"

void app_main(void) {
    i2c_master_bus_handle_t bus_handle;
    i2c_master_dev_handle_t dev_handle;

    configure_i2c(&bus_handle, &dev_handle);
    configure_accelerometer(dev_handle);

    // Configure servo
    configure_servo();

    xTaskCreate(accelerometer_task, "accelerometer_task", 4096, (void *)dev_handle, 5, NULL);
}