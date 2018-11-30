//
// Created by Josh Mottley on 27/08/2018.
//

#include "MPU9250.hpp"
#include <algorithm>
#include "Debugger/Debug.hpp"

App::Sensors::SensorModels::IMU::MPU9250::MPU9250(PinName t_Sda, PinName t_Scl,
                                                  uint8_t t_Address,
                                                  uint8_t t_GyroScale,
                                                  uint8_t t_AccelScale,
                                                  uint8_t t_ClkSelect,
                                                  uint8_t t_GyroTempDigiFilter,
                                                  uint8_t t_AccelDigiFilter,
                                                  uint8_t t_GyroScaleRange,
                                                  uint8_t t_AccelScaleRange) : I2CWrap(t_Sda, t_Scl)
{
    Debugger::Debug::sendMsg("MPU9250 Created.");
    initMPU9250();
    initAK8963();
    Debugger::Debug::sendMsg("Initilisation Finished");
}

App::Sensors::SensorModels::IMU::MPU9250::~MPU9250()
{
    Debugger::Debug::sendMsg("MPU9250 Destroyed");
}

std::shared_ptr<App::Sensors::SensorModels::Chip> App::Sensors::SensorModels::IMU::MPU9250::Create()
{
    if(Validate())
    {
        //struct m_EnableMakeShared : public MPU9250 {};  // Gives make_shared a public constructor.
        return make_shared<MPU9250>();
    } else
    {
        Debugger::Debug::sendMsg("MPU9250 parameter mismatch.");
        while(true) {};
    }
}

bool App::Sensors::SensorModels::IMU::MPU9250::Validate()
{
    return true;
}

void App::Sensors::SensorModels::IMU::MPU9250::initMPU9250()
{
    // MPU9250 starts of in sleep mode, so need to clear address.
    writeByte(m_Configs["Address"], m_FixedAddress::PWR_MGMT_1, 0); // Wake up MPU

    // Set clock source to be Phase Locked Looped
    writeByte(m_Configs["Address"], m_FixedAddress::PWR_MGMT_1, 0x01);

    // Set up the Digital Low Pass Filter
    writeByte(m_Configs["Address"], m_FixedAddress::CONFIG, m_Configs["GyroTempDigiFilter"]);
    writeByte(m_Configs["Address"], m_FixedAddress::ACCEL_CONFIG_TWO, m_Configs["AccelDigiFilter"]);

    // Set sample rate of gyro.
    // Note: sample rate = Gyroscope sample rate/(1+SMPLRT_DIV)
    // Also that gyroscope sample rate = 1000Hz.
    // Note that the Fchoice register must = 0b11, i.e not bypass the digital low pass filter.
    writeByte(m_Configs["Address"], m_FixedAddress::SMPLRT_DIV, 0x04); // 200Hz

    // Set gyro scale range.
    writeByte(m_Configs["Address"], m_FixedAddress::GYRO_CONFIG, m_Configs["GyroScale"]);

    // Set Accelerometer scale range.
    writeByte(m_Configs["Address"], m_FixedAddress::ACCEL_CONFIG_ONE, m_Configs["AccelScale"]);

    // Set up Interrupts and Bypass mode
    writeByte(m_Configs["Address"], m_FixedAddress::INT_PIN_CFG, 0x22);
    writeByte(m_Configs["Address"], m_FixedAddress::INT_ENABLE, 0x01);
    wait(0.1); // Wait for pass-through mode enabled.
}

void App::Sensors::SensorModels::IMU::MPU9250::initAK8963()
{
    writeByte(m_FixedAddress::MAGADDR, m_FixedAddress::MAG_CONTROL, 0); // Wake up mag
    wait(0.1);
    writeByte(m_FixedAddress::MAGADDR, m_FixedAddress::MAG_CONTROL, 0x16);
    wait(0.1);
    Debugger::Debug::sendMsg("0x%x", readByte(m_FixedAddress::MAGADDR, 0x00));
}

std::array<int, 10> App::Sensors::SensorModels::IMU::MPU9250::readSensor()
{
    Debugger::Debug::sendMsg("Reading Sensors.");
    std::array<int, 10> returnData;
    std::array<int, 3> tempData;
    tempData = readAccel();
    std::copy(tempData.begin(), tempData.end(), returnData.begin());
    tempData = readGyro();
    std::copy(tempData.begin(), tempData.end(), returnData.begin()+3);
    tempData = readMag();
    std::copy(tempData.begin(), tempData.end(), returnData.begin()+6);
    returnData[9] = readTemp();
    return returnData;
}

std::array<int, 3> App::Sensors::SensorModels::IMU::MPU9250::readAccel()
{
    //Debugger::Debug::sendMsg("Reading Accel");
    std::array<int, 3> returnData;
    std::vector<int8_t> tempData;
    tempData = readBytes(m_Configs["Address"], m_FixedAddress::ACCEL_XOUT_H, 6);
    for (int i = 0; i < 3; ++i)
    {
        returnData[i] = (tempData[2*i] << 8) | tempData[(2*i)+1];
    }
    //Debugger::Debug::sendMsg("Reading Finished");
    return returnData;
}

std::array<int, 3> App::Sensors::SensorModels::IMU::MPU9250::readGyro()
{
//    Debugger::Debug::sendMsg("Reading Gyro");
    std::array<int, 3> returnData;
    std::vector<int8_t> tempData;
    tempData = readBytes(m_Configs["Address"], m_FixedAddress::GYRO_XOUT_H, 6);
    for (int i = 0; i < 3; ++i)
    {
        returnData[i] = (tempData[2*i] << 8) | tempData[(2*i)+1];
    }
//    Debugger::Debug::sendMsg("Reading Finished");
    return returnData;
}

std::array<int, 3> App::Sensors::SensorModels::IMU::MPU9250::readMag()
{
    if(readByte(m_FixedAddress::MAGADDR, m_FixedAddress::MAG_ST1) & 0x01) {
//        Debugger::Debug::sendMsg("Reading Mag");
        std::array<int, 3> returnData;
        std::vector<int8_t> tempData;
        tempData = readBytes(m_FixedAddress::MAGADDR, m_FixedAddress::MAG_XOUT_L, 7);
        returnData[0] = (int)(((int)(tempData[1]) << 8) | tempData[0]);
        returnData[1] = (int)(((int)(tempData[3]) << 8) | tempData[2]);
        returnData[2] = (int)(((int)(tempData[5]) << 8) | tempData[4]);
//        for (int i = 0; i < 3; i++) {
//            returnData[i] = ((int)tempData[(2 * i) + 1] << 8) | (int)tempData[2 * i];
//        }
//        Debugger::Debug::sendMsg("Reading Finished");
        return returnData;
    } else {
        std::array<int, 3> returnData{0,0,0};
        return returnData;
    }

}

const int App::Sensors::SensorModels::IMU::MPU9250::readTemp()
{
//    Debugger::Debug::sendMsg("Reading Temp");
    std::vector<int8_t> tempData;
    tempData = readBytes(m_Configs["Address"], m_FixedAddress::TEMP_OUT_H, 2);
//    Debugger::Debug::sendMsg("Reading Finished");
    return (int)((tempData[0] << 8) | tempData[1]);
}





