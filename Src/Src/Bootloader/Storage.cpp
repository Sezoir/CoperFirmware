//
// Created by Josh Mottley on 29/11/2018.
//

#include "Storage.hpp"

#include <Debug.hpp>

//m_FATFileSystem(*new FATFileSystem("local")),
//m_SDBlockDevice(*new SDBlockDevice(PE_6, PE_5, PE_2, PE_4))

Bootloader::Storage::Storage()
{
    Debugger::Debug::sendMsg("Storage Created");
}

Bootloader::Storage::~Storage()
{
    Debugger::Debug::sendMsg("Manager Destroyed");
}

void Bootloader::Storage::initStorage(std::string t_Location)
{
    Debugger::Debug::sendMsg("Mounting/Initialising the filesystem...");
    int err = m_Storage[t_Location].second->mount(m_Storage[t_Location].first.get());
    Debugger::Debug::sendMsg((err ? "Fail :(" : "OK"));
    if (err) {
        Debugger::Debug::sendMsg("No filesystem found.");
    }
}

bool Bootloader::Storage::exists(std::string t_Location)
{
    if(m_Storage.find(t_Location) != m_Storage.end())
    {
        return true;
    } else
    {
        Debugger::Debug::sendMsg("Error: Location already exists");
        return false;
    }
}

void Bootloader::Storage::remove(std::string t_Location)
{
    if(exists(t_Location))
    {
        m_Storage.erase(t_Location);
    }
}

