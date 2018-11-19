#include "mbed.h"
#include "Debugger/Debug.hpp"
#include "Bootloader/Startup.hpp"

#include "Application/Sensors/Factory/Models/IMU/MPU9250.hpp"
#include <array>

int main() {

    Debugger::Debug debug;

    App::Sensors::SensorModels::IMU::MPU9250 sensor(D14, D15);

//    Bootloader::Startup boot;
//
//    if(boot.isValid())
//    {
//        Debugger::Debug::sendMsg("Boot is successful.");
//    }
//    else {return -1;}

    std::array<int, 10> sampData;
    bool endless = true;

    while (endless) {

        for(int i = 0; i < 10; i++)
        {
            std::array<int, 10> tempData = sensor.readSensor();
            for(int i = 0; i < 10; i++)
            {
                sampData[i] += tempData[i];
            }
        }

        for(int i = 0; i < 10; i++)
        {
            sampData[i] = sampData[i] / 10;
        }


        Debugger::Debug::sendMsg("AcX = %d", sampData[0]);
        Debugger::Debug::sendMsg("AcY = %d", sampData[1]);
        Debugger::Debug::sendMsg("AcZ = %d", sampData[2]);
        Debugger::Debug::sendMsg("GyX = %d", sampData[3]);
        Debugger::Debug::sendMsg("GyY = %d", sampData[4]);
        Debugger::Debug::sendMsg("GyZ = %d", sampData[5]);
        Debugger::Debug::sendMsg("MagX = %d", sampData[6]);
        Debugger::Debug::sendMsg("MagY = %d", sampData[7]);
        Debugger::Debug::sendMsg("MagZ = %d", sampData[8]);
        Debugger::Debug::sendMsg("Tmp = %.2f", (float)((sampData[9]/340)+36.53) ); // equation for degrees C from datasheet.

        wait(4);
    }
}

