#include "Interface.hpp"

namespace Copter::Engine
{

    Interface::Interface()
        : mMotors()
    {
        setup();
    }

    void Interface::setup()
    {
        std::array<PinName, MOTOR_NUM> pins = MOTOR_PINS;
        for(uint8_t cnt = 0; cnt < MOTOR_NUM; cnt++)
        {
            MOTOR_PROTOCOL proto(pins[cnt], MOTOR_PROTOCOL_PARAMETERS);
            this->mMotors[cnt] = Motor(proto, Motor::Profile::MOTOR_PROFILE, MOTOR_DELAY);
            //            this->mMotors[cnt] = Motor(std::move(MOTOR_PROTOCOL(pins[cnt], MOTOR_PROTOCOL_PARAMETERS)),
            //                                       Motor::Profile::MOTOR_PROFILE, MOTOR_DELAY);
        }
    }

    void Interface::update()
    {
        for(Motor& motor : this->mMotors)
        {
            motor.update();
        }
    }

    void Interface::setSpeed(uint8_t id, units::velocity::speed_t speed)
    {
        this->mMotors[id].setSpeed(speed);
    }
} // namespace Copter::Engine
