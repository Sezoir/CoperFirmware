#pragma once

// Std libs
#include <memory>
#include <string>

// Debug
#include "mbed.h"

#define IMPLEMENT_FILTER_CLONE(TYPE) \
            std::unique_ptr<BaseFilter> clone() override { return std::make_unique<TYPE>(); }

namespace App { namespace Sensors { namespace Filters {
	class BaseFilter
	{
		public:
		BaseFilter() = default;
		virtual ~BaseFilter() = default;

		virtual std::unique_ptr<BaseFilter> clone() = 0;

		private:

	};

}}} // End of namespace
