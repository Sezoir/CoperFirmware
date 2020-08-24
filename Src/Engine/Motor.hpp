#pragma once
#include <mbed.h>
#include <chrono>
#include "Driver.hpp"

namespace Copter::Engine
{
    class Motor
    {
    public:
        /**
         * @brief: Profiles for acceleration of the motor
         */
        enum class Profile
        {
            FastRamp,
            SlowRamp
        };
        /**
         * @brief: Constructor
         * @param delay: The time delay between update calls @todo: May remove in future
         */
        Motor(Driver&, Profile, std::chrono::duration<int64_t, std::micro> && delay);

        /**
         * @brief: Basic/empty constructor
         */
        Motor();

        /**
         * @brief: Destructor
         */
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
        /**
         * @brief: Updates mSpeed according to a 'fast' ramp
         */
        void fastRamp();

        /**
         * @brief: Update mSpeed according to a 'slow' ramp
         */
        void slowRamp();

        Driver mProtocol;
        Profile mProfile;

        units::velocity::speed_t mSpeed;
        units::velocity::speed_t mDesSpeed;
        std::chrono::duration<int64_t, std::micro> mDelay;

    };
}

