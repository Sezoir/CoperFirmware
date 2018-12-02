//
// Created by Josh Mottley on 29/11/2018.
//

#include "FileSystem.hpp"
#include "Debugger/Debug.hpp"

App::FileSystem::FileSystem::FileSystem(std::string t_Location) : m_Location(t_Location)
{
    Debugger::Debug::sendMsg("FileSystem Created.");
}

App::FileSystem::FileSystem::~FileSystem()
{
    Debugger::Debug::sendMsg("FileSystem Destroyed.");
}

void App::FileSystem::FileSystem::openFile(const char* t_SystemPath)
{
    Debugger::Debug::sendMsg("Opening %s", t_SystemPath);
    f = fopen(t_SystemPath, "r");
    Debugger::Debug::sendMsg((!f ? "Fail :(" : "OK"));
    if (!f) {
        Debugger::Debug::sendMsg("No file found. Creating file.");
        f = fopen(t_SystemPath, "w+");
        Debugger::Debug::sendMsg((!f ? "Fail :(" : "OK"));
        if (!f) {
            Debugger::Debug::sendErr(strerror(errno), (char*)-errno);
        }
        fclose(f);
        f = fopen(t_SystemPath, "r");
    }
}

void App::FileSystem::FileSystem::closeFile(const char *t_SystemPath)
{
    Debugger::Debug::sendMsg("Closing ", t_SystemPath);
    err = fclose(f);
    Debugger::Debug::sendMsg((err < 0 ? "Fail :(" : "OK"));
    if (err < 0) {
        Debugger::Debug::sendErr(strerror(errno), (char*)-errno);
    }
}

char* App::FileSystem::FileSystem::readFile(const std::string t_SystemPath)
{
    std::string path = "/" + m_Location + "/" + t_SystemPath;
    openFile(path.c_str());
    Debugger::Debug::sendMsg("Reading file");
    long length;
    fseek(f, 0, SEEK_END);
    length = ftell(f);
    fseek(f, 0, SEEK_SET);
    char* config = (char*)calloc( 1, length+1);
    fread(config, 1, length, f);
    closeFile(path.c_str());
    return config;
}