#pragma once
#include <mbed.h>
#include <chrono>
#include "Driver.hpp"

namespace Copter::Engine
{
    class Motor
    {
    public:
        enum class Profile
        {
            FastRamp,
            SlowRamp
        };
        Motor(Driver&, Profile, std::chrono::duration<double> && delay);
        ~Motor() = default;

        /**
         * @brief: Set the speed you want the motor to go to
         * @param: speed: The desired speed
         */
        void setSpeed(units::velocity::speed_t speed);

        /**
         * @brief: Send the signal and update the speed based on the current profile.
         * @note: This function should be called at regular intervals.
         */
        void update();

        /**
         * @brief: Change the current profile of the motor.
         * @todo: Change to only change profile after the required speed has been met.
         */
        void setProfile(Profile);


    private:
        void fastRamp();
        void slowRamp();

        Driver& mProtocol;
        Profile mProfile;

        units::velocity::speed_t mSpeed;
        units::velocity::speed_t mDesSpeed;
        std::chrono::duration<double>& mDelay;

    };
}

