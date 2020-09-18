#pragma once
// External libs
#include <units.h>
// Project files
#include "Interfaces/Voter.hpp"
#include "Interfaces/Accelerometer.hpp"
#include "Filters/LowPass.hpp"

namespace Copter::Sensors
{
    using namespace units::literals;

    class AccelVoter : public Interfaces::Accelerometer, public Interfaces::Voter<Interfaces::Accelerometer>
    {
    public:
        [[nodiscard]] std::array<units::acceleration::meters_per_second_squared_t, 3> readAccel() override
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

            // Process through filter
            avrAccel[0] = mXFilter(avrAccel[0]);
            avrAccel[1] = mYFilter(avrAccel[1]);
            avrAccel[2] = mZFilter(avrAccel[2]);

            return avrAccel;
        }

    private:
        Filters::LowPass<units::acceleration::meters_per_second_squared_t> mXFilter = {0.5, 0_mps_sq};
        Filters::LowPass<units::acceleration::meters_per_second_squared_t> mYFilter = {0.5, 0_mps_sq};
        Filters::LowPass<units::acceleration::meters_per_second_squared_t> mZFilter = {0.5, 9.81_mps_sq};
    };

} // namespace Copter::Sensors
