#include "Interface.hpp"

namespace Copter::Engine
{

    Interface::Interface()
    {
        setup();
    }

    void Interface::setup()
    {
        std::array<PinName, MOTOR_NUM> pins = MOTOR_PINS;
        for(uint8_t cnt = 0; cnt < MOTOR_NUM; cnt++)
        {
            auto proto = new MOTOR_PROTOCOL(pins[cnt], MOTOR_PROTOCOL_PARAMETERS);
            proto->setup();
            mMotors[cnt] = Motor(proto, Motor::Profile::MOTOR_PROFILE, MOTOR_DELAY);
        }
    }

    void Interface::update()
    {
        for(Motor& motor : mMotors)
        {
            motor.update();
        }
    }

    void Interface::setSpeed(uint8_t id, units::velocity::speed_t speed)
    {
        this->mMotors[id].setSpeed(speed);
    }
} // namespace Copter::Engine
