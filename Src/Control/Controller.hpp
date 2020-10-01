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
        void update(units::protocol::speed_t throttle, units::angle::degree_t roll, units::angle::degree_t pitch,
                    units::angle::degree_t yaw);

        /**
         * @brief: Update the desired values for the pid controllers.
         * @param newRoll: New roll angle.
         * @param newPitch: New pitch angle.
         * @param newYaw: New yaw angle.
         */
        void setDesValue(units::angle::degree_t newRoll, units::angle::degree_t newPitch,
                         units::angle::degree_t newYaw);

    private:
        // Motor container
        Engine::MotorContainer<Engine::MOTOR_PROTOCOL, MOTOR_COUNT>& mMotors;

        // PID Controllers
        PIDControl<units::angle::degree_t> mRollPID = {ROLL_PID_P, ROLL_PID_I, ROLL_PID_D};
        PIDControl<units::angle::degree_t> mPitchPID = {PITCH_PID_P, PITCH_PID_I, PITCH_PID_D};
        PIDControl<units::angle::degree_t> mYawPID = {YAW_PID_P, YAW_PID_I, YAW_PID_D};
    };
} // namespace Copter::Control
