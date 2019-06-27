#pragma once

// Extern Lib
#include <mbed.h>

// Std Lib
#include <map>
#include <type_traits>
#include <string>
#include <memory>

namespace Bootloader {
    class Storage {
    public:
        Storage() = default;
        ~Storage() = default;

        /**
         *  Creates a pair of a blockdevice, and a filesystem, and
         *  stored in a map, with the key being stored as a string.
         *
         * @tparam t_BlockDevice: Supported blockdevice.
         * @tparam t_FileSystem: Supported filesystem.
         * @tparam t_Pins: Pin list to be passed to the BlockDevice.
         * @param t_Location: Name for accessing/identifying storage devices.
         */
        template<typename t_BlockDevice, typename t_FileSystem, PinName... t_Pins>
        void create(const char* t_Name) // Note: passing in a const char* rather than std::string due to the fact the string gets destoryed,
        {                               // so its char* points to RM, which then screws the FileSystem const char* parameter.
            // Checks for correct class provided in template.
            static_assert(std::is_base_of<BlockDevice, t_BlockDevice>::value, "Incorrect parameter type in function create: Not a BlockDevice.");
            static_assert(std::is_base_of<FileSystem, t_FileSystem>::value, "Incorrect parameter type in function create: Not a FileSystem.");

            const std::string & t_Location = t_Name;

            // Checks for existence of the t_Name parameter in the map m_Storage.
            if(!exists(t_Location)) {
                // Creates new key, value in the map.
                m_Storage[t_Location] = std::make_pair(std::make_unique<t_BlockDevice>(t_Pins...), std::make_unique<t_FileSystem>(t_Name));
            } else MBED_ERROR(MBED_MAKE_ERROR(MBED_MODULE_APPLICATION, MBED_ERROR_CODE_EEXIST),"Error: Location already exists.");
        }

        /**
         * Deletes a storage pair.
         *
         * @param t_Location: std::string of the "name" assigned to the pair.
         */
        void remove(std::string t_Location);

        /**
         * Checks for existance and mounts the storage device.
         *
         * @param t_Location: std::string of the "name" assigned to the pair.
         */
        void initStorage(std::string t_Location);

    private:
        /**
         * Cheeks to see if Location already exists.
         *
         * @param t_Location: std::string of the "name" assigned to the pair.
         * @return bool: True if it exists, false if it doesn't.
         */
        bool exists(std::string t_Location);

        /**
         * Maps a std::string ("name") to a pair containing, 2 unique ptr pointing to a BlockDevice and a Filesystem.
         */
        std::map<std::string, std::pair<std::unique_ptr<BlockDevice>, std::unique_ptr<FileSystem>>> m_Storage;
    };

} // End of namespace.
