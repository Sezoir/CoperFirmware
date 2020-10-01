#include <array>

#include <mbed.h>

#include "Sensors/MPU9250.hpp"
#include "Sensors/SensorRepository.hpp"

#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"

using namespace units::literals;

int main()
{
    printf("Project started\n");

    // Create sensor repo
    Copter::Sensors::SensorRepository repo;
    repo.build();

    std::array<units::acceleration::meters_per_second_squared_t, 3> accel = {0_mps_sq, 0_mps_sq, 0_mps_sq};
    std::array<units::angular_velocity::degrees_per_second_t, 3> gyro = {0_deg_per_s, 0_deg_per_s, 0_deg_per_s};
    std::array<units::magnetic_field_strength::microtesla_t, 3> mag = {0_uTe, 0_uTe, 0_uTe};

    units::angle::degree_t mRoll = 0_deg;
    units::angle::degree_t mPitch = 0_deg;
    units::angle::degree_t mYaw = 0_deg;

    while(true)
    {
        repo.get<Copter::Sensors::Interfaces::Angle>().update();
        mRoll = repo.get<Copter::Sensors::Interfaces::Angle>().getRoll();
        mPitch = repo.get<Copter::Sensors::Interfaces::Angle>().getPitch();
        mYaw = repo.get<Copter::Sensors::Interfaces::Angle>().getYaw();

        int roll = mRoll.to<int>();
        int pitch = mPitch.to<int>();
        int yaw = mYaw.to<int>();

        printf("Angles: [Roll: %d, Pitch: %d, Yaw: %d]\n", roll, pitch, yaw);

        accel = repo.get<Copter::Sensors::AccelVoter>().readAccel();
        gyro = repo.get<Copter::Sensors::GyroVoter>().readGyro();
        mag = repo.get<Copter::Sensors::MagVoter>().readMag();
        std::array<int, 9> temp = {0, 0, 0};
        for(int i = 0; i < 3; i++)
        {
            temp[i] = static_cast<int>(accel[i] * 100);
            temp[i + 3] = static_cast<int>(gyro[i] * 100);
            temp[i + 6] = static_cast<int>(mag[i] * 100);
        }
        printf("Accel: [X: %d, Y: %d, Z: %d]\n", temp[0], temp[1], temp[2]);
        printf("Gyro: [X: %d, Y: %d, Z: %d]\n", temp[3], temp[4], temp[5]);
        printf("Accel: [X: %d, Y: %d, Z: %d]\n", temp[6], temp[7], temp[8]);

        accel = {0_mps_sq, 0_mps_sq, 0_mps_sq};
        gyro = {0_deg_per_s, 0_deg_per_s, 0_deg_per_s};
        mag = {0_uTe, 0_uTe, 0_uTe};

        ThisThread::sleep_for(10ms);
    }
}