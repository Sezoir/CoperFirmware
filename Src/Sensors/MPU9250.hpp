#pragma once
// Std libs
#include <array>
// External libs
#include <mbed.h>
#include <units.h>
// Project files
#include "SensorInterface.hpp"

namespace Copter::Sensors
{
    namespace acc = units::acceleration;
    namespace angVel = units::angular_velocity;
    namespace temp = units::temperature;
    namespace magStr = units::magnetic_field_strength;

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
            uint8_t magScale = 0x10;
            uint8_t magMode = 0x6;
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
         * @brief Read accelerometer.
         * @return std::array<units::acceleration::meters_per_second_squared_t, 3>: Array of acceleration in (x,y,z).
         */
        [[nodiscard]] std::array<acc::meters_per_second_squared_t, 3> readAccel() const override;

        /**
         * @brief Read gyroscope.
         * @return std::array<units::angular_velocity::degrees_per_second_t, 3>: Array of acceleration in (x,y,z).
         */
        [[nodiscard]] std::array<angVel::degrees_per_second_t, 3> readGyro() const override;

        /**
         * @brief Read magnetic strength.
         * @todo: Add calibration to result.
         * @return std::array<units::magnetic_field_strength::gauss_t, 3>: Array of magnetism in (x,y,z).
         */
        [[nodiscard]] std::array<magStr::gauss_t, 3> readMag() const override;

        /**
         * @brief Read temperature.
         * @return units::temperature::celsius_t: Temperature in celsius.
         */
        [[nodiscard]] temp::celsius_t readTemp() const override;

        /**
         * @brief: Returns an struct describing all the sensors in the chip. E.g: accerelation, gyroscope, etc.
         * @return SensorType: Struct describing sensor types of the chip.
         */
        [[nodiscard]] SensorType getType() const override;

    private:
        /**
         * @brief Returns the scaling for the acceleration measurements based on mConfig.
         * @return float: Scaling to meters per second squared.
         */
        [[nodiscard]] constexpr float getAccScaling() const;

        /**
         * @brief: Returns the scaling for the gyroscope measurements based on mConfig.
         * @return float: Scaling to degrees per second.
         */
        [[nodiscard]] constexpr float getGyroScaling() const;

        /**
         * @brief Returns scaling for the magnetometer measurements based on mConfig.
         * @return float: Scaling to gauss.
         */
        [[nodiscard]] constexpr float getMagScaling() const;

        // Id for correct I2C class when using I2CInterface @todo: change change of I2CInterface
        uint mI2CID = 0;

        // Configuration of chip
        Config mConfig = Config{};

        // The previous magnetic strength measurement
        std::array<magStr::gauss_t, 3> mPreMagStr = {};

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
