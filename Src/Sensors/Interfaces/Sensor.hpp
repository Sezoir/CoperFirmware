#pragma once

namespace Copter::Sensors::Interfaces
{
    class Sensor
    {
    public:
        struct Type
        {
            bool accelerometer = false;
            bool gyroscope = false;
            bool magnetometer = false;
            bool thermometer = false;
        };

        [[nodiscard]] constexpr Type getType() const
        {
            return mType;
        }

    protected:
        Type mType = {};
    };

} // namespace Copter::Sensors::Interfaces
