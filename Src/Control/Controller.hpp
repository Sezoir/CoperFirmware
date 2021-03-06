#pragma once
// External libs
#include <units.h>
// Project files
#include "../Engine/MotorContainer.hpp"
#include "../Engine/ProtocolInterface.hpp"
#include "PIDControl.hpp"
#include "../Config.hpp"

namespace Copter::Control
{
    template <typename ThrottleType, typename AngleType>
    class Controller
    {
    public:
        /**
         * @brief Controller constructor.
         * @param motorContainer: Reference to the motor container.
         */
        explicit Controller(Engine::MotorContainer<Engine::MOTOR_PROTOCOL, MOTOR_COUNT>& motorContainer)
            : mMotors(motorContainer)
        {
        }

        /**
         * @brief Update the pid and motor based on new angles/throttle
         * @param throttle: Base motor speed to lift quadcopter.
         * @param roll: New roll angle.
         * @param pitch: New pitch angle.
         * @param yaw
         */
        void update(ThrottleType throttle, AngleType roll, AngleType pitch, AngleType yaw)
        {
            // Get new pid adjustments
            auto throttlePID = mThrottle.update(throttle);
            auto rollPID = mRollPID.update(roll);
            auto pitchPID = mPitchPID.update(pitch);
            auto yawPID = mYawPID.update(yaw);

            // Get new motor values
            // Front left
            units::protocol::speed_t motorOne = units::protocol::speed_t(
                units::unit_cast<double>(throttlePID) + units::unit_cast<double>(-rollPID - pitchPID + yawPID));
            // Front right
            units::protocol::speed_t motorTwo = units::protocol::speed_t(
                units::unit_cast<double>(throttlePID) + units::unit_cast<double>(rollPID - pitchPID - yawPID));
            // Rear left
            units::protocol::speed_t motorThree = units::protocol::speed_t(
                units::unit_cast<double>(throttlePID) + units::unit_cast<double>(-rollPID + pitchPID - yawPID));
            // Rear right
            units::protocol::speed_t motorFour = units::protocol::speed_t(
                units::unit_cast<double>(throttlePID) + units::unit_cast<double>(rollPID + pitchPID + yawPID));

            // Update motors
            mMotors.setSpeed(0, motorOne);
            mMotors.setSpeed(1, motorTwo);
            mMotors.setSpeed(2, motorThree);
            mMotors.setSpeed(3, motorFour);
        }

        /**
         * @brief: Update the desired values for the pid controllers.
         * @param newRoll: New roll angle.
         * @param newPitch: New pitch angle.
         * @param newYaw: New yaw angle.
         */
        void setDesValue(ThrottleType throttle, AngleType newRoll, AngleType newPitch, AngleType newYaw)
        {
            // Update pid desired values
            mThrottle.setDesValue(throttle);
            mRollPID.setDesValue(newRoll);
            mPitchPID.setDesValue(newPitch);
            mYawPID.setDesValue(newYaw);
        }

    private:
        // Motor container
        Engine::MotorContainer<Engine::MOTOR_PROTOCOL, MOTOR_COUNT>& mMotors;

        // PID Controllers
        PIDControl<AngleType> mRollPID = {ROLL_PID_P, ROLL_PID_I, ROLL_PID_D};
        PIDControl<AngleType> mPitchPID = {PITCH_PID_P, PITCH_PID_I, PITCH_PID_D};
        PIDControl<AngleType> mYawPID = {YAW_PID_P, YAW_PID_I, YAW_PID_D};
        PIDControl<ThrottleType> mThrottle = {THROTTLE_PID_P, THROTTLE_PID_I, THROTTLE_PID_D};
    };
} // namespace Copter::Control
