//
// Created by Josh Mottley on 08/10/2018.
//

#include "I2CWrap.hpp"
#include "Debug.hpp"

App::I2C::I2CWrap::I2CWrap(PinName t_Sda, PinName t_Scl) : mbed::I2C(t_Sda, t_Scl)
{
    Debugger::Debug::sendMsg("I2CWrap Created");
}

App::I2C::I2CWrap::~I2CWrap()
{
    Debugger::Debug::sendMsg("I2C Destroyed");
}

int8_t App::I2C::I2CWrap::readByte(char t_Address, char t_SubAddress)
{
    m_RegAddr[0] = t_SubAddress;
    char readData[1];
    write(t_Address, m_RegAddr, 1, true);
    read(t_Address, readData, 1);
    return (int8_t)readData[0];
}

std::vector<int8_t> App::I2C::I2CWrap::readBytes(char t_Address, char t_SubAddress, int8_t t_ByteNumber)
{
    char readData[t_ByteNumber];
    m_RegAddr[0] = t_SubAddress;
    write(t_Address, m_RegAddr, 1, true);
    read(t_Address, readData, t_ByteNumber);
    std::vector<int8_t> returnData;
    for(const char &text : readData)
    {
        returnData.push_back(text);
    }
    return returnData;
}

void App::I2C::I2CWrap::writeByte(char t_Address, char t_SubAddress, char t_Data)
{
    char tempData[2] = {t_SubAddress, t_Data};
    write(t_Address, tempData, 2);
}

void App::I2C::I2CWrap::writeBytes(char t_Address, std::vector<std::pair<char, char>> t_Data)
{
    char tempData[2];
    for(std::pair<char, char> &pair : t_Data)
    {
        tempData[0] = pair.first;
        tempData[1] = pair.second;
        write(t_Address, tempData, 2);
    }
}