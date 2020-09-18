#pragma once
// Std libs
#include <array>
// External libs
#include <units.h>
// Project files
#include "BaseSensor.hpp"

namespace Copter::Sensors::Interfaces
{
    class Accelerometer : public BaseSensor
    {
    public:
        [[nodiscard]] virtual std::array<units::acceleration::meters_per_second_squared_t, 3> readAccel() = 0;
    };
} // namespace Copter::Sensors::Interfaces