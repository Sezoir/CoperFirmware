//
// Created by Josh Mottley on 24/08/2018.
//

#include "Startup.hpp"
#include "Debugger/Debug.hpp"

#include <FATFileSystem.h>
#include <SDBlockDevice.h>

Bootloader::Startup::Startup() :    m_Success(true),
                                    m_Storage(*new Storage)
{
    Debugger::Debug::sendMsg("Startup Created.");

    initStorage();

    m_Application = std::make_unique<App::Application>();
}

Bootloader::Startup::~Startup()
{

    Debugger::Debug::sendMsg("Startup Destroyed.");
}

bool Bootloader::Startup::isValid()
{
    return m_Success;
}

void Bootloader::Startup::initStorage()
{
    m_Storage.create<SDBlockDevice, FATFileSystem, PE_6, PE_5, PE_2, PE_4>("local");
}