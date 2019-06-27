#pragma once

// External Lib
#include "mbed.h"

// Std libs
#include <vector>
#include <utility>

namespace App { namespace Drivers {
	class I2CWrap : protected mbed::I2C
	{
		public:
			/**
			 * Constructor for I2CWrap.
			 * @param t_Sda: SDA pin.
			 * @param t_Scl: SCL pin.
			 */
			I2CWrap(PinName t_Sda, PinName t_Scl);

			/**
			 * Destructor for I2CWrap
			 */
			~I2CWrap() = default;

			/**
			 * Reads a single byte from a SubAddress, under a Address.
			 * @param t_Address: Address of chip.
			 * @param t_SubAddress: SubAddress you want to read.
			 * @return int8_t: 8 bit number the SubAddress holds.
			 */
			int8_t readByte(char t_Address, char t_SubAddress);

			/**
			 * Reads a series of bytes, from a initial starting SubAddress.
			 * @param t_Address: Address of chip.
			 * @param t_SubAddress: Initial SubAddress you want to read.
			 * @param t_ByteNumber: Number of consecutive bytes you want to read.
			 * @return std::vector<int8_t>: Contains all the bytes read in order of reading.
			 */
			std::vector<int8_t> readBytes(char t_Address, char t_SubAddress, int8_t t_ByteNumber);

			/**
			 * Writes a byte to the content of a SubAddress.
			 * @param t_Address: Address of chip.
			 * @param t_SubAddress: Initial SubAddress you want to read.
			 * @param t_Data: Data that will overwrite the SubAddress content.
			 */
			void writeByte(const char t_Address, const char t_SubAddress, const int8_t t_Data);

			/**
			 * Writes a series of bytes to a series of SubAddress.
			 * @param t_Address: Address of chip.
			 * @param t_Data: Vector of pairs, where the pairs map a SubAddress to a the content to write, i.e: pair.first = SubAddress, pair.second = content.
			 */
			void writeBytes(char t_Address, std::vector<std::pair<char, char>> t_Data);

		private:
			// @todo Maybe reprogram such that I don't need this variable.
			// Char to temp store current address.
			char m_RegAddr[1];

	};
}}
