#pragma once
// External libs
#include <units.h>
// Project files
#include "Interfaces/Voter.hpp"
#include "Interfaces/Magnetometer.hpp"

namespace Copter::Sensors
{
    using namespace units::literals;

    class MagVoter : public Interfaces::Magnetometer, public Interfaces::Voter<Interfaces::Magnetometer>
    {
    public:
        [[nodiscard]] std::array<units::magnetic_field_strength::microtesla_t, 3> readMag() const override
        {
            // Init average mags
            std::array<units::magnetic_field_strength::microtesla_t, 3> avrMag{0_uTe, 0_uTe, 0_uTe};
            // Loop through each mag
            for(uint i = 0; i < mCount; i++)
            {
                // Get sensor pointer
                auto* sensor = static_cast<Interfaces::Magnetometer*>(mSensors.at(i));
                // Get sum of mags
                std::array<units::magnetic_field_strength::microtesla_t, 3> mag = sensor->readMag();
                for(uint8_t j = 0; j < 3; j++)
                    avrMag[j] += mag[j];
            }
            // Get average
            for(units::magnetic_field_strength::microtesla_t& value : avrMag)
                value /= mCount;
            return avrMag;
        }
    };

} // namespace Copter::Sensors
