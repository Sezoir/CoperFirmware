#pragma once
// External libs
#include <units.h>
// Project files
#include "Interfaces/Voter.hpp"
#include "Interfaces/Thermometer.hpp"

namespace Copter::Sensors
{
    using namespace units::literals;

    class ThermoVoter : public Interfaces::Thermometer, public Interfaces::Voter<Interfaces::Thermometer>
    {
    public:
        [[nodiscard]] units::temperature::celsius_t readTemp() const override
        {
            // Init average temperature
            units::temperature::celsius_t avrTemp = 0_degC;
            // Loop through each thermometer
            for(uint i = 0; i < mCount; i++)
            {
                // Get sensor pointer
                auto* sensor = static_cast<Interfaces::Thermometer*>(mSensors.at(i));
                // Get sum of temperatures
                units::temperature::celsius_t temp = sensor->readTemp();
                avrTemp += temp;
            }
            // Get average
            avrTemp /= mCount;

            return avrTemp;
        }
    };

} // namespace Copter::Sensors
