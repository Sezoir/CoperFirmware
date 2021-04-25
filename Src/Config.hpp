#pragma once

// @TODO: Look at adding a json config file, with a python script that will then process and auto generate this file.
// @TODO: Also look at reworking without defines.
// @TODO: Seperate Config.hpp file from project, so unit tests can have there own Config.hpp files.

/**
 * Motor Config
 */
#define MOTOR_COUNT               2 //@TODO: Remove
#define MOTOR_PROTOCOL            DShot
#define MOTOR_PROTOCOL_PARAMETERS 1200       // Look at std::any
#define MOTOR_PINS                PB_8, PC_6 // Add check for 4 pins. Maybe even split into 4 defines?
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