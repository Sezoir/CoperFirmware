//
// Created by Josh Mottley on 24/08/2018.
//

#include "Startup.hpp"
#include "Debugger/Debug.hpp"

Bootloader::Startup::Startup() :    m_Success(true),
                                    m_Manager(*new Storage::Manager),
                                    m_Application(*new App::Application)
{
    Debugger::Debug::sendMsg("Startup Created.");
    m_Success = m_Manager.getSuccess();

}

Bootloader::Startup::~Startup()
{

    Debugger::Debug::sendMsg("Startup Destroyed.");
}

bool Bootloader::Startup::isValid()
{
    return m_Success;
}