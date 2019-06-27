#include "Application.hpp"

// Testing
#include "Settings/Packages/MPU9250/Object.hpp"
#include <memory>

/**
 * Constructor for Application.
 */
App::Application::Application() : m_settings(*new App::Settings::Settings)
{
	debug("Application created.\n");

	// Setup settings
	m_settings.load();

//	auto test = m_settings.get<Settings::Packages::MPU9250::ObjMPU9250>("left_wing");
//	std::string t = test->id;
//	debug("%s\n", t.c_str());
}

