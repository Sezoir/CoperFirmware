#include "Mpu9250.hpp"
#include "../Drivers/I2CInterface.hpp"

namespace Copter::Sensors
{
    using I2CInterface = Copter::Drivers::I2CInterface;

    MPU9250::MPU9250(PinName sda, PinName scl) : mI2CID(Copter::Drivers::I2CInterface::getInstance(sda, scl)) {}

    MPU9250::MPU9250(PinName sda, PinName scl, MPU9250::Config config) :
        mI2CID(Copter::Drivers::I2CInterface::getInstance(sda, scl)),
        mConfig(config) {}

    bool MPU9250::setup()
    {
        // MPU9250 starts of in sleep mode, so need to clear address.
        I2CInterface::writeByte(this->mI2CID, this->mConfig.address,
                                static_cast<char>(mFixedAddress::PWR_MGMT_1), 0); // Wake up MPU

        // Set clock source to be Phase Locked Looped
        I2CInterface::writeByte(this->mI2CID, this->mConfig.address,
                                static_cast<char>(mFixedAddress::PWR_MGMT_1), 0x01);

        // Set up the Digital Low Pass Filter
        I2CInterface::writeByte(this->mI2CID, this->mConfig.address,
                                static_cast<char>(mFixedAddress::CONFIG), this->mConfig.gyroTempDigiFilter);
        I2CInterface::writeByte(this->mI2CID, this->mConfig.address,
                                static_cast<char>(mFixedAddress::ACCEL_CONFIG_TWO), this->mConfig.accelDigiFilter);

        // Set sample rate of gyro.
        // Note: sample rate = Gyroscope sample rate/(1+SMPLRT_DIV)
        // Also that gyroscope sample rate = 1000Hz.
        // Note that the Fchoice register must = 0b11, i.e not bypass the digital low pass filter.
        I2CInterface::writeByte(this->mI2CID, this->mConfig.address,
                                static_cast<char>(mFixedAddress::SMPLRT_DIV), 0x04); // 200Hz

        // Set gyro scale range.
        I2CInterface::writeByte(this->mI2CID, this->mConfig.address,
                                static_cast<char>(mFixedAddress::GYRO_CONFIG), this->mConfig.gyroScale);

        // Set Accelerometer scale range.
        I2CInterface::writeByte(this->mI2CID, this->mConfig.address,
                                static_cast<char>(mFixedAddress::ACCEL_CONFIG_ONE), this->mConfig.accelScale);

        // Setup Interrupts and Bypass mode
        I2CInterface::writeByte(this->mI2CID, this->mConfig.address,
                                static_cast<char>(mFixedAddress::INT_PIN_CFG), 0x22);
        I2CInterface::writeByte(this->mI2CID, this->mConfig.address,
                                static_cast<char>(mFixedAddress::INT_ENABLE), 0x01);
        ThisThread::sleep_for(1ms);

        // Setup AK8963 (magnetometer)
        I2CInterface::writeByte(this->mI2CID, static_cast<char>(mFixedAddress::MAGADDR),
                                static_cast<char>(mFixedAddress::MAG_CONTROL), 0); // Wake up mag
        ThisThread::sleep_for(1ms);
        I2CInterface::writeByte(this->mI2CID, static_cast<char>(mFixedAddress::MAGADDR),
                                static_cast<char>(mFixedAddress::MAG_CONTROL), 0x16);
        ThisThread::sleep_for(1ms);
        return true;
    }

    std::array<int, 10> MPU9250::readSensor() const
    {
        std::array<int, 10> returnData = {};
        std::array<int, 3> tempData = {};
        tempData = readAccel();
        std::copy(tempData.begin(), tempData.end(), returnData.begin());
        tempData = readGyro();
        std::copy(tempData.begin(), tempData.end(), returnData.begin()+3);
        tempData = readMag();
        std::copy(tempData.begin(), tempData.end(), returnData.begin()+6);
        returnData[9] = readTemp();
        return returnData;
    }

    std::array<int, 3> MPU9250::readAccel() const
    {
        std::array<int, 3> returnData = {};
        std::vector<int8_t> tempData = {};
        tempData = I2CInterface::readBytes(this->mI2CID, this->mConfig.address,
                                           static_cast<char>(mFixedAddress::ACCEL_XOUT_H), 6);
        for (int i = 0; i < 3; ++i)
        {
            returnData[i] = (tempData[2*i] << 8) | tempData[(2*i)+1];
        }
        return returnData;
    }

    std::array<int, 3> MPU9250::readGyro() const
    {
        std::array<int, 3> returnData = {};
        std::vector<int8_t> tempData;
        tempData = I2CInterface::readBytes(this->mI2CID, this->mConfig.address,
                                           static_cast<char>(mFixedAddress::GYRO_XOUT_H), 6);
        for (int i = 0; i < 3; ++i)
        {
            returnData[i] = (tempData[2*i] << 8) | tempData[(2*i)+1];
        }
        return returnData;
    }

    std::array<int, 3> MPU9250::readMag() const
    {
        if(I2CInterface::readByte(this->mI2CID, static_cast<char>(mFixedAddress::MAGADDR),
                                  static_cast<char>(mFixedAddress::MAG_ST1)) & 0x01) {
            std::array<int, 3> returnData = {};
            std::vector<int8_t> tempData;
            tempData = I2CInterface::readBytes(this->mI2CID, static_cast<char>(mFixedAddress::MAGADDR),
                                               static_cast<char>(mFixedAddress::MAG_XOUT_L), 7);
            returnData[0] = (int)(((int)(tempData[1]) << 8) | tempData[0]);
            returnData[1] = (int)(((int)(tempData[3]) << 8) | tempData[2]);
            returnData[2] = (int)(((int)(tempData[5]) << 8) | tempData[4]);
            return returnData;
        } else {
            std::array<int, 3> returnData{0,0,0};
            return returnData;
        }
    }

    int MPU9250::readTemp() const
    {
        std::vector<int8_t> tempData;
        tempData = I2CInterface::readBytes(this->mI2CID, this->mConfig.address,
                                           static_cast<char>(mFixedAddress::TEMP_OUT_H), 2);
        return (int)((tempData[0] << 8) | tempData[1]);
    }




}