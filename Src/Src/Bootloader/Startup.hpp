//
// Created by Josh Mottley on 24/08/2018.
//

#ifndef COPTER_STARTUP_H
#define COPTER_STARTUP_H

#include <mbed.h>
#include <memory>

#include "Storage.hpp"
#include "Application/Application.hpp"

namespace Bootloader {

    class Startup {

    public:
        Startup();
        ~Startup();

        bool isValid();

    private:
        void initStorage();

        bool m_Success;
        Storage & m_Storage;
        std::unique_ptr<App::Application> m_Application;
    };

} // End of namespace
#endif //COPTER_STARTUP_H
