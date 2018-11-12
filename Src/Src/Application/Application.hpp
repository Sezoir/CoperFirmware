//
// Created by Josh Mottley on 24/08/2018.
//

#ifndef COPTER_APPLICATION_H
#define COPTER_APPLICATION_H

#include "Settings/Settings.hpp"
#include "Sensors/Sensors.hpp"

namespace App {

    class Application {

    public:
        Application();
        ~Application();

    private:
        Settings::Settings &m_Settings;
        Sensors::Sensors &m_Sensors;
    };

} // End of namespace
#endif //COPTER_APPLICATION_H
