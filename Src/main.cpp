#include "mbed.h"
#include "Engine/DShot.hpp"

BufferedSerial serial(USBTX, USBRX, 115200);
DigitalIn button(BUTTON1);
Ticker updater;

int main()
{
//    char const creMessage[] = "Creating DShot protocol!\n";
//    serial.write(creMessage, sizeof(creMessage));

    if (button.is_connected()) {
        printf("mypin is connected and initialized! \n\r");
    }

    // Optional: set mode as PullUp/PullDown/PullNone/OpenDrain
    button.mode(PullNone);

    Copter::Engine::DShot proto(1200);
    proto.setup();
    updater.attach(callback(&proto, &Copter::Engine::DShot::sendSignal), 50us);

    while(true)
    {
        if(button.read())
            for(int x=0; x<2000; x++)
                proto.incThrottle(0.005f);

        ThisThread::sleep_for(250);

//        char const message[] = "DShot protocol!\n";
//        serial.write(message, sizeof(message));
//        ThisThread::sleep_for(1000);
    }
}