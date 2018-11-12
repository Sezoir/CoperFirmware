//
// Created by Josh Mottley on 29/09/2018.
//

#ifndef COPTER_SDFATSYSTEM_HPP
#define COPTER_SDFATSYSTEM_HPP
#include "FileSystem/MediaInterface.hpp"
#include <FATFileSystem.h>

namespace App {
    namespace FileSystem {
        class SDFatSystem : public MediaInterface {
        public:
            SDFatSystem();
            ~SDFatSystem();

        protected:
            char *readFile(const char *t_systempath) override;

            void writeFile(const char *t_systempath) override;

            char *readFileRaw() override;

            void writeFileRaw() override;

            void openFileRaw(const char *t_systempath) override;

            void closeFileRaw(const char *t_systempath) override;

            FILE *f;
            int err = 0;
        };
    }
}

#endif //COPTER_SDFATSYSTEM_HPP
