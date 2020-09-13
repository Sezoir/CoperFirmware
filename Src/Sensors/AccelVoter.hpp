#pragma once
// External libs
#include <units.h>
// Project files
#include "Interfaces/Voter.hpp"
#include "Interfaces/Accelerometer.hpp"

namespace Copter::Sensors
{
    using namespace units::literals;
    class AccelVoter : public Interfaces::Accelerometer, public Interfaces::Voter<Interfaces::Accelerometer>
    {
    public:
        [[nodiscard]] std::array<units::acceleration::meters_per_second_squared_t, 3> readAccel() const override
        {
            // Init average acceleration
            std::array<units::acceleration::meters_per_second_squared_t, 3> avrAccel{0_mps_sq, 0_mps_sq, 0_mps_sq};
            // Loop through each accelerometer
            for(uint i = 0; i < mCount; i++)
            {
                // Get sensor pointer
                auto* accelSensor = static_cast<Interfaces::Accelerometer*>(mSensors.at(i));
                // Get sum of accelerations
                std::array<units::acceleration::meters_per_second_squared_t, 3> accel = accelSensor->readAccel();
                for(uint8_t j = 0; j < 3; j++)
                    avrAccel[j] += accel[j];
            }
            // Get average
            for(units::acceleration::meters_per_second_squared_t& value : avrAccel)
                value /= mCount;
            return avrAccel;
        }
    };

} // namespace Copter::Sensors
