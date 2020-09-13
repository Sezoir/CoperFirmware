#pragma once
// Std libs
#include <array>
// External libs
#include <units.h>
// Project files
#include "BaseSensor.hpp"

namespace Copter::Sensors::Interfaces
{
    class Magnetometer : public BaseSensor
    {
    public:
        [[nodiscard]] virtual std::array<units::magnetic_field_strength::microtesla_t, 3> readMag() const = 0;
    };
} // namespace Copter::Sensors::Interfaces