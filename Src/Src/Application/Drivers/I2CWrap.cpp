#include "I2CWrap.hpp"

/**
 * Constructor for I2CWrap.
 */
App::Drivers::I2CWrap::I2CWrap(PinName t_Sda, PinName t_Scl) : mbed::I2C(t_Sda, t_Scl) {}

/**
 * Reads a single byte from a SubAddress, under a Address.
 */
int8_t App::Drivers::I2CWrap::readByte(char t_Address, char t_SubAddress)
{
    // Set char to the array.
    m_RegAddr[0] = t_SubAddress;

    // Create char array to read Data.
    char readData[1];

    // Write to just the SubAddress to "point" there.
    // Also parameter to "true", to not send an end signal.
    write(t_Address, m_RegAddr, 1, true);

    // Read 1 byte starting from the register at the SubAddress.
    read(t_Address, readData, 1);

    //Return data that was read.
    return (int8_t)readData[0];
}

/**
 * Reads a series of bytes, from a initial starting SubAddress.
 */
std::vector<int8_t> App::Drivers::I2CWrap::readBytes(char t_Address, char t_SubAddress, int8_t t_ByteNumber)
{
    // @todo Change from char array, as initialing a array from parameter is unsupported C. (Prob change to vector)
    // Create a char array based on t_ByteNumber.
    char readData[t_ByteNumber];

    // Set char to the array.
    m_RegAddr[0] = t_SubAddress;

    // Write to just the SubAddress to "point" there.
    // Also parameter to "true", to not send an end signal.
    write(t_Address, m_RegAddr, 1, true);

    // Read several consecutive bytes based on ByteNumber, and store in char array.
    read(t_Address, readData, t_ByteNumber);

    // Create vector of int8_t.
    std::vector<int8_t> returnData;

    // Iterate through array, and add values to the vector.
    for(const char &text : readData)
    {
        returnData.push_back(text);
    }

    // Return the vector.
    return returnData;
}

/**
 * Writes a byte to the content of a SubAddress.
 */
void App::Drivers::I2CWrap::writeByte(const char t_Address, const char t_SubAddress, const int8_t t_Data)
{
    // Create temporary char array.
    char tempData[2] = {t_SubAddress, (char)t_Data};

    // From the Address, write 1 byte to the SubAddress content based on the t_Data data.
    write(t_Address, tempData, 2);
}

/**
 * Writes a series of bytes to a series of SubAddress.
 */
void App::Drivers::I2CWrap::writeBytes(char t_Address, std::vector<std::pair<char, char>> t_Data)
{
    // Create a temporary char array.
    char tempData[2];

    // Iterate through the vector.
    for(std::pair<char, char> &pair : t_Data)
    {
        // Set the SubAddress.
        tempData[0] = pair.first;

        //Set the content.
        tempData[1] = pair.second;


        // Write to Chip.
        write(t_Address, tempData, 2);
    }
}