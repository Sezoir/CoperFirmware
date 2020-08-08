#pragma once

namespace Copter::Engine
{
    class Driver
    {
        virtual bool setup() {return true;}
        virtual void incThrottle(const float && ramp) {}
        virtual void decThrottle() {}
    };
}

