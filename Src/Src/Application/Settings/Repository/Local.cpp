#include "Local.hpp"

// Debug
#include <mbed.h>

// Drivers
#include "Drivers/FileSystem.hpp"


/**
 * Get the setting data from a local storage device
 *
 * @return
 */
char * App::Settings::Repository::Local::read()
{
    // Create instance of file system
    Drivers::FileSystem filesystem;

    // Read the data from the file
    auto content = filesystem.read(m_Path);

    debug("Data %s\n", content);

    // Return content
    return content;
}