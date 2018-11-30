//
// Created by Josh Mottley on 29/11/2018.
//

#ifndef COPTER_FILESYSTEM_HPP
#define COPTER_FILESYSTEM_HPP

#include <mbed.h>
#include <string>
#include "FileSystem/Interface.hpp"

namespace App { namespace FileSystem {
        class FileSystem : public Interface
        {
        protected:
            FileSystem(std::string t_Location);
            ~FileSystem();

            char *readFile(const std::string t_SystemPath) override;

            void writeFile(const char *t_systempath) override;

            void openFile(const char *t_systempath) override;

            void closeFile(const char *t_systempath) override;

        private:
            FILE *f;
            int err = 0;
            std::string m_Location;
        };

    }
} // End of namespace

#endif //COPTER_FILESYSTEM_HPP
