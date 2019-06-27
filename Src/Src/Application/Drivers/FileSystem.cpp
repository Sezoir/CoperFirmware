#include "FileSystem.hpp"

/**
 * Read the whole content of a file at t_SystemPath, to a char*.
 */
char* App::Drivers::FileSystem::read(const std::string t_SystemPath)
{
    // Open the file.
    open(t_SystemPath.c_str());

    // Get the length of the file.
    long length;
    fseek(f, 0, SEEK_END);
    length = ftell(f);
    fseek(f, 0, SEEK_SET);

    // Create space for the content.
    char* config = (char*)calloc( 1, length+1);

    // Read the content.
    fread(config, 1, length, f);

    // Close the file handler.
    close(t_SystemPath.c_str());

    // Return the content.
    return config;
}


/**
 * Write content to a file.
 */
void App::Drivers::FileSystem::write(const char *t_SystemPath)
{

}


/**
 * @private Open the file at the Path and points the FILE* to it.
 */
void App::Drivers::FileSystem::open(const char* t_SystemPath)
{
    // Debug message.
    // debug("Opening %s", t_SystemPath);

    // Open path to file.
    f = fopen(t_SystemPath, "r");

    // Debug message.
    // debug((!f ? "Fail :(" : "OK"));

    // Has the file been opened.
    if (!f)
    {
        // Debug message.
        debug("No file found. Creating file.\n");

        // Open the file and create it.
        f = fopen(t_SystemPath, "w+");

        // Debug.
        // debug((!f ? "Fail :(" : "OK"));

        // Does file exist.
        if (!f)
        {
            // Error message
            MBED_ERROR(MBED_MAKE_ERROR(MBED_MODULE_FILESYSTEM, MBED_ERROR_CODE_ITEM_NOT_FOUND), strerror(errno));
        }

        // Close file handler
        fclose(f);
        f = fopen(t_SystemPath, "r");
    }
}


/**
 * @private Closes the file pointed at by the FILE* f.
 */
void App::Drivers::FileSystem::close(const char *t_SystemPath)
{
    // Close file handler
    err = fclose(f);

    // Debug message
    // debug((err < 0 ? "Fail :(" : "OK"));

    // Where there any errors
    if (err < 0)
    {
        // Debug message
        MBED_ERROR(MBED_MAKE_ERROR(MBED_MODULE_FILESYSTEM, MBED_ERROR_CODE_FAILED_OPERATION), strerror(errno));
    }
}
