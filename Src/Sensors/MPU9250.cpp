#include "MPU9250.hpp"
#include "../Drivers/I2CInterface.hpp"

namespace Copter::Sensors
{
    using I2CInterface = Copter::Drivers::I2CInterface;

    MPU9250::MPU9250(PinName sda, PinName scl)
        : Sensor()
        , Accelerometer()
        , Gyroscope()
        , Magnetometer()
        , Thermometer()
        , mI2CID(Copter::Drivers::I2CInterface::getInstance(sda, scl))
    {
    }

    MPU9250::MPU9250(PinName sda, PinName scl, Config config)
        : Sensor()
        , Accelerometer()
        , Gyroscope()
        , Magnetometer()
        , Thermometer()
        , mI2CID(Copter::Drivers::I2CInterface::getInstance(sda, scl))
        , mConfig(config)
    {
    }

    bool MPU9250::init()
    {
        uint8_t config = 0;
        ///////////////////////////////// Setup MPU9150 (accel/gyro) ///////////////////////////////////////////////////
        calAccelGyroBias();

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

        ///////////////////////////////// Setup AK8963 (magnetometer) //////////////////////////////////////////////////
        I2CInterface::writeByte(mI2CID, static_cast<char>(mFixedAddress::MAGADDR),
                                static_cast<char>(mFixedAddress::MAG_CONTROL), 0x00); // Wake up mag
        ThisThread::sleep_for(10ms);

        // Enter Fuse ROM access mode
        I2CInterface::writeByte(mI2CID, static_cast<char>(mFixedAddress::MAGADDR),
                                static_cast<char>(mFixedAddress::MAG_CONTROL), 0x0F);
        ThisThread::sleep_for(10ms);

        std::array<uint8_t, 3> rawData = {};
        rawData = I2CInterface::readConsBytes<3>(mI2CID, static_cast<char>(mFixedAddress::MAGADDR),
                                                 static_cast<char>(mFixedAddress::AK8963_ASAX));
        mMagFactCalibration[0] = (float) (rawData[0] - 128) / 256. + 1.;
        mMagFactCalibration[1] = (float) (rawData[1] - 128) / 256. + 1.;
        mMagFactCalibration[2] = (float) (rawData[2] - 128) / 256. + 1.;

        // Power down magnetometer
        I2CInterface::writeByte(mI2CID, static_cast<char>(mFixedAddress::MAGADDR),
                                static_cast<char>(mFixedAddress::MAG_CONTROL), 0x00);
        ThisThread::sleep_for(10ms);

        // Set scale and mode
        config = mConfig.magScale | mConfig.magMode;
        I2CInterface::writeByte(mI2CID, static_cast<char>(mFixedAddress::MAGADDR),
                                static_cast<char>(mFixedAddress::MAG_CONTROL), config);
        ThisThread::sleep_for(10ms);

        // Calibrate magnetometer
        //        calMagBias();

        return true;
    }

    std::array<acc::meters_per_second_squared_t, 3> MPU9250::readAccel()
    {
        std::array<acc::meters_per_second_squared_t, 3> returnData = {};
        std::array<uint8_t, 6> rawData = {};
        const double scaling = getAccScaling();
        rawData =
            I2CInterface::readConsBytes<6>(mI2CID, mConfig.address, static_cast<char>(mFixedAddress::ACCEL_XOUT_H));
        for(int i = 0; i < 3; i++)
        {
            // Bitshift uint8_t variables to one int16_t
            int16_t temp = (static_cast<int16_t>(rawData[2 * i]) << 8) | rawData[(2 * i) + 1];
            // Scale the value to acceleration
            returnData[i] = (static_cast<acc::standard_gravity_t>(temp) * scaling);
        }
        return returnData;
    }

