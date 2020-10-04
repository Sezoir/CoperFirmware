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
        // Initialise until we have calibrated our throttle
        while(!calibrateThrottle())
        {
        }
    }

    bool SelfLevelControl::calibrateThrottle()
    {
        // Get acceleration
        std::array<units::acceleration::meters_per_second_squared_t, 3> accel =
            mData.get<Sensors::Interfaces::Accelerometer>().readAccel();

        // Check whether throttle counteracts gravity
        if(accel[2] <= 0_mps_sq)
        {
            // Found throttle
            return true;
        }
        // Else increase throttle
        mThrottle += 1_sd;
        mController.update(mThrottle, 0_deg, 0_deg, 0_deg);

        return false;
    }

    void SelfLevelControl::update()
    {
        // Update angles
        mData.get<Sensors::Interfaces::Angle>().update();

        // Get current angles
        units::angle::degree_t curRoll = mData.get<Sensors::Interfaces::Angle>().getRoll();
        units::angle::degree_t curPitch = mData.get<Sensors::Interfaces::Angle>().getPitch();
        units::angle::degree_t curYaw = mData.get<Sensors::Interfaces::Angle>().getYaw();

        // Get adjustments @todo: Connect these to actual transmitter values
        units::angle::degree_t roll = 0_deg;
        units::angle::degree_t pitch = 0_deg;
        units::angle::degree_t yaw = 0_deg;
        units::protocol::speed_t throttle = 0_sd;

        // Update desired values of controller
        mController.setDesValue(roll, pitch, yaw);

        // Update controller
        mController.update(mThrottle + throttle, curRoll, curPitch, curYaw);
    }

} // namespace Copter::Control
