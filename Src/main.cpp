#include "mbed.h"
#include "Engine/DShot.hpp"

//BufferedSerial serial(USBTX, USBRX, 115200);

int main()
{
//    char const creMessage[] = "Creating DShot protocol!\n";
//    serial.write(creMessage, sizeof(creMessage));

    Copter::Engine::DShot proto(1200);
    proto.setup();
    proto.sendSignal();

//    while(true)
//    {
//        char const message[] = "DShot protocol!\n";
//        serial.write(message, sizeof(message));
//        ThisThread::sleep_for(1000);
//    }
}