    std::array<angVel::degrees_per_second_t, 3> MPU9250::readGyro()
    {
        std::array<angVel::degrees_per_second_t, 3> returnData = {};
        std::array<uint8_t, 6> rawData = {};
        const double scaling = getGyroScaling();
        rawData =
            I2CInterface::readConsBytes<6>(mI2CID, mConfig.address, static_cast<char>(mFixedAddress::GYRO_XOUT_H));
        for(int i = 0; i < 3; i++)
        {
            // Bitshift uint8_t variables to one int16_t
            int16_t temp = (static_cast<int16_t>(rawData[2 * i]) << 8) | rawData[(2 * i) + 1];
            // Scale the value to degrees per second
            returnData[i] = static_cast<angVel::degrees_per_second_t>(temp) * scaling;
        }
        return returnData;
    }

    std::array<magStr::microtesla_t, 3> MPU9250::readMag()
    {
        const double scaling = getMagScaling();
        std::array<int16_t, 3> rawData = readRawMag();
        std::array<magStr::microtesla_t, 3> returnData = {};
        for(int i = 0; i < 3; i++)
        {
            // Scale the value to degrees per second
            returnData[i] = units::magnetic_field_strength::microtesla_t(static_cast<float>(rawData[0]) * scaling *
                                                                         mMagFactCalibration[i]) /
                            10;
            returnData[i] *= mMagBiaScaling[i];
        }
        return returnData;
    }

    temp::celsius_t MPU9250::readTemp() const
    {
        std::array<uint8_t, 2> rawData = {};
        rawData = I2CInterface::readConsBytes<2>(mI2CID, mConfig.address, static_cast<char>(mFixedAddress::TEMP_OUT_H));
        int16_t returnData = (static_cast<int16_t>(rawData[0]) << 8) | rawData[1];
        temp::celsius_t temperature(returnData / 333.87 + 21.0);
        return temperature;
    }

    double MPU9250::getAccScaling() const
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

    double MPU9250::getGyroScaling() const
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

    double MPU9250::getMagScaling() const
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

