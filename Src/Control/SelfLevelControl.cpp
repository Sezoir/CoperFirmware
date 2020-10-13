#include "SelfLevelControl.hpp"

namespace Copter::Control
{
    SelfLevelControl::SelfLevelControl(Engine::MotorContainer<Engine::MOTOR_PROTOCOL, MOTOR_COUNT>& motorContainer,
                                       Sensors::SensorRepository& data)
        : ControlScheme<units::angle::degree_t>(motorContainer, data)
    {
    }

    void SelfLevelControl::init()
    {
    }

    void SelfLevelControl::update()
    {
        // Update angles (note this should also update the accelerometer reading so we don't explicitly update it)
        mData.get<Sensors::Interfaces::Angle>().update();

        // Get current angles and z-acceleration
        units::angle::degree_t curRoll = mData.get<Sensors::Interfaces::Angle>().getRoll();
        units::angle::degree_t curPitch = mData.get<Sensors::Interfaces::Angle>().getPitch();
        units::angle::degree_t curYaw = mData.get<Sensors::Interfaces::Angle>().getYaw();
        units::acceleration::meters_per_second_squared_t curAccelZ =
            mData.get<Sensors::Interfaces::Accelerometer>().readAccel()[2];

        // Get adjustments @todo: Connect these to actual transmitter values
        units::angle::degree_t roll = 0_deg;
        units::angle::degree_t pitch = 0_deg;
        units::angle::degree_t yaw = 0_deg;
        units::acceleration::meters_per_second_squared_t throttle = 0_mps_sq;

        // Update desired values of controller
        mController.setDesValue(throttle, roll, pitch, yaw);

        // Update controller
        mController.update(curAccelZ, curRoll, curPitch, curYaw);
    }

} // namespace Copter::Control
