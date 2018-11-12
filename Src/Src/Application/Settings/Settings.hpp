//
// Created by Josh Mottley on 28/09/2018.
//

#ifndef COPTER_SETTINGS_HPP
#define COPTER_SETTINGS_HPP

#include <map>
#include <memory>
#include <string>
#include <Debug.hpp>
#include "Application/Settings/Repository/BaseSettings.hpp"

namespace App { namespace Settings {
        class Settings {

        public:
            Settings();
            ~Settings();

            std::shared_ptr<App::Settings::BaseSettings> getSettings(std::string t_key);

        private:
            std::map<std::string, std::shared_ptr<BaseSettings>> m_Settings;
        };
    }
}

#endif //COPTER_SETTINGS_HPP
