#pragma once
// Std libs
#include <array>
// External libs
#include <mbed.h>
// Project files
#include "SensorInterface.hpp"

namespace Copter::Sensors
{
    class MPU9250 : public SensorInterface
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

        /**
         * @brief Constructor.
         * @param sda: I2C sda pin.
         * @param scl: I2C scl pin.
         */
        MPU9250(PinName sda, PinName scl);

        /**
         * Constructor with configuration manually set.
         * @param sda: I2C sda pin.
         * @param scl: I2C scl pin.
         * @param config: Config for the sensor.
         */
        MPU9250(PinName sda, PinName scl, Config config);

        /**
         *  @brief Deconstructor.
         */
        ~MPU9250() override = default;

        /**
         * @brief: Initialise the hardware sensor.
         * @return bool: Whether initialisation was successful.
         */
        [[nodiscard]] bool init() const override;

        /**
         * @brief Read all sensors and returns an array of all data.
         * @return std::array<int, 10>: Array containing all sensor in order of Accel, Gyro, Mag, Temp.
         */
        [[nodiscard]] std::array<int, 10> readSensor() const; // Order of data is: Accel, Gyro, Mag, Temp.

        /**
         * @brief Read accelerometer.
         * @return std::array<int, 3>: Array of acceleration in (x,y,z).
         */
        [[nodiscard]] std::array<int, 3> readAccel() const override;

        /**
         * @brief Read gyroscope.
         * @return std::array<int, 3>: Array of acceleration in (x,y,z).
         */
        [[nodiscard]] std::array<int, 3> readGyro() const override;

        /**
         * @brief Read magnetometer.
         * @return std::array<int, 3>: Array of magnetism in (x,y,z).
         */
        [[nodiscard]] std::array<int, 3> readMag() const override;

        /**
         * @brief Read temperature.
         * @return int: Temperature in @todo: determine scaling.
         */
        [[nodiscard]] int readTemp() const override;

        /**
         * @brief: Returns an struct describing all the sensors in the chip. E.g: accerelation, gyroscope, etc.
         * @return SensorType: Struct describing sensor types of the chip.
         */
        [[nodiscard]] SensorType getType() const override;

    private:
        // Id for correct I2C class when using I2CInterface @todo: change change of I2CInterface
        uint mI2CID = 0;

        // Configuration of chip
        Config mConfig = Config{};

        // Fixed address for set address of the chip
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
} // namespace Copter::Sensors
