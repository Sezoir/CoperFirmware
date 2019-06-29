#pragma once

// std libs
#include <string>

namespace App { namespace Settings { namespace Packages {
/**
 * Basic Object class to be inherited by Settings Packages Objects.
 */
	struct Object
	{
		std::string name;
		std::string method;
		std::string type;
		std::string address;
		std::string group;
	};
}}}
