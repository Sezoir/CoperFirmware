#pragma once

#include "Packages/Object.hpp"

#include <string>

namespace App { namespace Settings { namespace Packages { namespace MPU9250 {
	struct ObjMPU9250 : public App::Settings::Packages::Object
	{
		std::string id;
	};
}}}}
