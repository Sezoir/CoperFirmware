#pragma once
// Std libs
#include <array>
// External libs
#include <units.h>
// Project files
#include "../Config.hpp"
#include "../Sensors/SensorRepository.hpp"
#include "../Engine/MotorContainer.hpp"
#include "ControlScheme.hpp"

namespace Copter::Control
{
    using namespace units::literals;

    /**
     * @brief A control scheme for a self leveling quadcopter. In this case we are controlling the angles of the copter.
     */
    class SelfLevelControl : public ControlScheme<units::angle::degree_t>
    {
    public:
        /**
         * @brief Constructor
         * @param motorContainer: Reference to the container of motors.
         * @param data: Reference to the sensor repository.
         */
        SelfLevelControl(Engine::MotorContainer<Engine::MOTOR_PROTOCOL, MOTOR_COUNT>& motorContainer,
                         Sensors::SensorRepository& data);

        /**
         * @brief Initialises the class. Specifically finds the minimum throttle need on all motors to counteract
         * gravity.
         */
        void init() override;

        /**
         * @brief Finds the current roll, pitch and yaw angles, as well as retrieves the desired angles and updates the
         * controller with these values.
         */
        void update() override;

    private:
    };
} // namespace Copter::Control
