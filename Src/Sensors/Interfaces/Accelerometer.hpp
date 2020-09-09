#pragma once
// Std libs
#include <array>
// External libs
#include <units.h>
// Project files
#include "Sensor.hpp"

namespace Copter::Sensors::Interfaces
{
    class Accelerometer : public virtual Sensor
    {
    public:
        [[nodiscard]] virtual std::array<units::acceleration::meters_per_second_squared_t, 3> readAccel() const = 0;

    protected:
        Accelerometer()
        {
            mType.accelerometer = true;
        }
    };
} // namespace Copter::Sensors::Interfaces