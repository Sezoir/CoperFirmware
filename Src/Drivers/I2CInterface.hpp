#pragma once
#include <mbed.h>
#include <vector>
#include <memory>
#include <algorithm>

namespace Copter::Drivers
{
    class I2CInterface
    {
    public:
        /**
         * @brief: Get the id of the i2c instance for the current pins. If the instance doesn't exist it will create
         *         a new instance with the given pins
         * @param sda: PinName of SDA pin
         * @param scl: PinName of SCL pin
         * @return uint: Id of the I2C instance for the given pins
         */
        static uint getInstance(PinName sda, PinName scl);

        /**
         * @brief: Reads a single byte from a sub-address, under a Address
         * @param address: Address of chip
         * @param subAddress: SubAddress you want to read
         * @return int8_t: 8 bit number the sub-address holds
         */
        static int8_t readByte(int id, char address, char subAddress);

        /**
         * @brief: Reads a series of bytes, from a initial starting sub-address
         * @param address: Address of chip
         * @param subAddress: Initial sub-address you want to read
         * @param byteNumber: Number of consecutive bytes you want to read
         * @todo: may template to avoid vector
         * @return std::vector<int8_t>: Contains all the bytes read in consecutive order
         */
        static std::vector<int8_t> readBytes(int id, char address, char subAddress, int8_t byteNumber);

        /**
         * @brief: Writes a byte to the content of a sub-address
         * @param address: Address of chip
         * @param subAddress: Initial sub-address you want to read
         * @param data: Data that will overwrite the sub-address content
         */
        static void writeByte(int id, char address, char subAddress, int8_t data);

        /**
         * @brief: Writes a series of bytes to a series of sub-address
         * @param address: Address of chip
         * @param data: Vector of pairs, where the pairs map a SubAddress to a the content to write, i.e: pair.first = SubAddress, pair.second = content
         * @todo: may template to avoid vector
         */
        static void writeBytes(int id, char address, std::vector<std::pair<char, char>>& data);

    private:
        // Contains all instances of I2C class
        inline static std::vector<std::unique_ptr<I2C>> mI2C = {};
        // Contains all pins currently being used
        inline static std::vector<PinName> mPins = {};

        /**
         * @brief: Creates and stores a new instance of the I2C class
         * @param sda: PinName of SDA pin
         * @param scl: PinName of SCL pin
         * @return uint: Id of the created instance
         */
        static uint createInstance(PinName sda, PinName scl);
    };
}

