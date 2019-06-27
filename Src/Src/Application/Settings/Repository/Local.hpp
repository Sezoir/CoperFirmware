#pragma once

#include "RepoBase.hpp"

namespace App { namespace Settings { namespace Repository {
	class Local : public RepoBase
	{
		public:
			/**
			 * Constructor of Local
			 * 		- Provides inherited RepoBase t_Add.
			 * @param t_Path: Path to the settings file to be read.
			 */
			Local(std::string & t_Path) : RepoBase(t_Path) {}

			/**
			 * Destructor of Local
			 */
			~Local() = default;

			/**
			 * Reads the content of the file, assuming the method is FileSystem.
			 * @return Content of the settings file.
			 */
			char * read();

	};

}}}

