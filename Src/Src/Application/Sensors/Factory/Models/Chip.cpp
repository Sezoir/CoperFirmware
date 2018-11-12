//
// Created by Josh Mottley on 27/08/2018.
//

#include "Chip.hpp"
#include "Debugger/Debug.hpp"

App::Sensors::SensorModels::Chip::Chip()
{
    Debugger::Debug::sendMsg("Chip Created.");
};

App::Sensors::SensorModels::Chip::~Chip()
{
    Debugger::Debug::sendMsg("Chip Destroyed.");
};
