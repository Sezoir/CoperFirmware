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
         * @brief Get the id of the i2c instance for the current pins. If the instance doesn't exist it will create
         *         a new instance with the given pins.
         * @param sda: PinName of SDA pin.
         * @param scl: PinName of SCL pin.
         * @return uint: Id of the I2C instance for the given pins.
         */
        static uint getInstance(PinName sda, PinName scl);

        /**
         * @brief Reads a single byte from a sub-address, under a Address.
         * @param id: Id of the I2C class you want to use.
         * @param address: Address of chip.
         * @param subAddress: SubAddress you want to read.
         * @return uint8_t: 8 bit number the sub-address holds.
         */
        static uint8_t readByte(uint id, char address, char subAddress);

        /**
         * @brief Reads a series of register from an initial starting subAddress.
         * @tparam byteCnt: Number of consecutive registers you want to read.
         * @param id: Id of the I2C class you want to use.
         * @param address: Address of chip.
         * @param subAddress: Initial register you want to read.W
         * @return std::array<uint8_t, byteCnt>: Array containing all bytes read in consecutive order.
         */
        template <uint byteCnt>
        static std::array<uint8_t, byteCnt> readConsBytes(uint id, char address, char subAddress)
        {
            // Create vector of int8_t.
            std::array<uint8_t, byteCnt> returnData;

            // Read several consecutive bytes based on ByteNumber, and store in char array.
            for(uint i = 0; i < byteCnt; i++)
            {
                returnData[i] = (readByte(id, address, subAddress + i));
            }

            // Return the vector.
            return returnData;
        }

        /**
         * Reads a number of bytes from a register.
         * @tparam byteCnt: Number of consecutive bytes you want to read.
         * @param id: Id of the I2C class you want to use.
         * @param address: Address of chip.
         * @param subAddress: Initial register you want to read.
         * @return std::array<uint8_t, byteCnt>: Array containing all bytes.
         */
        template <uint byteCnt>
        static std::array<uint8_t, byteCnt> readBytes(uint id, char address, char subAddress)
        {
            // Set char to the array.
            char regAddr[1] = {subAddress};

            // Create char array to read Data.
            char rawData[byteCnt];

            // Write to just the SubAddress to "point" there.
            // Also parameter to "true", to not send an end signal.
            mI2C[id]->write(address, regAddr, 1, true);

            // Read 1 byte starting from the register at the SubAddress.
            mI2C[id]->read(address, rawData, byteCnt);

            // Convert data to return type
            std::array<uint8_t, byteCnt> returnData;
            for(uint i = 0; i < byteCnt; i++)
                returnData[i] = static_cast<uint8_t>(rawData[i]);

            // Return the vector.
            return returnData;
        }

        /**
         * @brief Writes a byte to the content of a sub-address
         * @param id: Id of the I2C class you want to use.
         * @param address: Address of chip
         * @param subAddress: Initial sub-address you want to read
         * @param data: Data that will overwrite the sub-address content
         */
        static void writeByte(uint id, char address, char subAddress, uint8_t data);

        /**
         * @brief Writes a series of bytes to a series of sub-address
         * @param id: Id of the I2C class you want to use.
         * @param address: Address of chip
         * @param data: Vector of pairs, where the pairs map a SubAddress to a the content to write, i.e: pair.first =
         * SubAddress, pair.second = content
         * @todo: Change vector to use iterators
         */
        static void writeBytes(uint id, char address, std::vector<std::pair<char, char>>& data);

    private:
        // Contains all instances of I2C class
        inline static std::vector<std::unique_ptr<I2C>> mI2C = {};
        // Contains all pins currently being used
        inline static std::vector<PinName> mPins = {};

        /**
         * @brief Creates and stores a new instance of the I2C class
         * @param sda: PinName of SDA pin
         * @param scl: PinName of SCL pin
         * @return uint: Id of the created instance
         */
        static uint createInstance(PinName sda, PinName scl);
    };
} // namespace Copter::Drivers
