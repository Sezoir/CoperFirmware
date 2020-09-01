#pragma once
// Std libs
#include <array>
// External libs
#include <mbed.h>
// Project files
#include "SensorInterface.hpp"

namespace Copter::Sensors
{
    template <uint8_t COUNT>
    class SensorContainer
    {
    public:
        /**
         * @brief: Default constructor.
         */
        SensorContainer() = default;

        /**
         * @brief: Default deconstructor. Also makes sure to call delete on sensor objects.
         */
        ~SensorContainer()
        {
            removeSensors();
        }

        /**
         * @brief: Adds a sensor setup to the internal array. The setup contains a sensor class pointer and SensorType
         * struct describing the type of sensor (e.g: gyroscope, etc). Note that the amount of setups you can add must
         * be less than or equal to the COUNT given with the initial constructor.
         * @tparam: SENSOR: The sensor class to add to the container. Note that the sensor class must inherit the
         * SensorInterface class.
         * @tparam: ARGS: The ARGS type to be passed to the sensor class.
         * @param: args: The arguments to pass to the sensor class.
         */
        template <typename SENSOR, typename... ARGS>
        void addSensor(ARGS... args)
        {
            SensorSetup sensorSetup;
            sensorSetup.sensor = std::move(new SENSOR(args...));
            sensorSetup.type = sensorSetup.sensor->getType();
            mSensors[mSensorCount] = std::move(sensorSetup);
            mSensorCount++;
        }

        /**
         * @brief: Initialises each sensor in the container.
         */
        void init()
        {
            for(SensorSetup& setup : mSensors)
            {
                setup.sensor->init();
            }
        }

        /**
         * Reads all stored accelerometers and returns the average change in the acceleration.
         * @return std::array<int, 3>: Acceleration in the (x,y,z).
         * @todo: Add conversion between accelerometers (as they may have different resolutions).
         */
        [[nodiscard]] std::array<int, 3> readAccel() const
        {
            uint8_t cnt = 0;
            std::array<int, 3> accel{0, 0, 0};
            for(SensorSetup& setup : mSensors)
            {
                if(setup.type.accelerometer)
                {
                    accel += setup.sensor.readAccel();
                    cnt++;
                }
            }
            for(int& value : accel)
                value / cnt;
            return accel;
        }

        /**
         * Reads all stored gyroscopes and returns the average change in the angle.
         * @return std::array<int, 3>: Gyroscope change in (x,y,z).
         * @todo: Add conversion between gyroscopes (as they may have different resolutions).
         */
        [[nodiscard]] std::array<int, 3> readGyro() const
        {
            uint8_t cnt = 0;
            std::array<int, 3> gyro{0, 0, 0};
            for(SensorSetup& setup : mSensors)
            {
                if(setup.type.accelerometer)
                {
                    gyro += setup.sensor.readGyro();
                    cnt++;
                }
            }
            for(int& value : gyro)
                value / cnt;
            return gyro;
        }

        /**
         * Reads all stored magnetometers and returns the average magnetism.
         * @return std::array<int, 3>: Magnetism in the (x,y,z).
         * @todo: Add conversion between magnetometers (as they may have different resolutions).
         */
        [[nodiscard]] std::array<int, 3> readMag() const
        {
            uint8_t cnt = 0;
            std::array<int, 3> mag{0, 0, 0};
            for(SensorSetup& setup : mSensors)
            {
                if(setup.type.accelerometer)
                {
                    mag += setup.sensor.readMag();
                    cnt++;
                }
            }
            for(int& value : mag)
                value / cnt;
            return mag;
        }

        /**
         * Reads all stored temperature sensors and returns the average temperature.
         * @return int: Temperature.
         * @todo: Add conversion between temperature sensors (as they may have different units).
         */
        [[nodiscard]] int readTemp() const
        {
            uint8_t cnt = 0;
            int temp = 0;
            for(SensorSetup& setup : mSensors)
            {
                if(setup.type.accelerometer)
                {
                    temp += setup.sensor.readTemp();
                    cnt++;
                }
            }
            return temp / cnt;
        }

    private:
        void removeSensors()
        {
            for(SensorSetup& setup : mSensors)
            {
                delete setup.sensor;
                setup.sensor = nullptr;
            }
        }

        // Setup for each sensor
        struct SensorSetup
        {
            SensorInterface* sensor = nullptr;
            SensorInterface::SensorType type = SensorInterface::SensorType();
        };
        // Current count of sensors
        uint8_t mSensorCount = 0;

        // Contains all sensor setups
        std::array<SensorSetup, COUNT> mSensors = {};
    };
} // namespace Copter::Sensors
