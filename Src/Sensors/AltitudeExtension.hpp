#pragma once

// External libs
#include <units.h>
#include <mbed.h>
// Project files
#include "AccelVoter.hpp"
#include "Filters/LinearKalman.hpp"

namespace Copter::Sensors
{
    template <typename Filter = Filters::LinearKalman<units::length::meter_t>>
    class AltitudeExtension
    {
    public:
        explicit AltitudeExtension(AccelVoter& accel)
            : mAccel(accel)
        {
        }

        void update()
        {
            std::array<units::acceleration::meters_per_second_squared_t, 3> accel = mAccel.readAccel();

            // Calculate time difference
            auto curTime = units::time::millisecond_t(Kernel::Clock::now().time_since_epoch().count());
            auto diffTime = curTime - prevTime;
            prevTime = curTime;

            mHeight += mHeight + accel[1] * diffTime * diffTime;

            mHeight = mAltitudeFilter(mHeight);
        }

        [[nodiscard]] units::length::meter_t readHeight() const
        {
            return mHeight;
        }

    private:
        // Voters
        AccelVoter& mAccel;

        // Filters
        Filter mAltitudeFilter = {2_m, 0_m, 2_m};

        // Current angles
        units::length::meter_t mHeight = 0_m;

        // Previous time
        units::time::millisecond_t prevTime =
            units::time::millisecond_t(Kernel::Clock::now().time_since_epoch().count());
    };
} // namespace Copter::Sensors
