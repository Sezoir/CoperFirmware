#pragma once

// Std libs
#include <map>
#include <memory>
#include <string>
#include <Debug.hpp>
#include <vector>

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
			static void load();

			/**
			 *	Enum to identify which setting belongs to which class.
			 */
			enum class Type
			{
					Sensor,
					Motor,
					Pilot
			};

			/**
			 * Searches the m_settings for all setting structs that belong to that type.
			 *
			 * @param t_Type: Enum type you want to search for.
			 * @return: Vector of all structs of the type from parameter.
			 */
			static std::vector<std::shared_ptr<Packages::Object>> get(Type t_Type);

			/**
			 *
			 */
			template<typename T>
			static T as(std::shared_ptr<Packages::Object> t_Obj)
			{
				return static_pointer_cast<T>(t_Obj);
			}

			/**
			 * Gets a reference to a setting struct.
			 * @todo Comments
			 * @return
			 */
			template<typename T>
			static std::shared_ptr<T> getSetting(std::string t_Id)
			{
				auto it = findId(t_Id);

				if(it == m_settings.end())
				{
					debug("Error: Cannot find setting with id: %s", t_Id.c_str());
					exit(1);
				}

				return std::static_pointer_cast<T>(*it);
			}

//			template<typename T>
//			std::shared_ptr<subSetting> get(std::string t_Id)
//			{
//				auto it = findId(t_Id);
//
//				if(it == m_settings.end())
//				{
//					debug("Error: Cannot find setting with id: %s", t_Id.c_str());
//					exit(1);
//				}
//
//				(*it)->setting = std::static_pointer_cast<T>((*it)->setting);
//
//				return (*it);
//				//return std::static_pointer_cast<T>(m_settings[t_Id]);
//			}

		private:
			// Init location
			static const std::string m_initLocation;

			// Settings container
			static std::vector<std::shared_ptr<Packages::Object>> m_settings;

			// Functions

			/**
			 *  Searches m_settings for a struct with the id as the same as the parameter string.
			 *
			 *  @return: True if there exist a struct with the id the same as parameter.
			 *  		 False otherwise.
			 */
			static std::vector<std::shared_ptr<Packages::Object>>::iterator findId(std::string);


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
			static void format(char * t_data);

			/**
			 * Reads content of the Settings file specified at t_Address using method t_Type.
			 *
			 * @param t_Type: How the file is accessed. (e.g: Ethernet, Local, etc)
			 * @param t_Address: Address of file to read.
			 * @return char*: Content of the settings file.
			 */
			static char * repoData(std::string t_Type, std::string t_Address);

			// @todo
			//Packages::Object * packageSettings(std::string type, char * data);
			/**
			 * Creates a parser object, and then reads the Object struct attached to it.
			 *
			 * @param type
			 * @param data
			 * @return Packages::Object struct: Struct with setting.
			 */
			static std::shared_ptr<App::Settings::Packages::Object> packageSettings(std::string type, char * data);

	};
}} // End of namespace
