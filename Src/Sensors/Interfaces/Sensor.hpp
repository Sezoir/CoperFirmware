#pragma once

namespace Copter::Sensors::Interfaces
{
    class Sensor
    {
    public:
        // Types of sensors
        struct Type
        {
            bool accelerometer = false;
            bool gyroscope = false;
            bool magnetometer = false;
            bool thermometer = false;
        };

        /**
         * @brief: Returns mType.
         * @return Type: Struct representing the type of sensors chip has.
         */
        [[nodiscard]] constexpr Type getType() const
        {
            return mType;
        }

        /**
         * @brief: Virtual function to be called to initial sensors.
         * @return bool: Success of initialisation of chip.
         */
        virtual bool init() = 0;

    protected:
        Type mType = {};
    };

} // namespace Copter::Sensors::Interfaces
