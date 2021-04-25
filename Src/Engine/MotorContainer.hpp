#pragma once
// Std libs
#include <array>
#include <memory>
#include <chrono>
// External libs
#include <mbed.h>
// Project files
#include "Motor.hpp"
#include "ProtocolInterface.hpp"

namespace Copter::Engine
{
    template <typename PROTOCOL, uint32_t COUNT>
    class MotorContainer
    {
    public:
        /**
         * @brief Default constructor.
         */
        MotorContainer() = default;

        /**
         * @brief Default deconstructor.
         */
        ~MotorContainer() = default;

        /**
         * @brief Initialises all the protocols, and then calls the setup function for all protocols and motors.
         * @tparam ARGS: Argument types for the protocol.
         * @param pins: Array of all pins for each motor.
         * @param ramp: The motor profile to use for all motors.
         * @param delay: The delay between motor update calls.
         * @param protocolArgs: The arguments for the protocol.
         */
        template <typename... ARGS>
        void setup(std::array<PinName, COUNT> pins, Motor::Profile ramp,
                   std::chrono::duration<int64_t, std::milli>&& delay, ARGS... protocolArgs)
        {
            // Check array size equal pin size
            if(pins.size() != COUNT)
                MBED_ERROR(MBED_MAKE_ERROR(MBED_MODULE_APPLICATION, MBED_ERROR_CODE_INVALID_SIZE),
                           "MotorContainer::setup : Container size does not equal the size of the pins container.");

            // For every MotorSetup setup the protocol and motor
            for(std::size_t i = 0; i < COUNT; i++)
            {
                // Initialise the protocol variables
                mContainer[i].protocol.init(pins[i], protocolArgs...);

                // Setup the protocol
                mContainer[i].protocol.setup();

                // Setup protocol
                mContainer[i].motor.init(mContainer[i].protocol, ramp, delay);
            }
        }

        /**
         * @brief Calls the update function of each motor in the container.
         */
        void update()
        {
            for(MotorSetup& setup : mContainer)
            {
                setup.motor.update();
            }
        }

        /**
         * @brief Calls the setSpeed for a specific motor.
         * @param id: Id of the motor to change speed.
         * @param speed: New speed to set the motor to.
         */
        void setSpeed(uint8_t id, units::protocol::speed_t speed)
        {
            // Check id is valid
            if(id >= COUNT || id < 0)
                MBED_ERROR(MBED_MAKE_ERROR(MBED_MODULE_APPLICATION, MBED_ERROR_CODE_INVALID_ARGUMENT),
                           "MotorContainer::setSpeed : Invalid id for array.");

            // Set motor speed
            mContainer[id].motor.setSpeed(speed);
        }

        units::protocol::speed_t getCurrentSpeed(uint8_t id) const
        {
            return mContainer[id].motor.getCurrentSpeed();
        }

    private:
        // Setup for each motor
        struct MotorSetup
        {
            Motor motor;
            PROTOCOL protocol;
        };

        // Contain all motor setups
        std::array<MotorSetup, COUNT> mContainer = {};
    };
} // namespace Copter::Engine
