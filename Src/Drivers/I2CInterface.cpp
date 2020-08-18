#include "I2CInterface.hpp"

#include <array>

namespace Copter::Drivers
{

    uint I2CInterface::getInstance(PinName sda, PinName scl)
    {
        // Check for existence of pins already in use
        auto itSDA = std::find(mPins.begin(), mPins.end(), sda);
        auto itSCL = std::find(mPins.begin(), mPins.end(), scl);
        // Check if only one pin is in use but the other isn't, in this case we error
        // (Don't want multiple instances of I2C interfering with each other)
        if((itSDA == mPins.end()) != (itSCL == mPins.end()))
            MBED_ERROR(MBED_MAKE_ERROR(MBED_MODULE_APPLICATION, MBED_ERROR_CODE_PINMAP_INVALID),
                       "Class 'I2CInterface': Invalid combination of pins. One pin is already in use.");
        // Else check whether we already have an instance with given pins
        else if((itSDA != mPins.end()) && (itSCL != mPins.end()))
        {
            // Get index position of sda
            uint ind = itSDA - mPins.begin();
            // We can get the id by using ordering of pins in mPins (e.g: SDA, SCL, SDA, SCL ...)
            return ind / 2;
        }
        else
        {
            uint id = createInstance(sda, scl);
            return id;
        }
    }

    uint I2CInterface::createInstance(PinName sda, PinName scl)
    {
        auto instance = std::make_unique<I2C>(sda, scl);
        mI2C.push_back(std::move(instance));
        mPins.push_back(sda);
        mPins.push_back(scl);
        return mI2C.size()-1;
    }

    int8_t I2CInterface::readByte(const int id, const char address, const char subAddress)
    {
        // Set char to the array.
        char regAddr[1] = {subAddress};

        // Create char array to read Data.
        char readData[1];

        // Write to just the SubAddress to "point" there.
        // Also parameter to "true", to not send an end signal.
        mI2C[id]->write(address, regAddr, 1, true);

        // Read 1 byte starting from the register at the SubAddress.
        mI2C[id]->read(address, readData, 1);

        //Return data that was read.
        return (int8_t)readData[0];
    }

    std::vector<int8_t> I2CInterface::readBytes(const int id, const char address, const char subAddress, const int8_t byteNumber)
    {
        // Create a char array based on t_ByteNumber.
        std::vector<char> readData;
        readData.reserve(byteNumber);

        // Set char to the array.
        char regAddr[1] = {subAddress};

        // Write to just the SubAddress to "point" there.
        // Also parameter to "true", to not send an end signal.
        mI2C[id]->write(address, regAddr, 1, true);

        // Read several consecutive bytes based on ByteNumber, and store in char array.
        mI2C[id]->read(address, readData.data(), byteNumber);

        // Create vector of int8_t.
        std::vector<int8_t> returnData;
        returnData.reserve(byteNumber);

        // Iterate through array, and add values to the vector.
        for(const char &text : readData)
        {
            returnData.push_back(text);
        }

        // Return the vector.
        return returnData;
    }

    void I2CInterface::writeByte(const int id, const char address, const char subAddress, const int8_t data)
    {
        // Create temporary char array.
        char tempData[2] = {subAddress, (char)data};

        // From the Address, write 1 byte to the SubAddress content based on the t_Data data.
        mI2C[id]->write(address, tempData, 2);
    }

    void I2CInterface::writeBytes(const int id, const char address, std::vector<std::pair<char, char>>& data)
    {
        // Create a temporary char array.
        char tempData[2];

        // Iterate through the vector.
        for(std::pair<char, char> &pair : data)
        {
            // Set the SubAddress.
            tempData[0] = pair.first;

            //Set the content.
            tempData[1] = pair.second;


            // Write to Chip.
            mI2C[id]->write(address, tempData, 2);
        }
    }


}