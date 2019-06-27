#pragma once

// Std Libs
#include <string>

namespace App { namespace Settings { namespace Repository
{
    class RepoBase
    {
        public:

            /**
             * Construct RepoBase.
             *      - Set the m_Path member variable
             *
             * @param t_Path: Variable to be set.
             */
            RepoBase(std::string& t_Path) : m_Path(t_Path) {}

            /**
             * Destory the class
             */
            ~RepoBase() = default;

            /**
             * Read the contents of the settings file.
             *
             * @note All reposities must return setting data.
             *
             * @return char*: Content of the settings file.
             */
            virtual char * read() = 0;

        protected:
            // Path to the settings file.
            std::string m_Path;
    };

}}}
