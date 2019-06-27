//
// Created by Josh Mottley on 27/08/2018.
//

#ifndef COPTER_SENSORS_H
#define COPTER_SENSORS_H

#include <memory>
#include "Settings/Settings.hpp"
#include "Application/Sensors/Factory/Factory.hpp"

namespace App { namespace Sensors {

        class Sensors {
        public:
            Sensors(Settings::Settings &);
            ~Sensors();
//            void updateSensors;
//            void getSensor;
//            void getAccel;
//            void getGyro;
//            void getMag;
//            void getAlt;

        private:
            //std::shared_ptr<App::Settings::BaseReader>  m_Settings;
            //Factory & m_Factory;

        };
    }}
#endif //COPTER_SENSORS_H
