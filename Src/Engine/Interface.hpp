#pragma once
#include <array>
#include <memory>
#include <chrono>

#include "Motor.hpp"
#include "Driver.hpp"

#include <mbed.h>

#define MOTOR_NUM                 2
#define MOTOR_PROTOCOL            DShot
#define MOTOR_PROTOCOL_PARAMETERS 1200
#define MOTOR_PINS                {PB_8, PC_6};
#define MOTOR_DELAY               1ms
#define MOTOR_PROFILE             SlowRamp

#if MOTOR_PROTOCOL == DShot
#include "DShot.hpp"
#endif

namespace Copter::Engine
{
    class Interface
    {
    public:
        Interface();

        ~Interface() = default;

        void setup();
        void update();
        void setSpeed(uint8_t id, units::velocity::speed_t speed);

    private:
        std::array<Motor, MOTOR_NUM> mMotors = {};
    };
} // namespace Copter::Engine
