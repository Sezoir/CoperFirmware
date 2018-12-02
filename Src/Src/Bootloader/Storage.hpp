//
// Created by Josh Mottley on 29/11/2018.
//

#ifndef COPTER_STORAGE_HPP
#define COPTER_STORAGE_HPP

#include <mbed.h>
#include <map>
#include <type_traits>
#include <string>
#include <memory>

#include "Debugger/Debug.hpp"

namespace Bootloader {
    class Storage {
    public:
        Storage();
        ~Storage();

        /**
         *  Creates a pair of a blockdevice, and a filesystem, and
         *  stored in a map, with the key being stored as a string.
         *
         * @tparam t_BlockDevice: Supported blockdevice.
         * @tparam t_FileSystem: Supported filesystem.
         * @tparam t_Pins Pin list to be passed to the BlockDevice.
         * @param t_Location: Name for accessing/identifying storage devices.
         */
        template<typename t_BlockDevice, typename t_FileSystem, PinName... t_Pins>
        void create(const char* t_test) // Note: passing in a const char* rather than std::string due to the fact the string gets destoryed,
        {                               // so its char* points to RM, which then screws the FileSystem const char* parameter.
            static_assert(std::is_base_of<BlockDevice, t_BlockDevice>::value, "Incorrect parameter type in function create: Not a BlockDevice.");
            static_assert(std::is_base_of<FileSystem, t_FileSystem>::value, "Incorrect parameter type in function create: Not a FileSystem.");

            const std::string & t_Location = t_test;
            if(!exists(t_Location)) {
                Debugger::Debug::sendMsg("Making pair.");
                Debugger::Debug::sendMsg("t_Location contains: %s", t_Location.c_str());
//                m_Storage[t_Location] = std::make_pair(std::make_shared<t_BlockDevice>(t_Pins...), std::make_shared<t_FileSystem>("local"));
                m_Storage[t_Location] = std::make_pair(std::make_shared<t_BlockDevice>(t_Pins...), std::make_shared<t_FileSystem>(t_test));
            } else Debugger::Debug::sendMsg("Error: Location already exists.");
        }

        /**
         * Deletes a storage pair.
         *
         * @param t_Location
         */
        void remove(std::string t_Location);

        /**
         * Initilises/mounts the storage device.
         *
         * @param t_Location
         */
        void initStorage(std::string t_Location);

    private:
        /**
         * Cheeks to see if Locations already exists.
         *
         * @param t_Location
         * @return: true if it exists, false if it doesn't.
         */
        bool exists(std::string t_Location);

        std::map<std::string, std::pair<std::shared_ptr<BlockDevice>, std::shared_ptr<FileSystem>>> m_Storage;
    };

} // End of namespace.

#endif //COPTER_STORAGE_HPP
