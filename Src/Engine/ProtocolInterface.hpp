#pragma once
#include "units.h"

namespace units
{
    UNIT_ADD(velocity, speed, speeds, sd, units::unit<std::ratio<1>, units::category::velocity_unit>)
    UNIT_ADD(velocity, dshot, dshot, ds, units::unit<std::ratio<10000, 2047>, units::velocity::speed>)
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

        virtual void sendSignal(const units::velocity::speed_t& ramp)
        {
        }
    };
} // namespace Copter::Engine
