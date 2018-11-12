//
// Created by Josh Mottley on 24/08/2018.
//

#ifndef COPTER_STARTUP_H
#define COPTER_STARTUP_H

#include <mbed.h>
#include "Storage/Manager.hpp"
#include "Application/Application.hpp"

namespace Bootloader {

    class Startup {

    private:
        bool m_Success;
        Storage::Manager & m_Manager;
        App::Application & m_Application;

    public:
        Startup();
        ~Startup();

        bool isValid();
    };

} // End of namespace
#endif //COPTER_STARTUP_H