    void MPU9250::calAccelGyroBias() const
    {
        // Reset device
        I2CInterface::writeByte(mI2CID, mConfig.address, static_cast<char>(mFixedAddress::PWR_MGMT_1), 0x80);
        ThisThread::sleep_for(100ms);

        // Get stable time source
        I2CInterface::writeByte(mI2CID, mConfig.address, static_cast<char>(mFixedAddress::PWR_MGMT_1), 0x01);
        I2CInterface::writeByte(mI2CID, mConfig.address, static_cast<char>(mFixedAddress::PWR_MGMT_2), 0x00);
        ThisThread::sleep_for(200ms);

        /*------------- Configure device for bias calculations --------------------*/
        // Disable all interrupts
        I2CInterface::writeByte(mI2CID, mConfig.address, static_cast<char>(mFixedAddress::INT_ENABLE), 0x00);
        // Disable FIFO
        I2CInterface::writeByte(mI2CID, mConfig.address, static_cast<char>(mFixedAddress::FIFO_EN), 0x00);
        // Turn on internal clock source
        I2CInterface::writeByte(mI2CID, mConfig.address, static_cast<char>(mFixedAddress::PWR_MGMT_1), 0x00);
        // Disable I2C master
        I2CInterface::writeByte(mI2CID, mConfig.address, static_cast<char>(mFixedAddress::I2C_MST_CTRL), 0x00);
        // Disable FIFO and I2C master modes
        I2CInterface::writeByte(mI2CID, mConfig.address, static_cast<char>(mFixedAddress::USER_CTRL), 0x00);
        // Reset FIFO and DMP
        I2CInterface::writeByte(mI2CID, mConfig.address, static_cast<char>(mFixedAddress::USER_CTRL), 0x0C);
        ThisThread::sleep_for(15ms);

        I2CInterface::writeByte(mI2CID, mConfig.address, static_cast<char>(mFixedAddress::CONFIG), 0x01);
        I2CInterface::writeByte(mI2CID, mConfig.address, static_cast<char>(mFixedAddress::SMPLRT_DIV), 0x00);
        I2CInterface::writeByte(mI2CID, mConfig.address, static_cast<char>(mFixedAddress::GYRO_CONFIG), 0x00); // 0x00
        I2CInterface::writeByte(mI2CID, mConfig.address, static_cast<char>(mFixedAddress::ACCEL_CONFIG_ONE),
                                0x00); // 0x00
        /*-------------------------------------------------------------------------*/

        // Configure FIFO to capture accelerometer and gyro data
        // Enable FIFO
        I2CInterface::writeByte(mI2CID, mConfig.address, static_cast<char>(mFixedAddress::USER_CTRL), 0x40);
        // Enable accelerometer and gyro for FIFO
        I2CInterface::writeByte(mI2CID, mConfig.address, static_cast<char>(mFixedAddress::FIFO_EN), 0x78);
        // Capture 40 samples in ~40 milliseconds
        ThisThread::sleep_for(41ms);
        // Turn off FIFO
        I2CInterface::writeByte(mI2CID, mConfig.address, static_cast<char>(mFixedAddress::FIFO_EN), 0x00);

        // Read FIFO sample count and number of full sets of gyro/accel data
        uint16_t fifoCnt, packetCnt;
        {
            std::array<uint8_t, 2> cnt =
                I2CInterface::readConsBytes<2>(mI2CID, mConfig.address, static_cast<char>(mFixedAddress::FIFO_COUNTH));
            fifoCnt = (static_cast<uint16_t>(cnt[0]) << 8) | cnt[1];
            packetCnt = fifoCnt / 12;
        }

        std::array<uint8_t, 12> rawData = {};
        std::array<int32_t, 3> avrAccelData = {0, 0, 0};
        std::array<int32_t, 3> avrGyroData = {0, 0, 0};
        for(uint packet = 0; packet < packetCnt; packet++)
        {
            // Get next data packet
            rawData = I2CInterface::readBytes<12>(mI2CID, mConfig.address, static_cast<char>(mFixedAddress::FIFO_R_W));
            // Correctly bitshift and add to average variables
            for(int i = 0; i < 3; i++)
            {
                int16_t accelTemp = (static_cast<int16_t>(rawData[2 * i]) << 8) | rawData[(2 * i) + 1];
                int16_t gyroTemp = (static_cast<int16_t>(rawData[2 * i + 6]) << 8) | rawData[(2 * i) + 7];

                avrAccelData[i] += static_cast<int32_t>(accelTemp);
                avrGyroData[i] += static_cast<int32_t>(gyroTemp);
            }
        }

        // Calculate average bias
        avrAccelData[0] /= static_cast<int32_t>(packetCnt);
        avrAccelData[1] /= static_cast<int32_t>(packetCnt);
        avrAccelData[2] /= static_cast<int32_t>(packetCnt);
        avrGyroData[0] /= static_cast<int32_t>(packetCnt);
        avrGyroData[1] /= static_cast<int32_t>(packetCnt);
        avrGyroData[2] /= static_cast<int32_t>(packetCnt);

        const uint16_t accelSensitivity = 16384; // = 16384 LSB/g
        // Remove gravity from the z-axis accelerometer bias calculation
        if(avrAccelData[2] > 0L)
            avrAccelData[2] -= (int32_t) accelSensitivity;
        else
            avrAccelData[2] += (int32_t) accelSensitivity;

        // Declare empty array for storing data to use when writing to I2C
        std::array<uint8_t, 6> data = {};
        // Construct gyro bias to conform to hardware bias registers
        // Divide by 4 to get 32.9 LSB per deg/s to conform to expected input format
        // Also bias are additive, so change sign on calculated average gyro
        data[0] = (-avrGyroData[0] / 4 >> 8) & 0xFF;
        data[1] = (-avrGyroData[0] / 4) & 0xFF;
        data[2] = (-avrGyroData[1] / 4 >> 8) & 0xFF;
        data[3] = (-avrGyroData[1] / 4) & 0xFF;
        data[4] = (-avrGyroData[2] / 4 >> 8) & 0xFF;
        data[5] = (-avrGyroData[2] / 4) & 0xFF;
        // Push gyro biases to hardware registers
        I2CInterface::writeByte(mI2CID, mConfig.address, static_cast<char>(mFixedAddress::XG_OFFSET_H), data[0]);
        I2CInterface::writeByte(mI2CID, mConfig.address, static_cast<char>(mFixedAddress::XG_OFFSET_L), data[1]);
        I2CInterface::writeByte(mI2CID, mConfig.address, static_cast<char>(mFixedAddress::YG_OFFSET_H), data[2]);
        I2CInterface::writeByte(mI2CID, mConfig.address, static_cast<char>(mFixedAddress::YG_OFFSET_L), data[3]);
        I2CInterface::writeByte(mI2CID, mConfig.address, static_cast<char>(mFixedAddress::ZG_OFFSET_H), data[4]);
        I2CInterface::writeByte(mI2CID, mConfig.address, static_cast<char>(mFixedAddress::ZG_OFFSET_L), data[5]);

        // Now construct the biases for the accelerometer. Note that these registers contain a factory trim value which
        // must be added to our avrAccelData. In addition bit 0 of the lower byte must be preservedsince it is used for
        // temperature compensation calculations. Accelerometer bias registers expect bias input as 2048 LSB per g, so
        // the accelerometer biases calculated above must be divided by 8.

        // Declare trim value array
        std::array<int32_t, 3> accelTrim = {};
        // Read/store the trim values
        {
            std::array<uint8_t, 2> temp =
                I2CInterface::readConsBytes<2>(mI2CID, mConfig.address, static_cast<char>(mFixedAddress::XA_OFFSET_H));
            accelTrim[0] = (static_cast<int16_t>(temp[0]) << 8) | temp[1];

            temp =
                I2CInterface::readConsBytes<2>(mI2CID, mConfig.address, static_cast<char>(mFixedAddress::YA_OFFSET_H));
            accelTrim[1] = (static_cast<int16_t>(temp[0]) << 8) | temp[1];

            temp =
                I2CInterface::readConsBytes<2>(mI2CID, mConfig.address, static_cast<char>(mFixedAddress::ZA_OFFSET_H));
            accelTrim[2] = (static_cast<int16_t>(temp[0]) << 8) | temp[1];
        }
        // Define mask for temperature compensation bit
        uint16_t mask = 1UL;
        std::array<uint8_t, 3> maskBit = {0, 0, 0};

        for(uint i = 0; i < 3; i++)
        {
            // If temperature compensation bit is set, record that fact in mask_bit
            if(accelTrim[i] & mask)
                maskBit[i] = 0x01;
        }
        // Construct total accel bias
        accelTrim[0] -= (avrAccelData[0] / 8);
        accelTrim[1] -= (avrAccelData[1] / 8);
        accelTrim[2] -= (avrAccelData[2] / 8);

        // Construct accelerometer bias to conform to hardware bias registers
        data[0] = (accelTrim[0] >> 8) & 0xFF;
        data[1] = (accelTrim[0]) & 0xFF;
        data[1] = data[1] | maskBit[0];
        data[2] = (accelTrim[1] >> 8) & 0xFF;
        data[3] = (accelTrim[1]) & 0xFF;
        data[3] = data[3] | maskBit[1];
        data[4] = (accelTrim[2] >> 8) & 0xFF;
        data[5] = (accelTrim[2]) & 0xFF;
        data[5] = data[5] | maskBit[2];

        // Send accel bias to hardware
        I2CInterface::writeByte(mI2CID, mConfig.address, static_cast<char>(mFixedAddress::XA_OFFSET_H), data[0]);
        I2CInterface::writeByte(mI2CID, mConfig.address, static_cast<char>(mFixedAddress::XA_OFFSET_L), data[1]);
        I2CInterface::writeByte(mI2CID, mConfig.address, static_cast<char>(mFixedAddress::YA_OFFSET_H), data[2]);
        I2CInterface::writeByte(mI2CID, mConfig.address, static_cast<char>(mFixedAddress::YA_OFFSET_L), data[3]);
        I2CInterface::writeByte(mI2CID, mConfig.address, static_cast<char>(mFixedAddress::ZA_OFFSET_H), data[4]);
        I2CInterface::writeByte(mI2CID, mConfig.address, static_cast<char>(mFixedAddress::ZA_OFFSET_L), data[5]);
    }

