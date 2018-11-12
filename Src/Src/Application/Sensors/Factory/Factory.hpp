#ifndef COPTER_SENSORSFACTORY_H
#define COPTER_SENSORSFACTORY_H

#include <memory>
#include <map>
#include "Application/Sensors/Factory/Models/Chip.hpp"
#include "Debugger/Debug.hpp"

namespace App { namespace Sensors {
        class Factory
        {
        public:
            enum class Type {
                IMU,
                GPS,
                Barometer,
                Temperature,
                Sonar,
                Laser
            };

            /**
             * Constructor for SensorFactory
             */
            Factory()
            {
                Debugger::Debug::sendMsg("Factory Created.");
            }

            /**
             * Destructor for SensorFactory
             */
            ~Factory()
            {
                Debugger::Debug::sendMsg("Factory Destroyed.");
            }

            /**
             * Create a new sensor instance
            */
            template<typename T>
            int create(Type type)
            {
                // Increment one from catergory size
                int count = m_sensors[type].size() + 1;

                // Create sensors as specified
                auto i = std::make_shared<T>();
                m_sensors[type][count] =  i;

                // Return the instance reference
                return count;
            }

            /**
            * Destroy an instance of an sensor
            */
            void destroy(Type type, int id)
            {
                // Check exists
                exists(type, id);

                // Release ownership
                // m_sensors[type][id].reset();

                // Erase the entry
                m_sensors[type].erase(id);
            }

            /**
            * Return a sensors object
            *
            * @param Type supported sensor type
            * @param int id of the sensor
            * @return typename sensor class
            */
            template<typename T>
            std::shared_ptr<T> get(Type type, int id)
            {
                // Check exists
                exists(type, id);

                // Turn sensor
                return std::static_pointer_cast<T>(m_sensors[type][id]);
            }

            /**
	        * Setup the sensor with the correct number of parameters
	        */
            template<typename T, typename... Args>
            void setup(Type type, int id, Args... args)
            {
                // Check exists
                exists(type, id);

                // Get sensor
                std::shared_ptr<T> i = get<T>(type, id);

                // Setup sensor
                i->setup(args...);
            }

            /**
             * Creates and sets up instance of sensor
             */
            template<typename T, typename... Args>
            int build(Type type, Args... args)
            {
                // Create sensor
                int id = create<T>(type);

                // Setup sensor
                setup<T>(type, id, args...);

                // Return id
                return id;
            }

        private:
            std::map<Type, std::map<int, std::shared_ptr<App::Sensors::SensorModels::Chip>>> m_sensors;

            /**
            * Check ID exists
            */
            void exists(Type type, int id)
            {
                auto container = m_sensors[type];

                if (container.find(id) == container.end())
                {
                    Debugger::Debug::sendMsg("Sensor ID does not exists");
                }
            }
        };

    }} // End of namespace
#endif //COPTER_SENSORSFACTORY_H
