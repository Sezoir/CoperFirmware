#include <mbed.h>
#include "Config.hpp"
#include "Engine/DShot.hpp"
#include "Engine/Motor.hpp"
#include "Engine/MotorContainer.hpp"

#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"

Ticker updater;
static BufferedSerial serial(USBTX, USBRX, 115200);

using namespace units::literals;

int main()
{

    printf("Project started\n");

    // Create motors
    Copter::Engine::MotorContainer<Copter::Engine::MOTOR_PROTOCOL, MOTOR_COUNT> motors;
    // Setup motors
    motors.setup({MOTOR_PINS}, Copter::Engine::Motor::Profile::MOTOR_PROFILE, MOTOR_DELAY, MOTOR_PROTOCOL_PARAMETERS);
    updater.attach(
        callback(&motors, &Copter::Engine::MotorContainer<Copter::Engine::MOTOR_PROTOCOL, MOTOR_COUNT>::update), 1ms);

    // Create variables
    char buf[5] = {0};

    while(true)
    {
        if(serial.readable())
        {
            // Read data
            serial.read(buf, sizeof(buf));

            // Convert data
            int num;
            sscanf(buf, "%ul", &num);

            // Clear buffer
            memset(buf, 0, sizeof(buf));

            // Cast to speed
            units::protocol::speed_t speed(num);

            // Set the new speed
            printf("calling set speed with %d\n", speed.to<uint16_t>());
            motors.setSpeed(0, speed);
            motors.setSpeed(1, speed);
        }

        motors.update();

        //        ThisThread::sleep_for(10ms);
    }
}
#pragma clang diagnostic pop