//
// Created by Josh Mottley on 11/10/2018.
//

#include "Settings.hpp"

App::Settings::Settings::Settings()
{
    Debugger::Debug::sendMsg("Settings Created");
    m_Settings["Sensors"] = std::make_shared<BaseSettings>();
}

App::Settings::Settings::~Settings()
{
    Debugger::Debug::sendMsg("Settings Destroyed");
}

std::shared_ptr<App::Settings::BaseSettings> App::Settings::Settings::getSettings(std::string t_key)
{
    return std::static_pointer_cast<App::Settings::BaseSettings>(m_Settings[t_key]);
}