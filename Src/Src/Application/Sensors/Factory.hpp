#pragma once

// Std libs
#include <map>
#include <vector>
#include <memory>
#include <string>
#include <utility>

// Setting class
#include "Settings/Settings.hpp"

// Base Sensor class
#include "Models/Chip.hpp"

// Sensor includes
#include "Application/Sensors/Models/MPU9250.hpp"

// Base Filter class
#include "Filter/BaseFilter.hpp"

// Filter includes
#include "Filter/NoFilter.hpp"
#include "Filter/Kalmin.hpp"

// Package Class
#include "Package.hpp"

#include "mbed.h"

#define MAKE_SENSOR_PROTOTYPE(TYPE) \
				addSensor(#TYPE, std::make_shared<App::Sensors::SensorModels::TYPE>());

#define MAKE_FILTER_PROTOTYPE(TYPE) \
				addFilter(#TYPE, std::make_shared<App::Sensors::Filters::TYPE>());

namespace App { namespace Sensors {
	class Factory
	{
		public:
			/**
			 * Constructor
			 */
			Factory() : m_filterSett(Settings::Settings::get(Settings::Settings::Type::Filter))
			{
				MAKE_SENSOR_PROTOTYPE(MPU9250)

				MAKE_FILTER_PROTOTYPE(NoFilter)
				MAKE_FILTER_PROTOTYPE(Kalmin)
			}

			/**
			 * Destructor
			 */
			~Factory() = default;

			/**
			 * Clears the m_sensors.
			 */
			void clear()
			{

			}

			static std::map<std::string, std::shared_ptr<SensorModels::Chip>> protoSensors;
			static std::map<std::string, std::shared_ptr<Filters::BaseFilter>> protoFilters;

			/**
			 * Creates a sensor based on parameter string, and stores shared pointer in m_sensors.
			 * @return
			 */
			std::shared_ptr<Package> create(std::shared_ptr<Settings::Packages::Object> t_Object)
			{
				std::string filterType;

				// @todo always referance in for
				for(auto i : m_filterSett)
				{
					//@todo Add in check for type
					if(i->name == t_Object->tag)
					{
						filterType = i->type;
					}
				}

				if(!exist(t_Object->type, filterType))
				{
					debug("Error: Sensor/Filter \"%s or %s\" class cannot be found.\n", t_Object->type.c_str(), t_Object->tag.c_str());
					exit(1);
				}

				auto chip = protoSensors[t_Object->type]->clone();

				SensorModels::MPU9250 a;
				//@todo change chip to a unique pointer by passing th pointer around in the verify function.
				chip->verify();
				std::unique_ptr<Filters::BaseFilter> filter;
				//std::unique_ptr<SensorModels::Chip> t_Chip = std::move(Chip);
				std::shared_ptr<Package> package = make_shared<Package>(chip);

				for(auto i : chip->getOvr())
				{
					if(i.second == true)
					{
						debug("%s\n", i.first.c_str());
						if(t_Object->tag.empty())
						{
							filter = protoFilters["NoFilter"]->clone();
						}
						else
						{
							filter = protoFilters[filterType]->clone();
						}
						package->addFilter(i.first, std::move(filter));
					}
				}

				return package;
			}

		private:
			std::vector<std::shared_ptr<Settings::Packages::Object>> m_filterSett;

			static void addSensor(std::string t_Type, std::shared_ptr<SensorModels::Chip> t_Chip)
			{
				protoSensors[t_Type] = t_Chip;
				debug("Added %s to sensor prototable.\n", t_Type.c_str());
			}

			static void addFilter(std::string t_Type, std::shared_ptr<Filters::BaseFilter> t_Filter)
			{
				protoFilters[t_Type] = t_Filter;
				debug("Added %s to filter prototable.\n", t_Type.c_str());
			}


			bool exist(std::string t_Type, std::string t_Filter)
			{
				if((protoSensors.find(t_Type) != protoSensors.end() &&
					((protoFilters.find(t_Filter) != protoFilters.end()) || t_Filter == "")))
				{
					return true;
				}
				return false;
			}
	};

}}