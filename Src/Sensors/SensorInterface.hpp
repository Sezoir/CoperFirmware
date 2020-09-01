#pragma once
#include <array>

namespace Copter::Sensors
{
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

        [[nodiscard]] virtual bool init() const
        {
            return false;
        }

        [[nodiscard]] virtual SensorType getType() const
        {
            SensorType type;
            return type;
        }

        [[nodiscard]] virtual std::array<int, 3> readAccel() const
        {
            return std::array<int, 3>{0, 0, 0};
        }

        [[nodiscard]] virtual std::array<int, 3> readGyro() const
        {
            return std::array<int, 3>{0, 0, 0};
        }

        [[nodiscard]] virtual std::array<int, 3> readMag() const
        {
            return std::array<int, 3>{0, 0, 0};
        }

        [[nodiscard]] virtual int readTemp() const
        {
            return 0;
        }
    };
} // namespace Copter::Sensors
