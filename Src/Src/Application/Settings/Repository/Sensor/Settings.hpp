//
// Created by Josh Mottley on 29/09/2018.
//

#ifndef COPTER_SENSORSETTINGS_HPP
#define COPTER_SENSORSETTINGS_HPP

#include <string>
#include <map>
#include <ArduinoJson-v5.13.2.hpp>
#include "FileSystem/SDFATSystem/SDFatSystem.hpp"
#include "Repository/BaseSettings.hpp"

namespace App {
    namespace Settings {
        namespace Sensor {
            class Settings : public FileSystem::SDFatSystem, public BaseSettings
            {
            public:
                Settings();
                ~Settings();

                bool updateSettings();
                static std::map<std::string, std::string> getSettings(int t_ID);

            private:
                static bool validateJson(ArduinoJson::JsonObject & t_Obj, std::string t_Key); // Using this method over the containskey() can lead to faster code.
                static std::map<int, std::map<std::string, std::string>> m_Settings;
            };
        }
    }
}
#endif //COPTER_SENSORSETTINGS_HPP
