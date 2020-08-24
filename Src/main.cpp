#include "mbed.h"
#include "Engine/DShot.hpp"
#include "Engine/Motor.hpp"
#include "Sensors/MPU9250.hpp"

static BufferedSerial serial(USBTX, USBRX, 115200);

Ticker updater;

int main()
{
    printf("Project started\n");

    Copter::Engine::DShot proto(PB_8, 1200);
    Copter::Engine::Motor motor(proto, Copter::Engine::Motor::Profile::SlowRamp, 1ms);
    proto.setup();

    Copter::Sensors::MPU9250 sensor(PD_13, PD_12);
    sensor.setup();

    char buf[5] = {0};
    updater.attach(callback(&motor, &Copter::Engine::Motor::update), 1ms);

    std::array<int, 3> accel = {};

    while(true)
    {
        if(serial.readable())
        {
            serial.read(buf, sizeof(buf));
            int num;
            sscanf(buf, "%d", &num);
            memset(buf, 0, sizeof(buf));
            units::velocity::speed_t speed(num);
            printf("%d\n", speed.to<uint16_t>());
            motor.setSpeed(speed);
        }

        accel = sensor.readGyro();
        printf("X: %d, Y: %d, Z: %d\n", accel[0], accel[1], accel[2]);
        ThisThread::sleep_for(250ms);
    }
}