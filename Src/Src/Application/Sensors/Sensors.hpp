//
// Created by Josh Mottley on 27/08/2018.
//

#ifndef COPTER_SENSORS_H
#define COPTER_SENSORS_H

// Std libs
#include <memory>
#include <vector>

// For access to settings for sensors.
#include "Settings/Settings.hpp"

// Sensor factory.
#include "Application/Sensors/Factory/Factory.hpp"

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
			int get(Type);

		private:
			//Factory & m_Factory;


	};
}}
#endif //COPTER_SENSORS_H
