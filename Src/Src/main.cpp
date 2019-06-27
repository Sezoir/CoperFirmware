// External lib
#include "mbed.h"

// Bootloader
#include "Bootloader/Startup.hpp"


int main()
{
	if(true) {}
    // Creates the startup.
    Bootloader::Startup boot;

    // Cheeks for successful startup.
    if(!boot.isValid())
        return -1;

    // Endless loop, as timers will call interrupts.
    while (true){}
}

