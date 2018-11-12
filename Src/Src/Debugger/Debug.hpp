//
// Created by Josh Mottley on 28/08/2018.
//

#ifndef COPTER_DEBUG_H
#define COPTER_DEBUG_H

#include <string>
#include <utility>
#include "mbed.h"
#include "copter_Config.h"

namespace Debugger {

    static Serial pc(USBTX, USBRX);

    class Debug
    {
    public:
        Debug()// : pc(*new Serial(USBTX, USBRX))
        {
            pc.baud(DEBUG_BAUD);
        }

        template<typename... ArgTypes>
        static void sendMsg(std::string && format, ArgTypes... args)
        {
            if(DEBUG == true) {
                std::string msg = format;
                msg.append("\n");
                pc.printf(msg.c_str(), args...);
            } else {}
        };

        static void sendMsg(std::string && format)
        {
            if(DEBUG == true) {
                std::string msg = format;
                msg.append("\n");
                pc.printf(msg.c_str());
            } else{}
        };


        static void sendErr(const char *t_errmsg, const char *t_errtype) {
            pc.printf("error: %s (%s)\n", t_errmsg, t_errtype);
        }
    };

} // End of namespace
#endif //COPTER_DEBUG_H
