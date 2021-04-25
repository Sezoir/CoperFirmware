#include "SelfLevelControl.hpp"

namespace Copter::Control
{
    SelfLevelControl::SelfLevelControl(Engine::MotorContainer<Engine::MOTOR_PROTOCOL, MOTOR_COUNT>& motorContainer,
                                       Sensors::SensorRepository& data)
        : mController(Controller<ThrottleType, AngleType>(motorContainer))
        , mData(data)
    {
    }

    void SelfLevelControl::init()
    {
    }

    void SelfLevelControl::update()
    {
        // Update angles (note this should also update the accelerometer reading so we don't explicitly update it)
        // @TODO: Look at updating all sensors at one points, than having to update every time in each function.
        mData.get<Sensors::Interfaces::Angle>().update();
        mData.get<Sensors::Interfaces::Altitude>().update();

        // Get current angles and z-acceleration
        AngleType curRoll = mData.get<Sensors::Interfaces::Angle>().getRoll();
        AngleType curPitch = mData.get<Sensors::Interfaces::Angle>().getPitch();
        AngleType curYaw = mData.get<Sensors::Interfaces::Angle>().getYaw();
        ThrottleType curAltitude = mData.get<Sensors::Interfaces::Altitude>().readHeight();

        // Get adjustments @todo: Connect these to actual transmitter values
        AngleType roll = 0_deg;
        AngleType pitch = 0_deg;
        AngleType yaw = 0_deg;
        ThrottleType altitude = 0_m;

        // Update desired values of controller
        mController.setDesValue(altitude, roll, pitch, yaw);

        // Update controller
        mController.update(curAltitude, curRoll, curPitch, curYaw);
    }

} // namespace Copter::Control
