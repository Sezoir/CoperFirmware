#pragma once

// Application Classes
#include "Settings/Settings.hpp"
//#include "Sensors/Sensors.hpp"

namespace App {

	class Application
	{

		public:
			/**
			 *  Constructor for Application.
			 */
			Application();

			/**
			 * Destructor for Application
			 */
			~Application() = default;

		private:
			// References to classes created at initialisation.
			Settings::Settings & m_settings;
	};

} // End of namespace
