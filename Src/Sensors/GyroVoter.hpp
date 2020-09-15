#pragma once
// External libs
#include <units.h>
// Project files
#include "Interfaces/Voter.hpp"
#include "Interfaces/Gyroscope.hpp"

namespace Copter::Sensors
{
    using namespace units::literals;

    class GyroVoter : public Interfaces::Gyroscope, public Interfaces::Voter<Interfaces::Gyroscope>
    {
    public:
        [[nodiscard]] std::array<units::angular_velocity::degrees_per_second_t, 3> readGyro() const override
        {
            // Init average angular velocity
            std::array<units::angular_velocity::degrees_per_second_t, 3> avrGyro{0_deg_per_s, 0_deg_per_s, 0_deg_per_s};
            // Loop through each gyroscope
            for(uint i = 0; i < mCount; i++)
            {
                // Get sensor pointer
                auto* sensor = static_cast<Interfaces::Gyroscope*>(mSensors.at(i));
                // Get sum of angular velocities
                std::array<units::angular_velocity::degrees_per_second_t, 3> gyro = sensor->readGyro();
                for(uint8_t j = 0; j < 3; j++)
                    avrGyro[j] += gyro[j];
            }
            // Get average
            for(units::angular_velocity::degrees_per_second_t& value : avrGyro)
                value /= mCount;
            return avrGyro;
        }
    };

} // namespace Copter::Sensors
