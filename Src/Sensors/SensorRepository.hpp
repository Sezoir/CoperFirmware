#pragma once
// Std libs //
#include <cstdlib>

// Project files //
// Voters
#include "AccelVoter.hpp"
#include "GyroVoter.hpp"
#include "MagVoter.hpp"
#include "ThermoVoter.hpp"
// Extensions
#include "AngleExtension.hpp"
#include "AltitudeExtension.hpp"
// Filters
#include "Filters/Complementary.hpp"
#include "Filters/None.hpp"
// Interfaces
#include "Interfaces/Sensor.hpp"
#include "Interfaces/Accelerometer.hpp"
#include "Interfaces/Gyroscope.hpp"
#include "Interfaces/Magnetometer.hpp"
#include "Interfaces/Thermometer.hpp"
#include "Interfaces/Angle.hpp"
#include "Interfaces/Altitude.hpp"
// Sensors
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
            init(mSensor1);

            // Register sensors to voters
            mAccelVoter.assign(mSensor1);
            mGyroVoter.assign(mSensor1);
            mMagVoter.assign(mSensor1);
            mThermoVoter.assign(mSensor1);
        }

        /**
         * @brief Returns a reference to the correct voter/expansion.
         * @tparam T: The voter of expansion class
         * @return
         */
        template <typename T>
        constexpr auto& get()
        {
            if constexpr(std::is_same<T, AccelVoter>::value || std::is_same<T, Interfaces::Accelerometer>::value)
            {
                return mAccelVoter;
            }
            else if constexpr(std::is_same<T, GyroVoter>::value || std::is_same<T, Interfaces::Gyroscope>::value)
            {
                return mGyroVoter;
            }
            else if constexpr(std::is_same<T, MagVoter>::value || std::is_same<T, Interfaces::Magnetometer>::value)
            {
                return mMagVoter;
            }
            else if constexpr(std::is_same<T, ThermoVoter>::value || std::is_same<T, Interfaces::Thermometer>::value)
            {
                return mThermoVoter;
            }
            else if constexpr(std::is_same<T, Interfaces::Angle>::value)
            {
                return mAngle;
            }
            else if constexpr(std::is_same<T, Interfaces::Altitude>::value)
            {
                return mAltitude;
            }
        }

    private:
        void init(Interfaces::Sensor& sensor)
        {
            bool success = sensor.init();
            if(!success)
            {
                MBED_ERROR(MBED_MAKE_ERROR(MBED_MODULE_APPLICATION, MBED_ERROR_INITIALIZATION_FAILED),
                           "Sensor initialisation failed.");
            }
        }

        // Voters
        AccelVoter mAccelVoter{};
        GyroVoter mGyroVoter{};
        MagVoter mMagVoter{};
        ThermoVoter mThermoVoter{};

        // Data expansions
        AngleExtension<Filters::LinearKalman<units::angle::degree_t>, Filters::LinearKalman<units::angle::degree_t>,
                       Filters::LinearKalman<units::angle::degree_t>, Filters::Complementary, Filters::Complementary,
                       Filters::None>
            mAngle{mAccelVoter, mGyroVoter, mMagVoter};
        AltitudeExtension<Filters::LinearKalman<units::length::meter_t>> mAltitude{mAccelVoter};

        // Sensors
        MPU9250 mSensor1 = {PD_13, PD_12};
    };
} // namespace Copter::Sensors
