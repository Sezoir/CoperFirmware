#pragma once

// Std libs
#include <map>
#include <string>
#include <memory>

// Drivers
#include <Debug.hpp>

// Default struct
#include "Object.hpp"

// Parser table
#include "Packages/Parser.hpp"


namespace App { namespace Settings { namespace Packages { namespace MPU9250
{

    class MPU9250: public Packages::Parser
    {

        public:
            MPU9250()
            {

            }

            ~MPU9250()
            {

            }

            std::shared_ptr<Object> get() override
            {
                debug("MPU9250 settings created dynamically.\n");

                auto t = make_shared<ObjMPU9250>();
                t->id = "Test ID";
                return t;
            }

            IMPLEMENT_SETTING_CLONE(MPU9250);
    };

}}}}


