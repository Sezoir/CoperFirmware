// Std libs
#include <string>

// Hpp
#include "Sensors.hpp"

// Settings
#include "Settings.hpp"

App::Sensors::Sensors::Sensors()
{
	createSensors();
}

App::Sensors::Sensors::~Sensors()
{

}

void App::Sensors::Sensors::updateSensors()
{

}

std::array<int, 3> App::Sensors::Sensors::get(App::Sensors::Sensors::Type t_type)
{
	if(t_type == App::Sensors::Sensors::Type::Accelerometer)
	{

	}
	else if(t_type == App::Sensors::Sensors::Type::Gyroscope)
	{

	}
	else if(t_type == App::Sensors::Sensors::Type::Magnetometer)
	{

	}

	return std::array<int, 3>{};
}

void App::Sensors::Sensors::createSensors()
{
	// Get vector of sensor setting structs.
	auto sensorSett = Settings::Settings::get(Settings::Settings::Type::Sensor);

	// Create Factory to create sensors packages.
	auto factory = Factory();

	// Clears current Sensors.
	m_sensors.clear();

	// Iterate through each setting, and create a package from setting struct and store the package.
	for(auto i : sensorSett)
	{
		m_sensors.emplace_back(factory.create(i));
	}
}



