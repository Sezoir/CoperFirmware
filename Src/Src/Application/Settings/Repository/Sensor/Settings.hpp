//
// Created by Josh Mottley on 29/09/2018.
//

#ifndef COPTER_SENSORSETTINGS_HPP
#define COPTER_SENSORSETTINGS_HPP

#include <string>
#include <ArduinoJson-v5.13.2.hpp>
#include "FileSystem/SDFATSystem/SDFatSystem.hpp"
#include "Repository/BaseSettings.hpp"

namespace App {
    namespace Settings {
        namespace Sensor {
            class Settings : public FileSystem::SDFatSystem, public BaseSettings
                    {
            private:
                ArduinoJson::DynamicJsonBuffer &m_DynJsonBuffer;
                ArduinoJson::JsonObject &m_JsonObject;

            public:
                Settings();
                ~Settings();
                std::string getTest();

            };
        }
    }
}
#endif //COPTER_SENSORSETTINGS_HPP
