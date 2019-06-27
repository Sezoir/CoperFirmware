#include "Storage.hpp"

/*
 * Checks for existance and mounts the storage device.
 */
void Bootloader::Storage::initStorage(std::string t_Location)
{
    if(exists(t_Location)) {
        //debug("Mounting/Initialising the filesystem...");
        int err = m_Storage[t_Location].second->mount(m_Storage[t_Location].first.get());
        //debug((err ? "Fail :(" : "OK"));
        if (err) {
            MBED_ERROR(MBED_MAKE_ERROR(MBED_MODULE_APPLICATION, MBED_ERROR_CODE_ESRMNT), "No filesystem found.");
        }
    } else MBED_ERROR(MBED_MAKE_ERROR(MBED_MODULE_APPLICATION, MBED_ERROR_CODE_ITEM_NOT_FOUND), "Error: Location doesn't exists.");
}

/**
 * Cheeks to see if Locations already exists.
 */
bool Bootloader::Storage::exists(std::string t_Location)
{
    if(m_Storage.find(t_Location) == m_Storage.end())
    {
        return false;
    } else
    {
        return true;
    }
}

/**
 * Deletes a storage pair.
 */
void Bootloader::Storage::remove(std::string t_Location)
{
    if(exists(t_Location))
    {
        m_Storage.erase(t_Location);
    }
}

