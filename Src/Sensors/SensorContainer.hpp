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
            std::array<int, 3> avrAccel{0, 0, 0};
            for(const SensorSetup& setup : mSensors)
            {
                if(setup.type.accelerometer)
                {
                    std::array<int, 3> accel = setup.sensor->readAccel();
                    for(uint8_t i = 0; i < 3; i++)
                        avrAccel[i] += accel[i];
                    cnt++;
                }
            }
            for(int& value : avrAccel)
                value /= cnt;
            return avrAccel;
        }

        /**
         * Reads all stored gyroscopes and returns the average change in the angle.
         * @return std::array<int, 3>: Gyroscope change in (x,y,z).
         * @todo: Add conversion between gyroscopes (as they may have different resolutions).
         */
        [[nodiscard]] std::array<int, 3> readGyro() const
        {
            uint8_t cnt = 0;
            std::array<int, 3> avrGyro{0, 0, 0};
            for(const SensorSetup& setup : mSensors)
            {
                if(setup.type.gyroscope)
                {
                    std::array<int, 3> gyro = setup.sensor->readGyro();
                    for(uint8_t i = 0; i < 3; i++)
                        avrGyro[i] += gyro[i];
                    cnt++;
                }
            }
            for(int& value : avrGyro)
                value /= cnt;
            return avrGyro;
        }

        /**
         * Reads all stored magnetometers and returns the average magnetism.
         * @return std::array<int, 3>: Magnetism in the (x,y,z).
         * @todo: Add conversion between magnetometers (as they may have different resolutions).
         */
        [[nodiscard]] std::array<int, 3> readMag() const
        {
            uint8_t cnt = 0;
            std::array<int, 3> avrMag{0, 0, 0};
            for(const SensorSetup& setup : mSensors)
            {
                if(setup.type.magnetometer)
                {
                    std::array<int, 3> mag = setup.sensor->readMag();
                    for(uint8_t i = 0; i < 3; i++)
                        avrMag[i] += mag[i];
                    cnt++;
                }
            }
            for(int& value : avrMag)
                value /= cnt;
            return avrMag;
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
            for(const SensorSetup& setup : mSensors)
            {
                if(setup.type.temperature)
                {
                    temp += setup.sensor.readTemp();
                    cnt++;
                }
            }
            return temp / cnt;
        }

    private:
        /**
         * Calls delete on all the SensorInterface* that was new'ed. Also sets the pointer afterwards to nullptr.
         */
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
