//
// Created by Josh Mottley on 29/09/2018.
//

#include "SDFatSystem.hpp"
#include <Debug.hpp>

App::FileSystem::SDFatSystem::SDFatSystem()
{
    Debugger::Debug::sendMsg("SDFatSystem Created");
}

App::FileSystem::SDFatSystem::~SDFatSystem()
{
    Debugger::Debug::sendMsg("SDFatSystem Destroyed");
}

void App::FileSystem::SDFatSystem::openFileRaw(const char* t_systempath)
{
    Debugger::Debug::sendMsg("Opening ", t_systempath);
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

void App::FileSystem::SDFatSystem::closeFileRaw(const char *t_systempath)
{
    Debugger::Debug::sendMsg("Closing ", t_systempath);
    err = fclose(f);
    Debugger::Debug::sendMsg((err < 0 ? "Fail :(" : "OK"));
    if (err < 0) {
        Debugger::Debug::sendErr(strerror(errno), (char*)-errno);
    }
}

char* App::FileSystem::SDFatSystem::readFileRaw()
{
    Debugger::Debug::sendMsg("Reading file");
    long length;
    fseek(f, 0, SEEK_END);
    length = ftell(f);
    fseek(f, 0, SEEK_SET);
    char* config = (char*)calloc( 1, length+1);
    fread(config, 1, length, f);
    return config;
}

char* App::FileSystem::SDFatSystem::readFile(const char *t_systempath)
{
    openFileRaw(t_systempath);
    char* temp = readFileRaw();
    closeFileRaw(t_systempath);
    return temp;
}

void App::FileSystem::SDFatSystem::writeFileRaw()
{

}

void App::FileSystem::SDFatSystem::writeFile(const char *t_systempath)
{

}