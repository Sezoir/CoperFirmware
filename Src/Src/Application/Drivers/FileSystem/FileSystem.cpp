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

void App::FileSystem::FileSystem::openFile(const char* t_systempath)
{
    Debugger::Debug::sendMsg("Opening %s", t_systempath);
    f = fopen(t_systempath, "r");
    Debugger::Debug::sendMsg((!f ? "Fail :(" : "OK"));
    if (!f) {
        Debugger::Debug::sendMsg("No file found. Creating file.");
        f = fopen(t_systempath, "w+");
        Debugger::Debug::sendMsg((!f ? "Fail :(" : "OK"));
        if (!f) {
            Debugger::Debug::sendErr(strerror(errno), (char*)-errno);
        }
        fclose(f);
        f = fopen(t_systempath, "r");
    }
}

void App::FileSystem::FileSystem::closeFile(const char *t_systempath)
{
    Debugger::Debug::sendMsg("Closing ", t_systempath);
    err = fclose(f);
    Debugger::Debug::sendMsg((err < 0 ? "Fail :(" : "OK"));
    if (err < 0) {
        Debugger::Debug::sendErr(strerror(errno), (char*)-errno);
    }
}

char* App::FileSystem::FileSystem::readFile(const std::string t_SystemPath)
{
    openFile(("/ " + m_Location + "/" + t_SystemPath).c_str());
    Debugger::Debug::sendMsg("Reading file");
    long length;
    fseek(f, 0, SEEK_END);
    length = ftell(f);
    fseek(f, 0, SEEK_SET);
    char* config = (char*)calloc( 1, length+1);
    fread(config, 1, length, f);
    closeFile((m_Location + t_SystemPath).c_str());
    return config;
}