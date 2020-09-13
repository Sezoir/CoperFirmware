#include "mbed.h"

#include "Config.hpp"
#include "Engine/DShot.hpp"
#include "Engine/Motor.hpp"
#include "Sensors/MPU9250.hpp"
#include "Engine/MotorContainer.hpp"
#include "Sensors/Repository.hpp"

#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"
static BufferedSerial serial(USBTX, USBRX, 115200);

Ticker updater;

using namespace units::literals;

int main()
{
    printf("Project started\n");

    //    Copter::Engine::DShot proto(PB_8, 1200);
    //    proto.setup();
    //
    //    Copter::Engine::Motor motor;
    //    motor.init(proto, Copter::Engine::Motor::Profile::SlowRamp, 1ms);

    Copter::Engine::MotorContainer<Copter::Engine::MOTOR_PROTOCOL, MOTOR_COUNT> motors;

    motors.setup({MOTOR_PINS}, Copter::Engine::Motor::Profile::MOTOR_PROFILE, MOTOR_DELAY, MOTOR_PROTOCOL_PARAMETERS);

    //    Copter::Sensors::MPU9250 sensor(PD_13, PD_12);
    //    sensor.setup();

    char buf[5] = {0};
    //    updater.attach(callback(&motor, &Copter::Engine::Motor::update), 1ms);

    updater.attach(
        callback(&motors, &Copter::Engine::MotorContainer<Copter::Engine::MOTOR_PROTOCOL, MOTOR_COUNT>::update), 1ms);

    //    std::array<units::acceleration::meters_per_second_squared_t, 3> accel = {0_mps_sq, 0_mps_sq, 0_mps_sq};
    //    std::array<units::angular_velocity::degrees_per_second_t, 3> accel = {0_deg_per_s, 0_deg_per_s, 0_deg_per_s};
    std::array<units::magnetic_field_strength::gauss_t, 3> accel = {0_G, 0_G, 0_G};

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

        //        motors.update();
        //        proto.sendSignal(0_sd);

        for(auto& i : accel)
            i = (i / 5) * 100;
        //        accel = sensor.readGyro();

        std::array<int, 3> temp = {0, 0, 0};
        for(int i = 0; i < 3; i++)
        {
            temp[i] = static_cast<int>(accel[i]);
        }
        printf("X: %d, Y: %d, Z: %d\n", temp[0], temp[1], temp[2]);
        //        accel = {0_mps_sq, 0_mps_sq, 0_mps_sq};
        //        accel = {0_deg_per_s, 0_deg_per_s, 0_deg_per_s};
        accel = {0_G, 0_G, 0_G};

        ThisThread::sleep_for(250ms);
    }
}
#pragma clang diagnostic pop