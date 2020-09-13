#pragma once
// Std libs
#include <array>
// External libs
#include <units.h>
// Project files
#include "BaseSensor.hpp"

namespace Copter::Sensors::Interfaces
{
    class Gyroscope : public BaseSensor
    {
    public:
        [[nodiscard]] virtual std::array<units::angular_velocity::degrees_per_second_t, 3> readGyro() const = 0;
    };
} // namespace Copter::Sensors::Interfaces