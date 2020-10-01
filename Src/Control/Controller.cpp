#include "Controller.hpp"

namespace Copter::Control
{

    void Controller::update(units::protocol::speed_t throttle, units::angle::degree_t roll,
                            units::angle::degree_t pitch, units::angle::degree_t yaw)
    {
        // Get new pid adjustments
        auto rollPID = mRollPID.update(roll);
        auto pitchPID = mPitchPID.update(pitch);
        auto yawPID = mYawPID.update(yaw);

        // Get new motor values
        // Front left
        units::protocol::speed_t motorOne =
            throttle + units::protocol::speed_t(-rollPID.to<double>() - pitchPID.to<double>() + yawPID.to<double>());
        // Front right
        units::protocol::speed_t motorTwo =
            throttle + units::protocol::speed_t(rollPID.to<double>() - pitchPID.to<double>() - yawPID.to<double>());
        // Rear left
        units::protocol::speed_t motorThree =
            throttle + units::protocol::speed_t(-rollPID.to<double>() + pitchPID.to<double>() - yawPID.to<double>());
        // Rear right
        units::protocol::speed_t motorFour =
            throttle + units::protocol::speed_t(rollPID.to<double>() + pitchPID.to<double>() + yawPID.to<double>());

        // Update motors
        mMotors.setSpeed(0, motorOne);
        mMotors.setSpeed(1, motorTwo);
        mMotors.setSpeed(2, motorThree);
        mMotors.setSpeed(3, motorFour);
    }
    void Controller::setDesValue(units::angle::degree_t newRoll, units::angle::degree_t newPitch,
                                 units::angle::degree_t newYaw)
    {
        // Update pid desired values
        mRollPID.setDesValue(newRoll);
        mPitchPID.setDesValue(newPitch);
        mYawPID.setDesValue(newYaw);
    }
} // namespace Copter::Control
