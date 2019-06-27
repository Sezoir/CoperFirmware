#pragma once

// External Lib
#include <mbed.h>

// Std Lib
#include <memory>

// Basic Serial Class
#include "Debug.hpp"

// Bootloaded Classes
#include "Storage.hpp"
#include "Application/Application.hpp"

namespace Bootloader {

    class Startup {

    public:
        /**
         *  Creates Startup, which bootloads the application.
         */
        Startup();

        /**
         * Default Destructor.
         */
        ~Startup() = default;

        /**
         * Checks whether Startup creation was successful.
         * @return Value of m_Success
         */
        bool isValid();

    private:
        /**
         * Creates and initialises a pair of a FileSystem and a BlockDevice, under a "name".
         */
        void initStorage();

        /**
         * Bool on success of Startup creation.
         * Defaults value = true.
         */
        bool m_Success;

        // Points to classes created in initialisation.
        Debugger::Debug & m_Debug;
        Storage & m_Storage;
        App::Application * m_application;
    };

} // End of namespace
