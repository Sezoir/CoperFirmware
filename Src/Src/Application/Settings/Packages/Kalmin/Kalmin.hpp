#pragma once

#pragma once

// Std libs
#include <map>
#include <string>
#include <memory>

// Drivers
#include <Debug.hpp>

// Default struct
#include "Object.hpp"

// Parser table
#include "Packages/Parser.hpp"


namespace App { namespace Settings { namespace Packages { namespace Kalmin
{

	class Kalmin: public Packages::Parser
	{

		public:
			Kalmin()
			{

			}

			~Kalmin()
			{

			}

			std::shared_ptr<Object> get() override
			{
				auto t = make_shared<ObjKalmin>();
				t->frequency = 1250;
				return t;
			}

			IMPLEMENT_SETTING_CLONE(Kalmin);
	};

}}}}



