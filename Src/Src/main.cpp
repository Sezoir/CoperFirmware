#include "mbed.h"
#include "Debugger/Debug.hpp"
#include "Bootloader/Startup.hpp"

#include "Application/Drivers/FileSystem/FileSystem.hpp"
#include "string"

int main() {

    Debugger::Debug debug;

    Bootloader::Startup boot;

    if(boot.isValid())
    {
        Debugger::Debug::sendMsg("Boot is successful.");
    }
    else {return -1;}

    bool endless = true;
    while (endless) {
    }
}

