#ifndef I2C_UTILS_H
#define I2C_UTILS_H

#include "driver/i2c_master.h"

#define I2C_MASTER_SDA_IO 6       // GPIO number for I2C SDA
#define I2C_MASTER_SCL_IO 7       // GPIO number for I2C SCL
#define I2C_PORT_NUM      0       // I2C port number
#define I2C_SLAVE_ADDR    0x6A    // I2C slave device address for LSM6DS3TR-C
#define I2C_CLOCK_SPEED   100000  // I2C clock speed: 100kHz (low-speed mode)

void configure_i2c(i2c_master_bus_handle_t *bus_handle, i2c_master_dev_handle_t *dev_handle);

#endif // I2C_UTILS_H