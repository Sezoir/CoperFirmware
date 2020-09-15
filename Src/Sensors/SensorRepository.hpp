#pragma once
// Std libs
#include <cstdlib>

// Project files
#include "AccelVoter.hpp"
#include "GyroVoter.hpp"
#include "MagVoter.hpp"
#include "ThermoVoter.hpp"
#include "MPU9250.hpp"

namespace Copter::Sensors
{
    class SensorRepository
    {
    public:
        SensorRepository() = default;

        void build()
        {
            // Setup sensors
            mSensor1.init();

            // Register sensors to voters
            mAccelVoter.assign(mSensor1);
            mGyroVoter.assign(mSensor1);
            mMagVoter.assign(mSensor1);
            mThermoVoter.assign(mSensor1);

            // Register data expansions
        }

        template <typename T>
        constexpr auto& get()
        {
            if constexpr(std::is_same<T, AccelVoter>::value)
            {
                return mAccelVoter;
            }
            else if constexpr(std::is_same<T, GyroVoter>::value)
            {
                return mGyroVoter;
            }
            else if constexpr(std::is_same<T, MagVoter>::value)
            {
                return mMagVoter;
            }
            else if constexpr(std::is_same<T, ThermoVoter>::value)
            {
                return mThermoVoter;
            }
        }

    private:
        // Voters
        AccelVoter mAccelVoter = {};
        GyroVoter mGyroVoter = {};
        MagVoter mMagVoter = {};
        ThermoVoter mThermoVoter = {};

        // Data expansions

        // Sensors
        MPU9250 mSensor1 = {PD_13, PD_12};
    };
} // namespace Copter::Sensors
