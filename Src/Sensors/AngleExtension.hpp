#pragma once

// External libs
#include <units.h>
#include <mbed.h>
// Project files
#include "AccelVoter.hpp"
#include "GyroVoter.hpp"
#include "MagVoter.hpp"

namespace Copter::Sensors
{
    template <typename RollFilter, typename PitchFilter, typename YawFilter>
    class AngleExtension
    {
    public:
        AngleExtension(AccelVoter& accel, GyroVoter& gyro, MagVoter& mag)
            : mAccel(accel)
            , mGyro(gyro)
            , mMag(mag)
        {
        }

        [[nodiscard]] constexpr units::angle::degree_t getRoll() const
        {
            return mRoll;
        }

        [[nodiscard]] constexpr units::angle::degree_t getPitch() const
        {
            return mPitch;
        }

        [[nodiscard]] constexpr units::angle::degree_t getYaw() const
        {
            return mYaw;
        }

        void update()
        {
            // Get values
            std::array<units::acceleration::meters_per_second_squared_t, 3> accel = mAccel.readAccel();
            std::array<units::angular_velocity::degrees_per_second_t, 3> gyro = mGyro.readGyro();

            // Initialise angle vectors
            std::array<units::angle::degree_t, 2> accelAngle = {0_deg, 0_deg};       // Roll, Pitch
            std::array<units::angle::degree_t, 3> gyroAngle = {0_deg, 0_deg, 0_deg}; // Roll, Pitch, Yaw

            // Calculate time difference
            auto curTime = units::time::millisecond_t(Kernel::Clock::now().time_since_epoch().count());
            auto diffTime = curTime - prevTime;
            prevTime = curTime;

            // Calculate change in angle for accelerometer
            accelAngle[0] = units::math::atan2(accel[1], accel[2]);
            accelAngle[1] = units::math::atan2(
                accel[0], units::math::sqrt(units::math::pow<2>(accel[1]) + units::math::pow<2>(accel[2])));

            // Calculate change in angle for gyro
            gyroAngle[0] = gyro[0] * diffTime;
            gyroAngle[1] = gyro[1] * diffTime;
            gyroAngle[2] = gyro[2] * diffTime;

            // Use filter to combine data for returnAngle
            mRoll = mRollFilter(accelAngle[0], gyroAngle[0]);
            mPitch = mPitchFilter(accelAngle[1], gyroAngle[1]);
            mYaw = mYawFilter(gyroAngle[2]);
        }

    private:
        // Voters
        AccelVoter& mAccel;
        GyroVoter& mGyro;
        MagVoter& mMag;

        // Filters
        RollFilter mRollFilter = {};
        PitchFilter mPitchFilter = {};
        YawFilter mYawFilter = {};

        // Current angles
        units::angle::degree_t mRoll = 0_deg;
        units::angle::degree_t mPitch = 0_deg;
        units::angle::degree_t mYaw = 0_deg;

        // Previous time
        units::time::millisecond_t prevTime =
            units::time::millisecond_t(Kernel::Clock::now().time_since_epoch().count());
    };
} // namespace Copter::Sensors
