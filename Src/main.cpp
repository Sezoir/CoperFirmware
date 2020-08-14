#include "mbed.h"
#include "Engine/DShot.hpp"
#include "Engine/Motor.hpp"

static BufferedSerial serial(USBTX, USBRX, 115200);

Ticker updater;

int main()
{
    printf("Project started\n");

    Copter::Engine::DShot proto(1200, Copter::Engine::DShot::Pin::PB8);
    Copter::Engine::Motor motor(proto, Copter::Engine::Motor::Profile::FastRamp, 50us);
    proto.setup();

    char buf[5] = {0};
    updater.attach(callback(&motor, &Copter::Engine::Motor::update), 50us);

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


        ThisThread::sleep_for(250ms);
    }
}