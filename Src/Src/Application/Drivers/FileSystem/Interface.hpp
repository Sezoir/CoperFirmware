//
// Created by Josh Mottley on 28/09/2018.
//

#ifndef COPTER_MEDIAINTERFACE_HPP
#define COPTER_MEDIAINTERFACE_HPP

namespace App {
    namespace FileSystem {
        class Interface {
        protected:
            virtual char *readFile(const std::string t_SystemPath);

            virtual void writeFile(const char *t_systempath);

            virtual void openFile(const char *t_systempath);

            virtual void closeFile(const char *t_systempath);

        };
    }
}

#endif //COPTER_MEDIAINTERFACE_HPP
