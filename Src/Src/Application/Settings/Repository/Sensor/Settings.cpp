//
// Created by Josh Mottley on 01/10/2018.
//

#include "Settings.hpp"
#include <Debug.hpp>


App::Settings::Sensor::Settings::Settings()
{
    Debugger::Debug::sendMsg("Settings Created.");

    Debugger::Debug::sendMsg("Parsing Json.");
    if(updateSettings())
    {
        Debugger::Debug::sendMsg("Json Parsed.");
    }
    else
    {
        Debugger::Debug::sendMsg("Error: Failed to parse.");
    }
}

bool App::Settings::Sensor::Settings::updateSettings()
{
    m_Settings.clear();

    ArduinoJson::DynamicJsonBuffer dynBuffer;
    ArduinoJson::JsonObject & jsonObt = dynBuffer.parseObject(readFile("/local/test.json"));

    if(jsonObt.success()) {
        ArduinoJson::JsonObject &Id = jsonObt["Sensors"]["Factory"];
        for (auto i : Id) {
            if (validateJson(Id, "PinSda")) {
                m_Settings[Id["id"].as<int>()]["PinSda"] = Id["Parameters"]["PinSda"].as<std::string>();
            }
            if (validateJson(Id, "PinScl")) {
                m_Settings[Id["id"].as<int>()]["PinScl"] = Id["Parameters"]["PinScl"].as<std::string>();
            }
            if (validateJson(Id, "Address")) {
                m_Settings[Id["id"].as<int>()]["Address"] = Id["Parameters"]["Address"].as<std::string>();
            }
            if (validateJson(Id, "GyroScale")) {
                m_Settings[Id["id"].as<int>()]["GyroScale"] = Id["Parameters"]["GyroScale"].as<std::string>();
            }
            if (validateJson(Id, "AccelScale")) {
                m_Settings[Id["id"].as<int>()]["AccelScale"] = Id["Parameters"]["AccelScale"].as<std::string>();
            }
            if (validateJson(Id, "ClkSelect")) {
                m_Settings[Id["id"].as<int>()]["ClkSelect"] = Id["Parameters"]["ClkSelect"].as<std::string>();
            }
            if (validateJson(Id, "GyroTempDigiFilter")) {
                m_Settings[Id["id"].as<int>()]["GyroTempDigiFilter"] = Id["Parameters"]["GyroTempDigiFilter"].as<std::string>();
            }
            if (validateJson(Id, "AccelDigiFilter")) {
                m_Settings[Id["id"].as<int>()]["AccelDigiFilter"] = Id["Parameters"]["AccelDigiFilter"].as<std::string>();
            }
            if (validateJson(Id, "GyroScaleRange")) {
                m_Settings[Id["id"].as<int>()]["GyroScaleRange"] = Id["Parameters"]["GyroScaleRange"].as<std::string>();
            }
            if (validateJson(Id, "AccelScaleRange")) {
                m_Settings[Id["id"].as<int>()]["AccelScaleRange"] = Id["Parameters"]["AccelScaleRange"].as<std::string>();
            }
//            if (validateJson(Id, "")) {
//                m_Settings[Id["id"].as<int>()][""] = Id["Parameters"][""].as<std::string>();
//            }
        }
        return true;
    }
    else
    {
        Debugger::Debug::sendMsg("Invalid Json structure.");
    }
}

bool App::Settings::Sensor::Settings::validateJson(ArduinoJson::JsonObject & t_Obj, std::string t_Key)
{
    ArduinoJson::JsonVariant error = t_Obj[t_Key];
    if(error.success())
    {
        return true;
    } else return false;
}


std::map<std::string, std::string> App::Settings::Sensor::Settings::getSettings(int t_ID)
{
    return m_Settings[t_ID];
}

App::Settings::Sensor::Settings::~Settings()
{
    Debugger::Debug::sendMsg("Settings Destroyed.");
}