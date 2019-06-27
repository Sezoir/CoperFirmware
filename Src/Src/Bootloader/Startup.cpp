#include "Startup.hpp"

// FileSystems
#include <FATFileSystem.h>

// BlockDevices
#include <SDBlockDevice.h>

Bootloader::Startup::Startup() :    m_Success(true),
                                    m_Debug(*new Debugger::Debug),
                                    m_Storage(*new Storage)
{
    // Initialise the Storage Class.
    initStorage();

    // Creates the Application last, after storage has been initialised.
    m_application = new App::Application();
}

bool Bootloader::Startup::isValid()
{
    return m_Success;
}

void Bootloader::Startup::initStorage()
{
    // Creates a pair of a BlockDevice, and a FileSystem, under a "name".
    m_Storage.create<SDBlockDevice, FATFileSystem, PE_6, PE_5, PE_2, PE_4>("local");

    // Initialise the pair with "name".
    m_Storage.initStorage("local");
}