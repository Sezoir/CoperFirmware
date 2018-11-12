//
// Created by Josh Mottley on 08/10/2018.
//

#ifndef COPTER_I2CWRAP_HPP
#define COPTER_I2CWRAP_HPP

#include <vector>
#include <utility>
#include "mbed.h"

namespace App { namespace I2C {
        class I2CWrap : protected mbed::I2C
        {
        public:
            I2CWrap(PinName t_Sda, PinName t_Scl);

            ~I2CWrap();

            int8_t readByte(char t_Address, char t_SubAddress);

            std::vector<int8_t> readBytes(char t_Address, char t_SubAddress, int8_t t_ByteNumber);

            void writeByte(char t_Address, char t_SubAddress, char t_Data);

            void writeBytes(char t_Address, std::vector<std::pair<char, char>> t_Data);

        private:
            char m_RegAddr[1];

        };
    }
}

#endif //COPTER_I2CWRAP_HPP
