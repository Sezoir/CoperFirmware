#pragma once
// Std libs
#include <array>
// Project files
#include "Sensor.hpp"

namespace Copter::Sensors::Interfaces
{
    template <typename T>
    class Voter
    {
    public:
        Voter& assign(T& sensor)
        {
            mSensors[mCount] = &sensor;
            mCount++;
            return *this;
        }

    protected:
        // Current count of sensors
        uint mCount = 0;
        // Array of sensors
        std::array<T*, 25> mSensors = {};
    };
} // namespace Copter::Sensors::Interfaces