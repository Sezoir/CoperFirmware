#pragma once
// Std libs
#include <array>
// External libs
#include <units.h>
// Project files
#include "Expansion.hpp"

namespace Copter::Sensors::Interfaces
{
    using namespace units::literals;

    class Angle
    {
    public:
        [[nodiscard]] constexpr units::angle::degree_t getRoll() const
        {
            return mRoll;
        }

        [[nodiscard]] constexpr units::angle::degree_t getPitch() const
        {
            return mPitch;
        }

        [[nodiscard]] constexpr units::angle::degree_t getYaw() const
        {
            return mYaw;
        }

        virtual void update() = 0;

    protected:
        units::angle::degree_t mRoll = 0_deg;
        units::angle::degree_t mPitch = 0_deg;
        units::angle::degree_t mYaw = 0_deg;
    };
} // namespace Copter::Sensors::Interfaces