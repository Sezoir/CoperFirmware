//
// Created by Josh Mottley on 27/08/2018.
//

#include <string>

#include "Sensors.hpp"
#include "Debugger/Debug.hpp"

App::Sensors::Sensors::Sensors(Settings::Settings & t_Settings) :   m_Settings(t_Settings.getSettings(std::string("Sensors"))),
                                                                    m_Factory(*new Factory)
{
    Debugger::Debug::sendMsg("Sensors Created.");
}

App::Sensors::Sensors::~Sensors()
{
    Debugger::Debug::sendMsg("Sensors Destroyed.");
}



