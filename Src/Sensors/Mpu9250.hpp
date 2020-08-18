#pragma once

#include <array>
#include <mbed.h>

namespace Copter::Sensors
{
    class MPU9250
    {
    public:
        struct Config
        {
            uint8_t address = (0x68 << 1);
            uint8_t gyroScale = 0x10;
            uint8_t accelScale = 0x10;
            uint8_t clkSelect = 0x01;
            uint8_t gyroTempDigiFilter = 0x00;
            uint8_t accelDigiFilter = 0x00;
            uint8_t gyroScaleRange = 0x10;
            uint8_t accelScaleRange = 0x10;
        };

        MPU9250(PinName sda, PinName scl);
        MPU9250(PinName sda, PinName scl, Config config);

        ~MPU9250() = default;

        bool setup();

        [[nodiscard]] std::array<int, 10> readSensor() const; // Order of data is: Accel, Gyro, Mag, Temp.
        [[nodiscard]] std::array<int, 3> readAccel() const;

        [[nodiscard]] std::array<int, 3> readGyro() const;

        [[nodiscard]] std::array<int, 3> readMag() const;

        [[nodiscard]] int readTemp() const;



    private:
        uint mI2CID;
        Config mConfig;

        enum class mFixedAddress : const char
        {
            PWR_MGMT_1 = 0x6B,
            CONFIG = 0x1A,
            SMPLRT_DIV = 0x19,
            GYRO_CONFIG = 0x1B,
            ACCEL_CONFIG_ONE = 0x1C,
            ACCEL_CONFIG_TWO = 0x1D,
            INT_PIN_CFG = 0x37,
            INT_ENABLE = 0x38,
            ACCEL_XOUT_H = 0x3B,
            TEMP_OUT_H = 0x41,
            GYRO_XOUT_H = 0x43,
            MAGADDR = (0x0C << 1), // mbed supports 8 bit address for i2c.
            MAG_CONTROL = 0x0A,
            MAG_ST1 = 0x02,
            MAG_XOUT_L = 0x03
        };



    };
}

