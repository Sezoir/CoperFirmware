#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"

#include <mbed.h>

#include "Config.hpp"
#include "Engine/MotorContainer.hpp"
#include "Sensors/SensorRepository.hpp"
#include "Control/SelfLevelControl.hpp"

static bool isTestCurrentlyRunning = false;
static Ticker countDown;

void stopTest()
{
    isTestCurrentlyRunning = false;
    countDown.detach();
}

int main()
{
    printf("Project started\n");

    Ticker updater;

    // Create sensor repo
    Copter::Sensors::SensorRepository repo;
    repo.build();

    // Create motors
    Copter::Engine::MotorContainer<Copter::Engine::MOTOR_PROTOCOL, MOTOR_COUNT> motors;
    // Setup motors
    motors.setup({MOTOR_PINS}, Copter::Engine::Motor::Profile::MOTOR_PROFILE, MOTOR_DELAY, MOTOR_PROTOCOL_PARAMETERS);
    updater.attach(
        callback(&motors, &Copter::Engine::MotorContainer<Copter::Engine::MOTOR_PROTOCOL, MOTOR_COUNT>::update), 1ms);

    Copter::Control::SelfLevelControl control(motors, repo);

    control.init();

    std::string input;
    while(true)
    {
        scanf("%s", input.data());
        if(input == "start")
        {
            isTestCurrentlyRunning = true;
            countDown.attach(stopTest, 3s);
        }
        else if(input == "stop")
        {
            // First focus on stopping motors
            motors.setSpeed(0, 0_sd);
            motors.setSpeed(1, 0_sd);
            motors.setSpeed(2, 0_sd);
            motors.setSpeed(3, 0_sd);

            bool motorsStopped = false;
            while(!motorsStopped)
            {
                motors.update();

                if(motors.getCurrentSpeed(0) == 0_sd && motors.getCurrentSpeed(1) == 0_sd &&
                   motors.getCurrentSpeed(2) == 0_sd && motors.getCurrentSpeed(3) == 0_sd)
                    motorsStopped = true;
            }
            isTestCurrentlyRunning = false;
        }

        if(isTestCurrentlyRunning)
            control.update();

        input.clear();
    }

    return 0;
}

#pragma clang diagnostic pop