#pragma once
#include "units.h"

namespace units
{
    UNIT_ADD(protocol, speed, speeds, sd, units::unit<std::ratio<1>, units::length::meter>)
    UNIT_ADD(protocol, dshot, dshot, dst, units::unit<std::ratio<10000, 2047>, units::protocol::speed>)
} // namespace units

namespace Copter::Engine
{
    class ProtocolInterface
    {
    public:
        ProtocolInterface() = default;

        virtual bool setup()
        {
            return true;
        }

        virtual void sendSignal(const units::protocol::speed_t& ramp)
        {
        }
    };
} // namespace Copter::Engine
