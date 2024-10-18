#ifndef SERVO_CONTROL_H
#define SERVO_CONTROL_H

#define SERVO_CH0_PIN GPIO_NUM_4
#define SERVO_CH1_PIN GPIO_NUM_3

void configure_servo(void);
void _set_angle(float angle1, float angle2);

#endif // SERVO_CONTROL_H