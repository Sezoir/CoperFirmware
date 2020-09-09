#pragma once
// Std libs
#include <array>
// External libs
#include <units.h>
// Project files
#include "Sensor.hpp"

namespace Copter::Sensors::Interfaces
{
    class Thermometer : public virtual Sensor
    {
    public:
        [[nodiscard]] virtual units::temperature::celsius_t readTemp() const = 0;

    protected:
        Thermometer()
        {
            mType.thermometer = true;
        }
    };
} // namespace Copter::Sensors::Interfaces