    void MPU9250::calMagBias()
    {
        // Find sample count
        uint16_t sampleCount = 0;
        if(mConfig.magMode == 0x02)
            sampleCount = 128; // at 8 Hz ODR, new mag data is available every 125 ms
        else if(mConfig.magMode == 0x06)
            sampleCount = 1500; // at 100 Hz ODR, new mag data is available every 10 ms

        std::array<int16_t, 3> magMax = {-32767, -32767, -32767};
        std::array<int16_t, 3> magMin = {32767, 32767, 32767};
        std::array<int16_t, 3> magTemp = {0, 0, 0};
        for(uint i = 0; i < sampleCount; i++)
        {
            magTemp = readRawMag();

            for(int j = 0; j < 3; j++)
            {
                if(magTemp[j] > magMax[j])
                    magMax[j] = magTemp[j];
                if(magTemp[j] < magMin[j])
                    magMin[j] = magTemp[j];
            }
            if(mConfig.magMode == 0x02)
                ThisThread::sleep_for(135ms); // at 8 Hz ODR, new mag data is available every 125 ms
            else if(mConfig.magMode == 0x06)
                ThisThread::sleep_for(12ms); // at 100 Hz ODR, new mag data is available every 10 ms
        }
        // Get corrections
        mMagBias[0] = (magMax[0] + magMin[0]) / 2.;
        mMagBias[1] = (magMax[1] + magMin[1]) / 2.;
        mMagBias[2] = (magMax[2] + magMin[2]) / 2.;

        mMagBias[0] = mMagBias[0] * mMagFactCalibration[0] * getMagScaling();
        mMagBias[1] = mMagBias[1] * mMagFactCalibration[1] * getMagScaling();
        mMagBias[2] = mMagBias[2] * mMagFactCalibration[2] * getMagScaling();

        mMagBiaScaling[0] = (magMax[0] - magMin[0]) / 2.;
        mMagBiaScaling[1] = (magMax[1] - magMin[1]) / 2.;
        mMagBiaScaling[2] = (magMax[2] - magMin[2]) / 2.;

        auto avr = mMagBiaScaling[0] + mMagBiaScaling[1] + mMagBiaScaling[2];
        avr /= 3.0;

        mMagBiaScaling[0] = avr / mMagBiaScaling[0];
        mMagBiaScaling[1] = avr / mMagBiaScaling[1];
        mMagBiaScaling[2] = avr / mMagBiaScaling[2];
    }

    std::array<int16_t, 3> MPU9250::readRawMag() const
    {
        std::array<uint8_t, 7> rawData = {};

        bool receivedNewData = (I2CInterface::readByte(mI2CID, static_cast<char>(mFixedAddress::MAGADDR),
                                                       static_cast<char>(mFixedAddress::MAG_ST1)) &
                                0x01);
        if(receivedNewData)
        {
            rawData = I2CInterface::readConsBytes<7>(mI2CID, static_cast<char>(mFixedAddress::MAGADDR),
                                                     static_cast<char>(mFixedAddress::MAG_XOUT_L));
            // End data read by reading ST2 register
            uint8_t st2Reg = rawData[6];
            // Make sure ST2 reg isn't 1
            if(!(st2Reg & 0x08))
            {
                std::array<int16_t, 3> returnData = {};
                for(int i = 0; i < 3; i++)
                {
                    // Bitshift uint8_t variables to one int16_t
                    returnData[i] = static_cast<uint16_t>(rawData[2 * i + 1]) | rawData[2 * i];
                }
                return returnData;
            }
            else
                return std::array<int16_t, 3>{0, 0, 0};
        }
        else
            return std::array<int16_t, 3>{0, 0, 0};
    }

} // namespace Copter::Sensors