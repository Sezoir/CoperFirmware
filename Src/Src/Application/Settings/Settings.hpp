#pragma once

// Std libs
#include <map>
#include <memory>
#include <string>
#include <Debug.hpp>

// Repos
#include "Repository/RepoBase.hpp"
#include "Repository/Local.hpp"

// Packages
#include "Packages/Parser.hpp"
#include "Packages/Object.hpp"


namespace App { namespace Settings {
	class Settings
	{
		public:
			/**
 		 	* Constructor for Settings, and also:
 		 	*      - Registers all settings packages.
 		 	*/
			Settings();

			/**
			 * Destructor for Settings.
			 */
			~Settings() = default;

			/**
			 * Reads the init json file from m_InitLocation.
			 * Then it creates and stores all the objects specified in the init.json file in m_Settings.
			 */
			void load();

			/**
			 * Gets a reference to a setting struct.
			 * @todo Comments
			 * @return
			 */
			template<typename T>
			std::shared_ptr<T> get(std::string t_Id)
			{
				auto it = m_settings.find(t_Id);
				if(it == m_settings.end())
				{
					debug("Error: Cannot find setting with id: %s", t_Id.c_str());
					exit(1);
				}

				return std::static_pointer_cast<T>(m_settings[t_Id]);
			}

		private:
			// Init location
			std::string m_initLocation = "/local/init.json";

			// Settings container
			std::map<std::string, std::shared_ptr<Packages::Object>> m_settings;

			// Functions

			/**
			 * Creates a parser class of each settings specified from t_Data and then stores settings as a struct in m_Settings.
			 *
			 * Parses t_Data in a JsonObject. Then:
			 * 	- Iterates through the JsonObject (i.e through the settings that want to be created),
			 * 		- Upon each iteration, pulls and out all the data from JsonObject,
			 * 		- Reads settings file based on the type and address given by Init.json.
			 * 		- Then creates a parser of the correct type, and passes in the "type" and content from file read.
			 *
			 * @param t_data: Content to Init.json to parse.
			 */
			void format(char * t_data);

			/**
			 * Reads content of the Settings file specified at t_Address using method t_Type.
			 *
			 * @param t_Type: How the file is accessed. (e.g: Ethernet, Local, etc)
			 * @param t_Address: Address of file to read.
			 * @return char*: Content of the settings file.
			 */
			char * repoData(std::string t_Type, std::string t_Address);

			// @todo
			//Packages::Object * packageSettings(std::string type, char * data);
			/**
			 * Creates a parser object, and then reads the Object struct attached to it.
			 *
			 * @param type
			 * @param data
			 * @return Packages::Object struct: Struct with setting.
			 */
			std::shared_ptr<App::Settings::Packages::Object> packageSettings(std::string type, char * data);

	};
}} // End of namespace
