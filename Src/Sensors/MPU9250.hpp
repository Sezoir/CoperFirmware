#pragma once
// Std libs
#include <array>
// External libs
#include <mbed.h>
#include <units.h>
// Project files
#include "Interfaces/Sensor.hpp"
#include "Interfaces/Accelerometer.hpp"
#include "Interfaces/Gyroscope.hpp"
#include "Interfaces/Magnetometer.hpp"
#include "Interfaces/Thermometer.hpp"

namespace Copter::Sensors
{
    namespace acc = units::acceleration;
    namespace angVel = units::angular_velocity;
    namespace temp = units::temperature;
    namespace magStr = units::magnetic_field_strength;

    class MPU9250 : public Interfaces::Sensor,
                    public Interfaces::Accelerometer,
                    public Interfaces::Gyroscope,
                    public Interfaces::Magnetometer,
                    public Interfaces::Thermometer
    {
    public:
        struct Config
        {
            uint8_t address = (0x68 << 1);
            uint8_t gyroScale = 0x00;
            uint8_t accelScale = 0x00;
            uint8_t clkSelect = 0x01;
            uint8_t gyroTempDigiFilter = 0x00;
            uint8_t accelDigiFilter = 0x00;
            uint8_t gyroScaleRange = 0x10;
            uint8_t accelScaleRange = 0x10;
            uint8_t magScale = 0x10;
            uint8_t magMode = 0x06;
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
        ~MPU9250() = default;

        /**
         * @brief: Initialise the hardware sensor.
         * @return bool: Whether initialisation was successful.
         */
        [[nodiscard]] bool init() override;

        /**
         * @brief Read accelerometer.
         * @return std::array<units::acceleration::meters_per_second_squared_t, 3>: Array of acceleration in (x,y,z).
         */
        [[nodiscard]] std::array<acc::meters_per_second_squared_t, 3> readAccel() override;

        /**
         * @brief Read gyroscope.
         * @return std::array<units::angular_velocity::degrees_per_second_t, 3>: Array of acceleration in (x,y,z).
         */
        [[nodiscard]] std::array<angVel::degrees_per_second_t, 3> readGyro() override;

        /**
         * @brief Read magnetic strength.
         * @todo: Add calibration to result.
         * @return std::array<units::magnetic_field_strength::gauss_t, 3>: Array of magnetism in (x,y,z).
         */
        [[nodiscard]] std::array<magStr::microtesla_t, 3> readMag() override;

        /**
         * @brief Read temperature.
         * @return units::temperature::celsius_t: Temperature in celsius.
         */
        [[nodiscard]] temp::celsius_t readTemp() const override;

    private:
        /**
         * @brief Returns the scaling for the acceleration measurements based on mConfig.
         * @return float: Scaling to meters per second squared.
         */
        [[nodiscard]] float getAccScaling() const;

        /**
         * @brief: Returns the scaling for the gyroscope measurements based on mConfig.
         * @return float: Scaling to degrees per second.
         */
        [[nodiscard]] float getGyroScaling() const;

        /**
         * @brief Returns scaling for the magnetometer measurements based on mConfig.
         * @return float: Scaling to gauss.
         */
        [[nodiscard]] float getMagScaling() const;

        /**
         * @brief: Calibrates the bias for the accelerometer and gyroscope
         */
        void calAccelGyroBias() const;

        /**
         * @brief: Calibrate the bias for magnetometer.
         */
        void calMagBias();

        /**
         * @brief Returns the raw measurement of the magnetometer.
         * @return std::array<int16_t, 3>: Raw measurements (without modifications) from chip.
         */
        [[nodiscard]] std::array<int16_t, 3> readRawMag() const;

        // Id for correct I2C class when using I2CInterface @todo: change change of I2CInterface
        uint mI2CID = 0;

        // Configuration of chip
        Config mConfig = Config{};

        // Magnetometer bias
        std::array<float, 3> mMagBias = {0, 0, 0};

        // Factory calibrations
        std::array<float, 3> mMagFactCalibration = {0, 0, 0};

        // Magnetometer bias scaling
        std::array<float, 3> mMagBiaScaling = {0, 0, 0};

        // Magnetometer

        // The previous magnetic strength measurement
        std::array<magStr::microtesla_t, 3> mPreMagStr = {};

        // Fixed address for set address of the chip
        enum class mFixedAddress : const char
        {
            PWR_MGMT_1 = 0x6B,
            PWR_MGMT_2 = 0x6C,
            FIFO_EN = 0x23,
            I2C_MST_CTRL = 0x24,
            USER_CTRL = 0x6A,
            FIFO_COUNTH = 0x72,
            FIFO_R_W = 0x74,
            CONFIG = 0x1A,
            SMPLRT_DIV = 0x19,
            GYRO_CONFIG = 0x1B,
            ACCEL_CONFIG_ONE = 0x1C,
            ACCEL_CONFIG_TWO = 0x1D,
            INT_PIN_CFG = 0x37,
            INT_ENABLE = 0x38,
            ACCEL_XOUT_H = 0x3B,
            XA_OFFSET_H = 0x77,
            XA_OFFSET_L = 0x78,
            YA_OFFSET_H = 0x7A,
            YA_OFFSET_L = 0x7B,
            ZA_OFFSET_H = 0x7D,
            ZA_OFFSET_L = 0x7E,
            TEMP_OUT_H = 0x41,
            GYRO_XOUT_H = 0x43,
            XG_OFFSET_H = 0x13,
            XG_OFFSET_L = 0x14,
            YG_OFFSET_H = 0x15,
            YG_OFFSET_L = 0x16,
            ZG_OFFSET_H = 0x17,
            ZG_OFFSET_L = 0x18,
            MAGADDR = (0x0C << 1), // mbed supports 8 bit address for i2c.
            MAG_CONTROL = 0x0A,
            MAG_ST1 = 0x02,
            MAG_XOUT_L = 0x03,
            AK8963_ASAX = 0x10
        };
    };
} // namespace Copter::Sensors
