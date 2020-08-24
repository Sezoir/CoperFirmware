#include "Interface.hpp"

namespace Copter::Engine
{


    Interface::Interface() :
    mMotors()
    {
        setup();
    }

    void Interface::setup()
    {
        std::array<PinName, MOTOR_NUM> pins = MOTOR_PINS;
        for(uint8_t cnt = 0; cnt < MOTOR_NUM; cnt++)
        {
            MOTOR_PROTOCOL proto(pins[cnt], MOTOR_PROTOCOL_PARAMETERS);
        }
    }
}
