//
// Created by Josh Mottley on 24/08/2018.
//

#include "Application.hpp"
#include "Debugger/Debug.hpp"

#include <memory>
//#include "Settings/Repository/Sensor/Settings.hpp"

App::Application::Application() :   m_Settings(*new App::Settings::Settings),
                                    m_Sensors(*new App::Sensors::Sensors(m_Settings))
{
    Debugger::Debug::sendMsg("Application Created.");
    //std::shared_ptr<Settings::Sensor::Settings> temp = m_Settings.getSettings<Settings::Sensor::Settings>("Sensors");
    //Debugger::Debug::sendMsg(temp->getTest().c_str());
}

App::Application::~Application()
{
    Debugger::Debug::sendMsg("Application Destroyed.");
}

