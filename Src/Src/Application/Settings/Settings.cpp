#include "Settings.hpp"

// Ext libs
#include <ArduinoJson-v5.13.2.hpp>

//Drivers
#include "Drivers/FileSystem.hpp"

// Packages
#include "Packages/Parser.hpp"

// Include all settings packages
#include "Packages/MPU9250/MPU9250.hpp"

/**
 * Construct the settings class
 *      - Registers all settings packages
 */
App::Settings::Settings::Settings()
{
	// Register all settings packages
	MAKE_SETTINGS_PROTOTYPE(MPU9250);
}

/**
 * Reads the init json file from m_InitLocation.
 * Then it creates and stores all the objects specified in the init.json file in m_Settings.
 */
void App::Settings::Settings::load()
{
	// Create instance of file system
	Drivers::FileSystem system;
	//auto system = Drivers::FileSystem();

	// Read the data from the file
	auto initContent = system.read(m_initLocation);

	// @todo validate initContent

	// Format the data
	format(initContent);
}


/**
 * Creates a parser class of each settings specified from t_Data and then stores settings as a struct in m_Settings.
 */
void App::Settings::Settings::format(char * t_data)
{
	// Create a buffer for json
	ArduinoJson::DynamicJsonBuffer jsonBuff;

	// Create an array from json
	ArduinoJson::JsonArray & jsonInit = jsonBuff.parseArray(t_data);

	// Loop through all settings
	for (auto i : jsonInit) {
		// Name of setting
		std::string name = i["id"].as<std::string>();
		std::string method = i["method"].as<std::string>();
		std::string type = i["type"].as<std::string>();
		std::string address = i["address"].as<std::string>();

		// Fill strut
		char * data = repoData(method, address);

		// Call parser
		auto obj = packageSettings(type, data);

		// Check if id already used.
		auto it = m_settings.find(name);
		if(it != m_settings.end())
		{
			debug("Error: There is more than 1 sub settings of id: %s", name.c_str());
			exit(1);
		}

		// Insert to map
		m_settings.insert(std::pair<std::string, std::shared_ptr<Packages::Object>>(name, obj));
	}
}

/**
 * Get the package object settings
 *
 *      - @todo return package object
 *
 * @param data
 * @return
 */
//App::Settings::Packages::Object * App::Settings::Settings::packageSettings(std::string type, char * data)
std::shared_ptr<App::Settings::Packages::Object> App::Settings::Settings::packageSettings(std::string type, char * data)
{
	auto p = Packages::Parser::make(type);

	p->set(data);

	if(!(p->validate()))
	{
		debug("Error with validation of setting type: %s\n", type.c_str());
		exit(1);
		// @todo change to mbed error.
	}

	return p->get();
}


/**
 * Reads content of the Settings file specified at t_Address using method t_Type.
 */
char * App::Settings::Settings::repoData(std::string t_Type, std::string t_Address)
{
	if(t_Type == "Local")
	{
		// Get instance of local
		Repository::Local i(t_Address);

		// Read the settings data
		return i.read();
	}
	else
	{
		debug("Error: No valid setting type called %s with address %s\n", t_Type.c_str(), t_Address.c_str());
		exit(1);
	}
}


