#pragma once

// External libs
#include <units.h>
#include <mbed.h>
// Project files
#include "AccelVoter.hpp"
#include "GyroVoter.hpp"
#include "MagVoter.hpp"
#include "Filters/LinearKalman.hpp"
#include "Filters/Complementary.hpp"

namespace Copter::Sensors
{
    template <typename RollFilter = Filters::LinearKalman<units::angle::degree_t>,
              typename PitchFilter = Filters::LinearKalman<units::angle::degree_t>,
              typename YawFilter = Filters::LinearKalman<units::angle::degree_t>,
              typename RollMix = Filters::Complementary, typename PitchMix = Filters::Complementary,
              typename YawMix = Filters::Complementary>
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
            std::array<units::angle::degree_t, 2> accelAngle = {0_deg, 0_deg}; // Roll, Pitch

            // Calculate time difference
            auto curTime = units::time::millisecond_t(Kernel::Clock::now().time_since_epoch().count());
            auto diffTime = curTime - prevTime;
            prevTime = curTime;

            // Calculate change in angle for accelerometer
            accelAngle[0] = units::math::atan2(
                accel[1], units::math::sqrt(units::math::pow<2>(accel[0]) + units::math::pow<2>(accel[2])));
            accelAngle[1] = units::math::atan2(
                accel[0], units::math::sqrt(units::math::pow<2>(accel[1]) + units::math::pow<2>(accel[2])));

            // Calculate change in angle for gyro
            mGyroAngle[0] += gyro[1] * diffTime;
            mGyroAngle[1] += gyro[0] * diffTime;
            mGyroAngle[2] += gyro[2] * diffTime;

            // Use filter to combine data for returnAngle
            mRoll = mRollFilter(mGyroAngle[0], accelAngle[0]);
            mPitch = mPitchFilter(mGyroAngle[1], accelAngle[1]);
            mYaw = mYawFilter(mGyroAngle[2]);

            mRoll = mRollKalmin(mRoll);
            mPitch = mPitchKalmin(mPitch);
            mYaw = mYawKalmin(mYaw);
        }

    private:
        // Voters
        AccelVoter& mAccel;
        GyroVoter& mGyro;
        MagVoter& mMag;

        // Filters
        RollMix mRollFilter = {};
        PitchMix mPitchFilter = {};
        YawMix mYawFilter = {};

        RollFilter mRollKalmin = {2_deg, 0_deg, 2_deg};
        PitchFilter mPitchKalmin = {2_deg, 0_deg, 2_deg};
        YawFilter mYawKalmin = {2_deg, 0_deg, 2_deg};

        // Current angles
        units::angle::degree_t mRoll = 0_deg;
        units::angle::degree_t mPitch = 0_deg;
        units::angle::degree_t mYaw = 0_deg;

        // Current angle
        std::array<units::angle::degree_t, 3> mGyroAngle = {0_deg, 0_deg, 0_deg}; // Roll, Pitch, Yaw

        // Previous time
        units::time::millisecond_t prevTime =
            units::time::millisecond_t(Kernel::Clock::now().time_since_epoch().count());
    };
} // namespace Copter::Sensors
