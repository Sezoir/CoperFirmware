#pragma once

// Std libs
#include <memory>
#include <string>
#include <array>
#include <map>
#include <utility>
#include <typeinfo>

#include "mbed.h"

// Settings
#include "Settings.hpp"


#define IMPLEMENT_SENSOR_CLONE(TYPE) \
            std::shared_ptr<Chip> clone() override { return std::make_shared<TYPE>(); ; }

namespace App { namespace Sensors { namespace Models {

	class Chip
	{
		public:



	};


}}}
