//
// Created by Josh Mottley on 03/10/2018.
//

#include "Manager.hpp"
#include <Debug.hpp>

Storage::Manager::Manager() :   m_Success(true),
                                m_FATFileSystem(*new FATFileSystem("local")),
                                m_SDBlockDevice(*new SDBlockDevice(PE_6, PE_5, PE_2, PE_4))
{
    Debugger::Debug::sendMsg("Manager Created");
    initFileSystem();
}

Storage::Manager::~Manager()
{
    Debugger::Debug::sendMsg("Manager Destroyed");
}

void Storage::Manager::initFileSystem()
{
    Debugger::Debug::sendMsg("Mounting/Initialising the filesystem...");
    int err = m_FATFileSystem.mount(&m_SDBlockDevice);
    Debugger::Debug::sendMsg((err ? "Fail :(" : "OK"));

    if (err) {
        Debugger::Debug::sendMsg("No filesystem found.");
        m_Success = false;
    }
}

bool Storage::Manager::getSuccess()
{
    return m_Success;
}