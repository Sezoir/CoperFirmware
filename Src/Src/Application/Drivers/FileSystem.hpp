#pragma once

// External Lib
#include <mbed.h>

// Standard Lib
#include <string>

namespace App { namespace Drivers {

	class FileSystem
	{
		public:
			/**
			 * Constructor for FileSystem
			 */
			FileSystem() = default;

			/**
			 * Destructor for FileSystem.
			 */
			~FileSystem() = default;

			/**
			 * Read the whole content of a file at t_SystemPath, to a char*.
			 * @param t_SystemPath: Path to the file, including the name of the FileSystem given.
			 * @return char*: Whole content of a file.
			 * @example char* p = read("/local/test.json") // Where "local" is the name of the FileSystem.
			 */
			char * read(std::string t_SystemPath);

			/**
			 * @todo Write the implementation of the function.
			 */
			void write(const char * t_SystemPath);

		private:
			// Current pointer to a File.
			FILE * f;

			// Current error value.
			int err = 0;

			/**
			 * @private Open the file at the Path and points the FILE* to it.
			 * @param t_SystemPath: Path to the file, including the name of the FileSystem given.
			 */
			void open(const char * t_SystemPath);

			/**
			 * @private Closes the file pointed at by the FILE* f.
			 * @param t_SystemPath: Path to the file, including the name of the FileSystem given.
			 */
			void close(const char * t_SystemPath);
	};

}}
