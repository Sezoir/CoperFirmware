#pragma once
// Std libs
#include <array>
// External libs
#include <units.h>
// Project files
#include "Sensor.hpp"

namespace Copter::Sensors::Interfaces
{
    class Magnetometer : public virtual Sensor
    {
    public:
        [[nodiscard]] virtual std::array<units::magnetic_field_strength::microtesla_t, 3> readMag() const = 0;

    private:
        Magnetometer()
        {
            mType.magnetometer = true;
        }
    };
} // namespace Copter::Sensors::Interfaces