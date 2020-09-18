#include "mbed.h"

#include "Config.hpp"
#include "Engine/DShot.hpp"
#include "Engine/Motor.hpp"
#include "Sensors/MPU9250.hpp"
#include "Engine/MotorContainer.hpp"
#include "Sensors/SensorRepository.hpp"

#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"
static BufferedSerial serial(USBTX, USBRX, 115200);

Ticker updater;

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

    // Create sensor repo
    Copter::Sensors::SensorRepository repo;
    repo.build();

    // Create variables
    char buf[5] = {0};
    //    std::array<units::acceleration::meters_per_second_squared_t, 3> accel = {0_mps_sq, 0_mps_sq, 0_mps_sq};
    //    std::array<units::angular_velocity::degrees_per_second_t, 3> accel = {0_deg_per_s, 0_deg_per_s, 0_deg_per_s};
    //    std::array<units::magnetic_field_strength::gauss_t, 3> accel = {0_G, 0_G, 0_G};

    units::angle::degree_t mRoll = 0_deg;
    units::angle::degree_t mPitch = 0_deg;
    units::angle::degree_t mYaw = 0_deg;

    //    auto prevTime = units::time::millisecond_t(Kernel::Clock::now().time_since_epoch().count());

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

        // Calculate difference in time between calls
        //        auto curTime = units::time::millisecond_t(Kernel::Clock::now().time_since_epoch().count());
        //        auto diffTime = curTime - prevTime;
        //        prevTime = curTime;
        //        printf("%u\n", diffTime.to<uint16_t>());

        repo.get<Copter::Sensors::Interfaces::Angle>().update();
        mRoll = repo.get<Copter::Sensors::Interfaces::Angle>().getRoll();
        mPitch = repo.get<Copter::Sensors::Interfaces::Angle>().getPitch();
        mYaw = repo.get<Copter::Sensors::Interfaces::Angle>().getYaw();

        int roll = mRoll.to<int>();
        int pitch = mPitch.to<int>();
        int yaw = mYaw.to<int>();

        printf("%d, %d, %d\n", roll, pitch, yaw);

        //        auto accel = repo.get<Copter::Sensors::AccelVoter>().readAccel();
        //        std::array<int, 3> temp = {0, 0, 0};
        //        for(int i = 0; i < 3; i++)
        //        {
        //            temp[i] = static_cast<int>(accel[i] * 100);
        //        }
        //        printf("X: %d, Y: %d, Z: %d\n", temp[0], temp[1], temp[2]);
        //        accel = {0_mps_sq, 0_mps_sq, 0_mps_sq};
        //        accel = {0_deg_per_s, 0_deg_per_s, 0_deg_per_s};
        //        accel = {0_G, 0_G, 0_G};

        ThisThread::sleep_for(250ms);
    }
}
#pragma clang diagnostic pop