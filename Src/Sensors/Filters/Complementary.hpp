#pragma once
// Std libs
#include <chrono>
#include <array>
// External libs
#include <mbed.h>
#include <units.h>
// Project files
#include "../Interfaces/Angle.hpp"
#include "../SensorRepository.hpp"
#include "HighPass.hpp"
#include "LowPass.hpp"

namespace Copter::Sensors::Filters
{
    using namespace units::literals;

    class Complementary : public Interfaces::Angle
    {
    public:
        Complementary(AccelVoter& accel, GyroVoter& gyro)
            : mAccelVoter(accel)
            , mGyroVoter(gyro)
        {
        }

        ~Complementary() = default;

        void update() override
        {
            // Get new sensor values
            std::array<units::acceleration::meters_per_second_squared_t, 3> temp = mAccelVoter.readAccel();

            std::array<units::acceleration::standard_gravity_t, 3> accel = {};
            for(uint8_t i = 0; i < 3; i++)
            {
                accel[i] = temp[i];
            }
            std::array<units::angular_velocity::degrees_per_second_t, 3> gyro = mGyroVoter.readGyro();

            // Filter raw data
            for(uint8_t i = 0; i < 3; i++)
            {
                accel[i] = mLowPass[i](accel[i]);
                gyro[i] = mHighPass[i](gyro[i]);
            }

            // Calculate difference in time between calls
            auto curTime = units::time::millisecond_t(Kernel::Clock::now().time_since_epoch().count());
            auto diffTime = curTime - prevTime;
            prevTime = curTime;

            // Get calculated angles for accel and gyro
            // Order of array goes: Roll, Pitch, Yaw
            std::array<units::angle::degree_t, 3> gyroAngle = {0_deg, 0_deg, 0_deg};
            std::array<units::angle::degree_t, 2> accelAngle = {0_deg, 0_deg};

            gyroAngle[0] = gyro[1] * diffTime;
            gyroAngle[1] = gyro[0] * diffTime;
            gyroAngle[2] = gyro[2] * diffTime;

            accelAngle[0] = units::math::atan2(accel[1], accel[2]);
            accelAngle[1] = units::math::atan2(
                accel[0], units::math::sqrt(units::math::pow<2>(accel[1]) + units::math::pow<2>(accel[2])));

            mRoll += (0.98 * gyroAngle[0]) + (0.02 * accelAngle[0]);
            mPitch += (0.98 * gyroAngle[1]) + (0.02 * accelAngle[1]);
            mYaw += gyroAngle[2];
        }

    private:
        units::time::millisecond_t prevTime =
            units::time::millisecond_t(Kernel::Clock::now().time_since_epoch().count());

        // Voters
        AccelVoter& mAccelVoter;
        GyroVoter& mGyroVoter;

        // Filters
        std::array<HighPass<units::angular_velocity::degrees_per_second_t>, 3> mHighPass = {
            HighPass<units::angular_velocity::degrees_per_second_t>(0.1, 0_deg_per_s, 0_deg_per_s),
            HighPass<units::angular_velocity::degrees_per_second_t>(0.1, 0_deg_per_s, 0_deg_per_s),
            HighPass<units::angular_velocity::degrees_per_second_t>(0.1, 0_deg_per_s, 0_deg_per_s)};
        std::array<LowPass<units::acceleration::standard_gravity_t>, 3> mLowPass = {
            LowPass<units::acceleration::standard_gravity_t>(0.1, 0_SG),
            LowPass<units::acceleration::standard_gravity_t>(0.1, 0_SG),
            LowPass<units::acceleration::standard_gravity_t>(0.1, 0_SG)};
    };

} // namespace Copter::Sensors::Filters