#pragma once
// Std libs
#include <cstdlib>

// Project files
#include "AccelVoter.hpp"
#include "MPU9250.hpp"

namespace Copter::Sensors
{
    class Repository
    {
    public:
        void build()
        {
            // Setup sensors
            mSensor1.init();
            // Register sensors to voters
            mAccelVoter.assign(mSensor1);
            // Register data expansions
        }

        template <typename T>
        constexpr auto& get()
        {
            if constexpr(std::is_same<T, AccelVoter>::value)
            {
                return mAccelVoter;
            }
        }

    private:
        // Voters
        AccelVoter mAccelVoter;

        // Sensors
        MPU9250 mSensor1;
    };
} // namespace Copter::Sensors
