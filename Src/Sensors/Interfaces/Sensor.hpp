#pragma once
// Project files
#include "BaseSensor.hpp"

namespace Copter::Sensors::Interfaces
{
    class Sensor : public BaseSensor
    {
    public:
        /**
         * @brief: Virtual function to be called to initial sensors.
         * @return bool: Success of initialisation of chip.
         */
        virtual bool init() = 0;
    };

} // namespace Copter::Sensors::Interfaces
