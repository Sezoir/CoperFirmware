#include "MPU9250.hpp"
#include "../Drivers/I2CInterface.hpp"

namespace Copter::Sensors
{
    using I2CInterface = Copter::Drivers::I2CInterface;

    MPU9250::MPU9250(PinName sda, PinName scl)
        : mI2CID(Copter::Drivers::I2CInterface::getInstance(sda, scl))
    {
    }

    MPU9250::MPU9250(PinName sda, PinName scl, Config config)
        : mI2CID(Copter::Drivers::I2CInterface::getInstance(sda, scl))
        , mConfig(config)
    {
    }

    bool MPU9250::init() const
    {
        uint8_t config = 0;

        // MPU9250 starts of in sleep mode, so need to clear address.
        I2CInterface::writeByte(mI2CID, mConfig.address, static_cast<char>(mFixedAddress::PWR_MGMT_1),
                                0); // Wake up MPU

        // Set clock source to be Phase Locked Looped
        I2CInterface::writeByte(mI2CID, mConfig.address, static_cast<char>(mFixedAddress::PWR_MGMT_1), 0x01);

        // Set up the Digital Low Pass Filter
        I2CInterface::writeByte(mI2CID, mConfig.address, static_cast<char>(mFixedAddress::CONFIG),
                                mConfig.gyroTempDigiFilter);
        I2CInterface::writeByte(mI2CID, mConfig.address, static_cast<char>(mFixedAddress::ACCEL_CONFIG_TWO),
                                mConfig.accelDigiFilter);

        // Set sample rate of gyro.
        // Note: sample rate = Gyroscope sample rate/(1+SMPLRT_DIV)
        // Also that gyroscope sample rate = 1000Hz.
        // Note that the Fchoice register must = 0b11, i.e not bypass the digital low pass filter.
        I2CInterface::writeByte(mI2CID, mConfig.address, static_cast<char>(mFixedAddress::SMPLRT_DIV),
                                0x04); // 200Hz

        // Set gyro scale range.
        I2CInterface::writeByte(mI2CID, mConfig.address, static_cast<char>(mFixedAddress::GYRO_CONFIG),
                                mConfig.gyroScale);

        // Set Accelerometer scale range.
        I2CInterface::writeByte(mI2CID, mConfig.address, static_cast<char>(mFixedAddress::ACCEL_CONFIG_ONE),
                                mConfig.accelScale);

        // Setup Interrupts and Bypass mode
        I2CInterface::writeByte(mI2CID, mConfig.address, static_cast<char>(mFixedAddress::INT_PIN_CFG), 0x22);
        I2CInterface::writeByte(mI2CID, mConfig.address, static_cast<char>(mFixedAddress::INT_ENABLE), 0x01);
        ThisThread::sleep_for(1ms);

        // Setup AK8963 (magnetometer)
        I2CInterface::writeByte(mI2CID, static_cast<char>(mFixedAddress::MAGADDR),
                                static_cast<char>(mFixedAddress::MAG_CONTROL), 0); // Wake up mag
        ThisThread::sleep_for(1ms);
        config = mConfig.magScale | mConfig.magMode;
        I2CInterface::writeByte(mI2CID, static_cast<char>(mFixedAddress::MAGADDR),
                                static_cast<char>(mFixedAddress::MAG_CONTROL), config);
        ThisThread::sleep_for(1ms);
        return true;
    }

    std::array<acc::meters_per_second_squared_t, 3> MPU9250::readAccel() const
    {
        std::array<acc::meters_per_second_squared_t, 3> returnData = {};
        std::array<uint8_t, 6> rawData = {};
        const float scaling = getAccScaling();
        rawData = I2CInterface::readBytes<6>(mI2CID, mConfig.address, static_cast<char>(mFixedAddress::ACCEL_XOUT_H));
        for(int i = 0; i < 3; i++)
        {
            // Bitshift uint8_t variables to one int16_t
            int16_t temp = (static_cast<int16_t>(rawData[2 * i]) << 8) | rawData[(2 * i) + 1];
            // Scale the value to acceleration
            returnData[i] = static_cast<acc::meters_per_second_squared_t>(temp) * scaling *
                            9.81; // @todo: Add bias calibration scaling
        }
        return returnData;
    }

