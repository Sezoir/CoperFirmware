#pragma once
#include <array>
#include <memory>

#include "Motor.hpp"
#include "Driver.hpp"

#include <mbed.h>

#define MOTOR_NUM 4
#define MOTOR_PROTOCOL DShot
#define MOTOR_PROTOCOL_PARAMETERS 1200
#define MOTOR_PINS {PB_8, PC_6};

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

    private:
        std::array<Motor, MOTOR_NUM> mMotors;


    };
}

