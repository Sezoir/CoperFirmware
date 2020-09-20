#pragma once
// External libs
#include <units.h>
// Project files
#include "Interfaces/Voter.hpp"
#include "Interfaces/Gyroscope.hpp"
#include "Filters/HighPass.hpp"

namespace Copter::Sensors
{
    using namespace units::literals;

    class GyroVoter : public Interfaces::Gyroscope, public Interfaces::Voter<Interfaces::Gyroscope>
    {
    public:
        [[nodiscard]] std::array<units::angular_velocity::degrees_per_second_t, 3> readGyro() override
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

            // Process through filter
            avrGyro[0] = mXFilter(avrGyro[0]);
            avrGyro[1] = mXFilter(avrGyro[1]);
            avrGyro[2] = mXFilter(avrGyro[2]);

            return avrGyro;
        }

    private:
        Filters::HighPass<units::angular_velocity::degrees_per_second_t> mXFilter = {0.00001, 0_deg_per_s, 0_deg_per_s};
        Filters::HighPass<units::angular_velocity::degrees_per_second_t> mYFilter = {0.00001, 0_deg_per_s, 0_deg_per_s};
        Filters::HighPass<units::angular_velocity::degrees_per_second_t> mZFilter = {0.00001, 0_deg_per_s, 0_deg_per_s};
    };

} // namespace Copter::Sensors
