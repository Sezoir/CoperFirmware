#pragma once
// Std libs
#include <array>
// External libs
#include <units.h>

namespace Copter::Sensors
{
    using namespace units::literals;
    class SensorInterface
    {
    public:
        struct SensorType
        {
            SensorType() = default;
            bool accelerometer = false;
            bool gyroscope = false;
            bool magnetometer = false;
            bool temperature = false;
        };

        virtual ~SensorInterface() = default;

        [[nodiscard]] virtual bool init()
        {
            return false;
        }

        [[nodiscard]] virtual SensorType getType() const
        {
            SensorType type;
            return type;
        }

        [[nodiscard]] virtual std::array<units::acceleration::meters_per_second_squared_t, 3> readAccel() const
        {
            return std::array<units::acceleration::meters_per_second_squared_t, 3>{0_mps_sq, 0_mps_sq, 0_mps_sq};
        }

        [[nodiscard]] virtual std::array<units::angular_velocity::degrees_per_second_t, 3> readGyro() const
        {
            return std::array<units::angular_velocity::degrees_per_second_t, 3>{0_deg_per_s, 0_deg_per_s, 0_deg_per_s};
        }

        [[nodiscard]] virtual std::array<units::magnetic_field_strength::microtesla_t, 3> readMag() const
        {
            return std::array<units::magnetic_field_strength::microtesla_t, 3>{0_uTe, 0_uTe, 0_uTe};
        }

        [[nodiscard]] virtual units::temperature::celsius_t readTemp() const
        {
            return units::temperature::celsius_t(0);
        }
    };
} // namespace Copter::Sensors
