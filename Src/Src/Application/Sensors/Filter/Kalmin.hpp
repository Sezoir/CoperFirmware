#pragma once

// Std libs
#include <memory>

// Base Filter
#include "BaseFilter.hpp"

// Mbed
#include "mbed.h"

namespace App { namespace Sensors { namespace Filters {
	class Kalmin : public BaseFilter
	{
		public:
			Kalmin() {debug("Kalmin created.\n");}
			~Kalmin() override = default;

			IMPLEMENT_FILTER_CLONE(Kalmin)

		private:

	};
}}}

