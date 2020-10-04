#pragma once
// External libs
#include <units.h>
// Project files
#include "../Engine/ProtocolInterface.hpp"
#include "../Sensors/SensorRepository.hpp"
#include "Controller.hpp"

namespace Copter::Control
{
    template <typename ControlType>
    class ControlScheme
    {
    public:
        explicit ControlScheme(Engine::MotorContainer<Engine::MOTOR_PROTOCOL, MOTOR_COUNT>& motorContainer,
                               Sensors::SensorRepository& data)
            : mController(Controller<ControlType>(motorContainer))
            , mData(data)
        {
        }

        virtual units::angle::degree_t getRoll() = 0;
        virtual units::angle::degree_t getPitch() = 0;
        virtual units::angle::degree_t getYaw() = 0;
        virtual units::protocol::speed_t getThrottle() = 0;

    protected:
        Controller<ControlType> mController;
        Sensors::SensorRepository& mData;
    };
} // namespace Copter::Control
