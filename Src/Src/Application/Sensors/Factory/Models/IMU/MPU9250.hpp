//
// Created by Josh Mottley on 27/08/2018.
//

#ifndef COPTER_MPU9250_H
#define COPTER_MPU9250_H

#include <array>
#include "mbed.h"
#include "Application/Sensors/Factory/Models/Chip.hpp"
#include "Drivers/I2C/I2CWrap.hpp"

namespace App { namespace Sensors { namespace SensorModels { namespace IMU {

                class MPU9250 : public Chip, public App::I2C::I2CWrap
                {
                public:
                    MPU9250(PinName t_Sda, PinName t_Scl);
                    ~MPU9250();

                    std::array<int, 10> readSensor();
                    std::array<int, 3> readAccel();
                    std::array<int, 3> readGyro();
                    std::array<int, 3> readMag();
                    const int readTemp();


                private:
                    void initMPU9250();
                    void initAK8963();

                    enum Address : uint8_t
                    {
                        IMUADDR = (0x68 << 1), // mbed supports 8 bit address for i2c.
                        PWR_MGMT_1 = 0x6B,
                        CONFIG = 0x1A,
                        SMPLRT_DIV = 0x19,
                        GYRO_CONFIG = 0x1B,
                        ACCEL_CONFIG = 0x1C,
                        ACCEL_XOUT_H = 0x3B,
                        TEMP_OUT_H = 0x41,
                        GYRO_XOUT_H = 0x43,
                        MAGADDR = (0x0C << 1), // mbed supports 8 bit address for i2c.
                        MAG_CONTROL = 0x0A,
                        MAG_ST1 = 0x02,
                        MAG_XOUT_L = 0x03

                    };

                    enum GyroScale : uint8_t
                    {
                        DPS250 = 0,
                        DPS500 = 0x08,
                        DPS1000 = 0x10,
                        DPS2000 = 0x18,
                    };

                    enum AccelScale : uint8_t
                    {
                        G2 = 0,
                        G4 = 0x08,
                        G8 = 0x10,
                        G16 = 0x18,

                    };
                };

            }}}} // end of namespace
#endif //COPTER_MPU9250_H
