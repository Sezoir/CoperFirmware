#pragma once
// Std libs
#include <array>
// External libs
#include <units.h>

namespace Copter::Sensors::Filters
{
    using namespace units::literals;

    class Complementary
    {
    public:
        template <typename T>
        T operator()(T major, T minor)
        {
            return (0.98 * major) + (0.02 * minor);
        }
    };
} // namespace Copter::Sensors::Filters