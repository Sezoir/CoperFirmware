// External lib
#include "mbed.h"

// Bootloader
#include "Bootloader/Startup.hpp"


#include "Sensors.hpp"

int main()
{
	App::Sensors::Sensors p;

	p.getSensor()->getIC()->;

	if(true) {}
    // Creates the startup.
    Bootloader::Startup boot;

    // Cheeks for successful startup.
    if(!boot.isValid())
        return -1;

    // Endless loop, as timers will call interrupts.
    while (true){}
}

