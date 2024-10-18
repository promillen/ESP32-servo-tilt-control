#ifndef ACCELEROMETER_H
#define ACCELEROMETER_H

#include "driver/i2c_master.h"

#define OUTX_L_XL 0x28            // X-axis accelerometer lower byte
#define OUTX_H_XL 0x29            // X-axis accelerometer higher byte
#define OUTY_L_XL 0x2A            // Y-axis accelerometer lower byte
#define OUTY_H_XL 0x2B            // Y-axis accelerometer higher byte

void configure_accelerometer(i2c_master_dev_handle_t dev_handle);
void accelerometer_task(void *arg);

#endif // ACCELEROMETER_H