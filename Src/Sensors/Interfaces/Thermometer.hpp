#pragma once
// Std libs
#include <array>
// External libs
#include <units.h>
// Project files
#include "BaseSensor.hpp"

namespace Copter::Sensors::Interfaces
{
    class Thermometer : public BaseSensor
    {
    public:
        [[nodiscard]] virtual units::temperature::celsius_t readTemp() const = 0;
    };
} // namespace Copter::Sensors::Interfaces