//
// Created by Josh Mottley on 27/08/2018.
//

#ifndef COPTER_CHIP_H
#define COPTER_CHIP_H

#include <memory>
#include <string>
#include <map>

namespace App { namespace Sensors { namespace SensorModels {

            class Chip
            {
            public:
                virtual std::shared_ptr<Chip> Create(std::map<std::string, uint8_t>);
                Chip();
                ~Chip();

            private:
                virtual bool Validate();
            };


        }}
}


#endif //COPTER_CHIP_H