    std::array<angVel::degrees_per_second_t, 3> MPU9250::readGyro() const
    {
        std::array<angVel::degrees_per_second_t, 3> returnData = {};
        std::array<uint8_t, 6> rawData = {};
        const float scaling = getGyroScaling();
        rawData = I2CInterface::readBytes<6>(mI2CID, mConfig.address, static_cast<char>(mFixedAddress::GYRO_XOUT_H));
        for(int i = 0; i < 3; i++)
        {
            // Bitshift uint8_t variables to one int16_t
            int16_t temp = (static_cast<int16_t>(rawData[2 * i]) << 8) | rawData[(2 * i) + 1];
            // Scale the value to degrees per second
            returnData[i] =
                static_cast<angVel::degrees_per_second_t>(temp) * scaling; // @todo: Add bias calibration scaling
        }
        return returnData;
    }

    std::array<magStr::gauss_t, 3> MPU9250::readMag() const
    {
        std::array<uint8_t, 7> rawData = {};

        bool receivedNewData = (I2CInterface::readByte(mI2CID, static_cast<char>(mFixedAddress::MAGADDR),
                                                       static_cast<char>(mFixedAddress::MAG_ST1)) &
                                0x01);
        if(receivedNewData)
        {
            rawData = I2CInterface::readBytes<7>(mI2CID, static_cast<char>(mFixedAddress::MAGADDR),
                                                 static_cast<char>(mFixedAddress::MAG_XOUT_L));
            // End data read by reading ST2 register
            uint8_t st2Reg = rawData[6];
            // Make sure ST2 reg isn't 1
            if(!(st2Reg & 0x08))
            {
                std::array<magStr::gauss_t, 3> returnData = {};
                const float scaling = getMagScaling();
                for(int i = 0; i < 3; i++)
                {
                    // Bitshift uint8_t variables to one int16_t
                    int16_t temp = static_cast<uint16_t>(rawData[2 * i + 1]) | rawData[(2 * i)];
                    // Scale the value to degrees per second
                    returnData[i] = magStr::gauss_t(temp * scaling);
                }
                return returnData;
            }
            else
                return mPreMagStr;
        }
        else
            return mPreMagStr;
    }

    temp::celsius_t MPU9250::readTemp() const
    {
        std::array<uint8_t, 2> rawData = {};
        rawData = I2CInterface::readBytes<2>(mI2CID, mConfig.address, static_cast<char>(mFixedAddress::TEMP_OUT_H));
        int16_t returnData = (static_cast<int16_t>(rawData[0]) << 8) | rawData[1];
        temp::celsius_t temperature(returnData / 333.87 + 21.0);
        return temperature;
    }

    SensorInterface::SensorType MPU9250::getType() const
    {
        SensorType type;
        type.accelerometer = true;
        type.gyroscope = true;
        type.magnetometer = true;
        type.temperature = true;
        return type;
    }
    constexpr float MPU9250::getAccScaling() const
    {
        // Note that 32769 is the maximum/minimum value of the acceleration
        switch(mConfig.accelScale)
        {
            // 2g
            case 0x0:
                return 2.0 / 32768.0;
            // 4g
            case 0x8:
                return 4.0 / 32768.0;
            // 8g
            case 0x10:
                return 8.0 / 32768.0;
            // 16g
            case 0x18:
                return 16.0 / 32768.0;
            default:
                MBED_ERROR(MBED_MAKE_ERROR(MBED_MODULE_APPLICATION, MBED_ERROR_INVALID_ARGUMENT),
                           "MPU9250 accelScale config is not valid.");
        }
    }
    constexpr float MPU9250::getGyroScaling() const
    {
        // Note that 32769 is the maximum/minimum value of the acceleration
        switch(mConfig.gyroScale)
        {
            // 250 dps
            case 0x0:
                return 250.0 / 32768.0;
            // 500 dps
            case 0x8:
                return 500.0 / 32768.0;
            // 1000 dps
            case 0x10:
                return 1000.0 / 32768.0;
            // 2000 dps
            case 0x18:
                return 2000.0 / 32768.0;
            default:
                MBED_ERROR(MBED_MAKE_ERROR(MBED_MODULE_APPLICATION, MBED_ERROR_INVALID_ARGUMENT),
                           "MPU9250 gyroScale config is not valid.");
        }
    }
    constexpr float MPU9250::getMagScaling() const
    {
        // Note that 32769 is the maximum/minimum value of the acceleration
        switch(mConfig.magScale)
        {
            // 14-bit output
            case 0x0:
                return 10000. * 4912. / 8190.;
            // 16-bit output
            case 0x10:
                return 10000. * 4912. / 32760.0;
            default:
                MBED_ERROR(MBED_MAKE_ERROR(MBED_MODULE_APPLICATION, MBED_ERROR_INVALID_ARGUMENT),
                           "MPU9250 magnetometer config is not valid.");
        }
    }

} // namespace Copter::Sensors