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

    std::array<int, 10> array;
    bool endless = true;

    while (endless) {
        array = sensor.readSensor();

        Debugger::Debug::sendMsg("AcX = %d", array[0]);
        Debugger::Debug::sendMsg("AcY = %d", array[1]);
        Debugger::Debug::sendMsg("AcZ = %d", array[2]);
        Debugger::Debug::sendMsg("GyX = %d", array[3]);
        Debugger::Debug::sendMsg("GyY = %d", array[4]);
        Debugger::Debug::sendMsg("GyZ = %d", array[5]);
        Debugger::Debug::sendMsg("MagX = %d", array[6]);
        Debugger::Debug::sendMsg("MagY = %d", array[7]);
        Debugger::Debug::sendMsg("MagZ = %d", array[8]);
        Debugger::Debug::sendMsg("Tmp = %.2f", (float)((array[9]/340)+36.53) ); // equation for degrees C from datasheet.

        wait(4);
    }
}

