#pragma once
// Std libs
#include <array>
// External libs
#include <units.h>
// Project files
#include "Sensor.hpp"

namespace Copter::Sensors::Interfaces
{
    class Gyroscope : public virtual Sensor
    {
    public:
        [[nodiscard]] virtual std::array<units::angular_velocity::degrees_per_second_t, 3> readGyro() const = 0;

    protected:
        Gyroscope()
        {
            mType.gyroscope = true;
        }
    };
} // namespace Copter::Sensors::Interfaces