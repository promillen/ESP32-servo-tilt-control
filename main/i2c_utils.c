#include "i2c_utils.h"
#include "driver/i2c_master.h"
#include "esp_err.h"

void configure_i2c(i2c_master_bus_handle_t *bus_handle, i2c_master_dev_handle_t *dev_handle) {
    // Master bus configuration
    i2c_master_bus_config_t i2c_mst_config = {
        .clk_source = I2C_CLK_SRC_DEFAULT,  // Default clock source
        .i2c_port = I2C_PORT_NUM,
        .scl_io_num = I2C_MASTER_SCL_IO,
        .sda_io_num = I2C_MASTER_SDA_IO,
        .glitch_ignore_cnt = 7,             // Glitch filter count
        .intr_priority = 1,                 // Interrupt priority
        .flags.enable_internal_pullup = false,
    };

    // Initialize I2C master bus
    ESP_ERROR_CHECK(i2c_new_master_bus(&i2c_mst_config, bus_handle));

    // I2C device configuration
    i2c_device_config_t dev_cfg = {
        .dev_addr_length = I2C_ADDR_BIT_LEN_7,
        .device_address = I2C_SLAVE_ADDR,
        .scl_speed_hz = I2C_CLOCK_SPEED,
    };

    // Add I2C device to the bus
    ESP_ERROR_CHECK(i2c_master_bus_add_device(*bus_handle, &dev_cfg, dev_handle));
}
