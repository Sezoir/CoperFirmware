#pragma once
// Std libs
#include <chrono>

// External libs
#include <mbed.h>
#include <gsl/gsl>

// Project includes
#include "Driver.hpp"
#include "DShot.hpp"

using namespace units::literals;

namespace Copter::Engine
{
    class Motor
    {
    public:
        /**
         * @brief: Profiles for acceleration of the motor
         */
        enum class Profile : uint8_t
        {
            FastRamp,
            SlowRamp
        };
        /**
         * @brief: Constructor
         * @param: protocol: Reference to the protocol for the motor to use (e.g DShot)
         * @param: profile: The speed profile for acceleration of the motor speed
         * @param delay: The time delay between update calls @todo: May remove in future
         */
        Motor(Driver& protocol, Motor::Profile profile, std::chrono::duration<int64_t, std::milli>&& delay);

        /**
         * @brief: Constructor
         * @param: protocol: Pointer to the protocol for the motor to use (e.g DShot)
         * @param: profile: The speed profile for acceleration of the motor speed
         * @param delay: The time delay between update calls @todo: May remove in future
         */
        Motor(Driver* protocol, Motor::Profile profile, std::chrono::duration<int64_t, std::milli>&& delay);

        /**
         * @brief: Basic/empty constructor
         */
        Motor() = default;

        /**
         * @brief: Destructor
         */
        ~Motor() = default;

        void init(Driver& protocol, Motor::Profile profile, std::chrono::duration<int64_t, std::milli>&& delay);

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

    private:
        /**
         * @brief: Updates mSpeed according to a 'fast' ramp
         */
        void fastRamp();

        /**
         * @brief: Update mSpeed according to a 'slow' ramp
         */
        void slowRamp();

        // Default values
        Driver* mProtocol = nullptr;
        Profile mProfile = Profile::SlowRamp;
        std::chrono::duration<int64_t, std::milli> mDelay = 1ms;
        units::velocity::speed_t mSpeed = 0_sd;
        units::velocity::speed_t mDesSpeed = 0_sd;

        // Has the class been setup
        bool isInit = false;
    };
} // namespace Copter::Engine
