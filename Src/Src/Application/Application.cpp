#include "Application.hpp"

// Testing
#include "Settings/Packages/MPU9250/Object.hpp"
#include <memory>

/**
 * Constructor for Application.
 */
App::Application::Application() : m_settings(*new App::Settings::Settings),
								  m_sensors(*new App::Sensors::Sensors)
{
	// Setup settings
	//Settings::Settings::load();


//	auto test = Settings::Settings::get(Settings::Settings::Type::Sensor);
//	for(auto i : test)
//	{
//		debug("%s\n", i->name.c_str());
//	}
}

