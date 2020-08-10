#include "mbed.h"
#include "Engine/DShot.hpp"

BufferedSerial serial(USBTX, USBRX, 115200);
DigitalIn button(BUTTON1);
Ticker updater;

Copter::Engine::DShot protoOne(1200, Copter::Engine::DShot::Pin::PC7);
Copter::Engine::DShot protoTwo(1200, Copter::Engine::DShot::Pin::PB8);

void callb()
{
    protoOne.sendSignal();
    protoTwo.sendSignal();
}

int main()
{
//    char const creMessage[] = "Creating DShot protocol!\n";
//    serial.write(creMessage, sizeof(creMessage));

    if (button.is_connected()) {
        printf("mypin is connected and initialized! \n\r");
    }

    // Optional: set mode as PullUp/PullDown/PullNone/OpenDrain
    button.mode(PullNone);

    protoOne.setup();
    protoTwo.setup();



//    updater.attach(callback(&proto, &Copter::Engine::DShot::sendSignal), 50us);
    updater.attach(callb, 50us);

    while(true)
    {
        if(button.read())
            for(int x=0; x<2000; x++)
            {
                protoOne.incThrottle(0.005f);
                protoTwo.incThrottle(0.005f);
            }

        ThisThread::sleep_for(250);

//        char const message[] = "DShot protocol!\n";
//        serial.write(message, sizeof(message));
//        ThisThread::sleep_for(1000);
    }
}