#pragma once

namespace Copter::Engine
{
    class Driver
    {
        virtual bool setup() {return true;}
        virtual void incThrottle() {}
        virtual void decThrottle() {}
    };
}

