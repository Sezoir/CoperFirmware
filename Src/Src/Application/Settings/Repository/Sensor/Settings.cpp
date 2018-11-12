//
// Created by Josh Mottley on 01/10/2018.
//

#include "Settings.hpp"
#include <Debug.hpp>


App::Settings::Sensor::Settings::Settings() : m_DynJsonBuffer(*new ArduinoJson::DynamicJsonBuffer),
                                                  m_JsonObject(m_DynJsonBuffer.parseObject(readFile("/local/test.json")))
{
    Debugger::Debug::sendMsg("Settings Created.");

    Debugger::Debug::sendMsg("Parsing Json");
    if(m_JsonObject.success())
    {

    }
    else
    {
        Debugger::Debug::sendMsg("Error: Failed to parse");
    }
}

App::Settings::Sensor::Settings::~Settings()
{
    Debugger::Debug::sendMsg("Settings Destroyed");
}

std::string App::Settings::Sensor::Settings::getTest()
{
    return m_JsonObject["test"];
}