//
// Created by Josh Mottley on 27/08/2018.
//

#ifndef COPTER_MPU9250_H
#define COPTER_MPU9250_H

#include <array>
#include <map>
#include <Debugger/Debug.hpp>
#include "mbed.h"
#include "Application/Sensors/Factory/Models/Chip.hpp"
#include "Application/Drivers/I2CWrap.hpp"

namespace App { namespace Sensors { namespace SensorModels { namespace IMU {

                class MPU9250 : public Chip, public App::Drivers::I2CWrap {
                public:
                    MPU9250();
                    ~MPU9250();

                    std::array<int, 10> readSensor(); // Order of data is: Accel, Gyro, Mag, Temp.
                    std::array<int, 3> readAccel();
                    std::array<int, 3> readGyro();
                    std::array<int, 3> readMag();
                    const int readTemp();

                    std::shared_ptr<Chip> Create(std::map<std::string, uint8_t>) override;

                private:
                    bool Validate() override;

                    MPU9250(PinName t_Sda,
                            PinName t_Scl,
                            uint8_t t_Address,
                            uint8_t t_GyroScale,
                            uint8_t t_AccelScale,
                            uint8_t t_ClkSelect,
                            uint8_t t_GyroTempDigiFilter,
                            uint8_t t_AccelDigiFilter,
                            uint8_t t_GyroScaleRange,
                            uint8_t t_AccelScaleRange
                    );

                    enum m_FixedAddress : const char // Fixed address that do not have multiple configurations.
                    {
                      // IMUADDR = (0x68 << 1),
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

                    std::map<std::string, std::map<std::string, uint8_t>> m_Address =
                        {
                            {"Address", {
                                            {"HIGH", (0x68 << 1)},  // mbed supports 8 bit address for i2c.
                                            {"LOW", (0x69 << 1)}    // Need to cheek address and correspondence.
                                        }
                            },
                            {"GyroScale",
                                        {
                                            {"DPS250", 0},
                                            {"DPS500", 0x08},
                                            {"DPS1000", 0x10},
                                            {"DPS2000", 0x18}
                                        }
                            },
                            {
                             "AccelScale", {
                                            {"G2", 0},
                                            {"G4", 0x08},
                                            {"G8", 0x10},
                                            {"G16", 0x18}
                                        }
                            },
                            {
                             "ClkSelect", {
                                            {"20MHZ", 0},
                                            {"AUTO", 0x01}
                                        }
                            },
                            {
                             "GyroTempDigiFilter", {
                                            {"BYPASSONE", 0x00},
                                            {"BYPASSTWO", 0x01},
                                            {"ZERO", 0x00},
                                            {"ONE", 0x01},
                                            {"TWO", 0x02},
                                            {"THREE", 0x03},
                                            {"FOUR", 0x04},
                                            {"FIVE", 0x05},
                                            {"SIX", 0x06},
                                            {"SEVEN", 0x07}
                                        }
                            },
                            {
                             "AccelDigiFilter", {
                                            {"BYPASS", 0x00},
                                            {"ZERO", 0x00},
                                            {"ONE", 0x01},
                                            {"TWO", 0x02},
                                            {"THREE", 0x03},
                                            {"FOUR", 0x04},
                                            {"FIVE", 0x05},
                                            {"SIX", 0x06},
                                            {"SEVEN", 0x07}
                                        }
                            },
                            {
                             "GyroScaleRange", {
                                            {"DPS250", 0x00},
                                            {"DPS500", 0x08},
                                            {"DPS1000", 0x10},
                                            {"GPS2000", 0x18}
                                        }
                            },
                            {
                             "AccelScaleRange", {
                                            {"G2", 0x00},
                                            {"G4", 0x08},
                                            {"G8", 0x10},
                                            {"G16", 0x18}
                                        }
                            }
                        };

                    std::map<std::string, uint8_t> m_Configs =
                        {
                            {"Address", (0x68 << 1)},
                            {"GyroScale", 0x10},
                            {"AccelScale", 0x10},
                            {"ClkSelect", 0x01},
                            {"GyroTempDigiFilter", 0x00},
                            {"AccelDigiFilter", 0x00},
                            {"GyroScaleRange", 0x10},
                            {"AccelScaleRange", 0x10}
                        };

//                    enum class m_Configs : uint8_t
//                    {
//                        Address,
//                        GyroScale,
//                        AccelScale,
//                        CLKSLK,
//                        GyroTempDigiFilter,
//                        AccelDigiFilter,
//                        GyroScaleRange,
//                        AccelScaleRange
//                    };

                    void initMPU9250();
                    void initAK8963();
                };

            }}}} // end of namespace
#endif //COPTER_MPU9250_H
