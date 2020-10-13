#pragma once

/**
 * Motor Config
 */
#define MOTOR_COUNT               2
#define MOTOR_PROTOCOL            DShot
#define MOTOR_PROTOCOL_PARAMETERS 1200
#define MOTOR_PINS                PB_8, PC_6
#define MOTOR_DELAY               1ms
#define MOTOR_PROFILE             SlowRamp

/**
 * Controller Config
 */
#define ROLL_PID_P 0.4
#define ROLL_PID_I 0.4
#define ROLL_PID_D 0.2

#define PITCH_PID_P 0.4
#define PITCH_PID_I 0.4
#define PITCH_PID_D 0.4

#define YAW_PID_P 0.4
#define YAW_PID_I 0.4
#define YAW_PID_D 0.4

#define THROTTLE_PID_P 0.4
#define THROTTLE_PID_I 0.4
#define THROTTLE_PID_D 0.4