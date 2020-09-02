#include "Motor.hpp"
#include <cassert>

namespace Copter::Engine
{

    Motor::Motor(ProtocolInterface& protocol, Motor::Profile profile,
                 std::chrono::duration<int64_t, std::milli>&& delay)
        : mProtocol(&protocol)
        , mProfile(profile)
        , mDelay(delay)
        , isInit(true)
    {
    }

    void Motor::init(ProtocolInterface& protocol, Motor::Profile profile,
                     std::chrono::duration<int64_t, std::milli> delay)
    {
        mProtocol = &protocol;
        mProfile = profile;
        mDelay = delay;
        isInit = true;
    }

    void Motor::setSpeed(units::protocol::speed_t speed)
    {
        // Is the class ready?
        assert(isInit == true);

        // Set speed
        mDesSpeed = speed;
    }

    void Motor::update()
    {
        // Is the class ready?
        assert(isInit == true);

        if(units::math::abs(mDesSpeed - mSpeed) != 0_sd)
        {
            switch(mProfile)
            {
                case Profile::FastRamp:
                    fastRamp();
                    break;
                case Profile::SlowRamp:
                    slowRamp();
                    break;
            }
        }
        mProtocol->sendSignal(mSpeed);
    }

    void Motor::fastRamp()
    {
        // Is the class ready?
        assert(isInit == true);

        // Set gradient for how fast to ramp
        const int grad = 20;

        // Calculate change in terms of speed_t @todo: May change to not include the delay time
        units::protocol::speed_t ramp(grad * mDelay.count());

        // Check whether we are ascending/descending
        if(mDesSpeed - mSpeed < 0_sd)
            ramp *= -1;

        // Calculate new speed
        units::protocol::speed_t newSpeed = mSpeed + ramp;

        // Check whether we are over or under wanted speed
        if((newSpeed > mDesSpeed) && (mSpeed < mDesSpeed))
        {
            mSpeed = mDesSpeed;
        }
        else if((newSpeed < mDesSpeed) && (mSpeed > mDesSpeed))
        {
            mSpeed = mDesSpeed;
        }
        else
        {
            mSpeed = newSpeed;
        }
    }

    void Motor::slowRamp()
    {
        // Is the class ready?
        assert(isInit == true);

        // Set gradient for how fast to ramp
        const int grad = 1;

        // Calculate change in terms of speed_t @todo: May change to not include the delay time
        units::protocol::speed_t ramp(grad * mDelay.count());

        // Check whether we are ascending/descending
        if(mDesSpeed - mSpeed < 0_sd)
            ramp = ramp * -1;

        // Calculate new speed
        units::protocol::speed_t newSpeed = mSpeed + ramp;

        // Check whether we are over or under wanted speed
        if((newSpeed > mDesSpeed) && (mSpeed < mDesSpeed))
        {
            mSpeed = mDesSpeed;
        }
        else if((newSpeed < mDesSpeed) && (mSpeed > mDesSpeed))
        {
            mSpeed = mDesSpeed;
        }
        else
        {
            mSpeed = newSpeed;
        }
    }
} // namespace Copter::Engine
