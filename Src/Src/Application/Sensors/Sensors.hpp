//
// Created by Josh Mottley on 27/08/2018.
//

#ifndef COPTER_SENSORS_H
#define COPTER_SENSORS_H

// Std libs
#include <memory>
#include <vector>
#include <utility>

// For access to settings for sensors.
#include "Settings/Settings.hpp"

// Sensor factory.
#include "Factory.hpp"

namespace App { namespace Sensors {

	class Sensors {
		public:
			/**
			 * Constructor
			 */
			Sensors();

			/**
			 * Destructor
			 */
			~Sensors();

			/**
			 * Updates the current values of the sensors.
			 */
			void updateSensors();

			/**
			 * Recreates all the sensors in the factory based on the setting.
			 */
			void createSensors();

			enum class Type
			{
					Gyroscope,
					Accelerometer,
					Magnetometer
			};

			/**
			 * Get the value a type of sensor.
			 *
			 * @return
			 */
			std::array<int, 3> get(Type);

			std::shared_ptr<Package> getSensor()
			{
				return m_sensors[0];
			}


		private:

			std::vector<std::shared_ptr<Package>> m_sensors;
	};
}}
#endif //COPTER_SENSORS_H
