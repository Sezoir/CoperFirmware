#include "Factory.hpp"

std::map<std::string, std::shared_ptr<App::Sensors::Models::Chip>> App::Sensors::Factory::protoSensors;

std::map<std::string, std::shared_ptr<App::Sensors::Filters::BaseFilter>> App::Sensors::Factory::protoFilters;