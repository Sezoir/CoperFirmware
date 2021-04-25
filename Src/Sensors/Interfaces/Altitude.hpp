#pragma once
// Std libs
#include <array>
// External libs
#include <units.h>
// Project files
#include "BaseSensor.hpp"

namespace Copter::Sensors::Interfaces
{
    class Altitude : public BaseSensor
    {
    public:
        [[nodiscard]] virtual units::length::meter_t readHeight() = 0;
    };
} // namespace Copter::Sensors::Interfaces