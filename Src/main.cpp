#include "mbed.h"
#include "Engine/DShot.hpp"
#include "Engine/Motor.hpp"
#include "Sensors/MPU9250.hpp"
#include "Engine/Interface.hpp"

#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"
static BufferedSerial serial(USBTX, USBRX, 115200);

Ticker updater;

int main()
{
    printf("Project started\n");

    Copter::Engine::DShot proto(PB_8, 1200);
    proto.setup();

    ThisThread::sleep_for(100ms);

    Copter::Engine::Motor motor;
    motor.init(proto, Copter::Engine::Motor::Profile::SlowRamp, 1ms);
    ThisThread::sleep_for(100ms);

    //    Copter::Engine::Motor motorInited(proto, Copter::Engine::Motor::Profile::SlowRamp, 1ms);

    //    proto.setup();

    //    Copter::Engine::Motor motor(Copter::Engine::DShot(PB_8, 1200), Copter::Engine::Motor::Profile::SlowRamp, 1ms);

    //    Copter::Engine::Interface motors;

    //    Copter::Sensors::MPU9250 sensor(PD_13, PD_12);
    //    sensor.setup();

    char buf[5] = {0};
    updater.attach(callback(&motor, &Copter::Engine::Motor::update), 1ms);
    //    updater.attach(callback(&motors, &Copter::Engine::Interface::update), 1ms);

    std::array<int, 3> accel = {};

    while(true)
    {
        if(serial.readable())
        {
            // Read data
            serial.read(buf, sizeof(buf));

            // Convert data
            uint16_t num;
            sscanf(buf, "%ul", &num);

            // Clear buffer
            memset(buf, 0, sizeof(buf));

            // Cast to speed
            units::velocity::speed_t speed(num);

            // Set the new speed
            printf("calling set speed with %d\n", speed.to<uint16_t>());
            motor.setSpeed(speed);
        }

        motor.update();

        //        accel = sensor.readGyro();
        //        printf("X: %d, Y: %d, Z: %d\n", accel[0], accel[1], accel[2]);
        ThisThread::sleep_for(1ms);
    }
}
#pragma clang diagnostic pop