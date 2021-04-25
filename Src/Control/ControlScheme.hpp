#pragma once
// External libs
#include <units.h>
// Project files
#include "../Engine/ProtocolInterface.hpp"
#include "../Sensors/SensorRepository.hpp"
#include "Controller.hpp"

namespace Copter::Control
{
    /**
     * @brief Base class for any control scheme to inherit from (e.g: an Self-Level class).
     * @tparam ControlType: A "units" type which will be controlled by the Controller.
     */
    class ControlScheme
    {
    public:
        /**
         * @brief Initialises the control scheme for whatever it may need (for example it may need to calibrate a
         * minimum throttle to counteract gravity).
         */
        virtual void init() = 0;

        /**
         * @brief Updates the Controller of the new desired values as well calls the Controller update.
         */
        virtual void update() = 0;
    };
} // namespace Copter::Control
