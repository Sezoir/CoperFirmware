//
// Created by Josh Mottley on 28/09/2018.
//

#ifndef COPTER_MEDIAINTERFACE_HPP
#define COPTER_MEDIAINTERFACE_HPP

namespace App {
    namespace FileSystem {
        class MediaInterface {
        protected:
            virtual char *readFile(const char *t_systempath);

            virtual void writeFile(const char *t_systempath);

            virtual char *readFileRaw();

            virtual void writeFileRaw();

            virtual void openFileRaw(const char *t_systempath);

            virtual void closeFileRaw(const char *t_systempath);

        };
    }
}

#endif //COPTER_MEDIAINTERFACE_HPP
