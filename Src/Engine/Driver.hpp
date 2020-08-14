#pragma once
#include "units.h"


namespace units
{
    UNIT_ADD(velocity, speed, speeds, sd, units::unit<std::ratio<1>, units::category::velocity_unit>)
    UNIT_ADD(velocity, dshot, dshot, ds, units::unit<std::ratio<10000, 2047>, units::velocity::speed>)
}

namespace Copter::Engine
{
    class Driver
    {
    public:
        virtual bool setup() {return true;}
        virtual void incThrottle(const float && ramp) {}
        virtual void decThrottle(const float && ramp) {}
        virtual void sendSignal(const units::velocity::speed_t & ramp) {}
    };
}

