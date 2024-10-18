| Supported Targets | ESP32 | ESP32-C2 | ESP32-C3 | ESP32-C5 | ESP32-C6 | ESP32-H2 | ESP32-P4 | ESP32-S2 | ESP32-S3 |
| ----------------- | ----- | -------- | -------- | -------- | -------- | -------- | -------- | -------- | -------- |

# ESP32 Accelerometer (LSM6DS3TR-C) Servo Control
This project demonstrates how to read data from an LSM6DS3TR-C accelerometer and control two servos based on the accelerometer's x and y tilt using an ESP32. The servos' angles are adjusted according to the filtered accelerometer readings.

## How to use example
We encourage the users to use the example as a template for the new projects.
A recommended way is to follow the instructions on a [docs page](https://docs.espressif.com/projects/esp-idf/en/latest/api-guides/build-system.html#start-a-new-project).

## Pin Configuration

- **I2C Pins**:
  - SDA: GPIO 6
  - SCL: GPIO 7
- **Servo Pins**:
  - Servo 1: GPIO 4
  - Servo 2: GPIO 3

These can be configured in i2c_utils.h and servo_control.h

## License
This project is licensed under the MIT License.

## Author

- **Christoffer Kolbe Boye-Hansen**
  - Email: [christoffer.kolbe@gmail.com](mailto:christoffer.kolbe@gmail.com)
  - GitHub: [promillen](https://github.com/promillen)
  - Portfolio: [portfolio](https://promillen.github.io/Portfolio/)