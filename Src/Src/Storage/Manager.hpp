//
// Created by Josh Mottley on 03/10/2018.
//

#ifndef COPTER_MANAGER_HPP
#define COPTER_MANAGER_HPP

#include <FATFileSystem.h>
#include <SDBlockDevice.h>

namespace Storage {
    class Manager {
    public:
        Manager();
        ~Manager();
        bool getSuccess();

    private:
        void initFileSystem();
        bool m_Success;
        FATFileSystem & m_FATFileSystem;
        SDBlockDevice & m_SDBlockDevice;
    };
}

#endif //COPTER_MANAGER_HPP
