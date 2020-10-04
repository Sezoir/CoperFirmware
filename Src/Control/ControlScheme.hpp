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
    template <typename ControlType>
    class ControlScheme
    {
    public:
        /**
         * @brief Constructor.
         * @param motorContainer: Reference to the container of motors.
         * @param data: Reference to the sensor repository.
         */
        explicit ControlScheme(Engine::MotorContainer<Engine::MOTOR_PROTOCOL, MOTOR_COUNT>& motorContainer,
                               Sensors::SensorRepository& data)
            : mController(Controller<ControlType>(motorContainer))
            , mData(data)
        {
        }

        /**
         * @brief Initialises the control scheme for whatever it may need (for example it may need to calibrate a
         * minimum throttle to counteract gravity).
         */
        virtual void init() = 0;

        /**
         * @brief Updates the Controller of the new desired values as well calls the Controller update.
         */
        virtual void update() = 0;

    protected:
        Controller<ControlType> mController;
        Sensors::SensorRepository& mData;
    };
} // namespace Copter::Control
