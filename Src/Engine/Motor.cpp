#include "Motor.hpp"
using namespace units::literals;
namespace Copter::Engine
{

    Motor::Motor(Driver& protocol, Motor::Profile profile, std::chrono::duration<double> && delay):
    mProtocol(protocol),
    mProfile(profile),
    mSpeed(0),
    mDesSpeed(0),
    mDelay(delay)
    {

    }

    void Motor::setSpeed(units::velocity::speed_t speed)
    {
        this->mDesSpeed = speed;
    }

    void Motor::update()
    {
        if(units::math::abs(this->mDesSpeed-this->mSpeed) != 0_sd)
            switch(this->mProfile)
            {
                case Profile::FastRamp:
                    fastRamp();
                    break;
                case Profile::SlowRamp:
                    slowRamp();
                    break;
            }

        this->mProtocol.sendSignal(this->mSpeed);
    }

    void Motor::setProfile(Motor::Profile profile)
    {
        this->mProfile = profile;
    }

    void Motor::fastRamp()
    {
        // Set gradient for how fast to ramp
        const int grad = 20000;
        // Calculate change in terms of speed_t @todo: May change to not include the delay time
        units::velocity::speed_t ramp(grad*this->mDelay.count());
        // Check whether we are ascending/descending
        if(this->mDesSpeed-this->mSpeed < 0_sd) ramp *= -1;
        // Calculate new speed
        units::velocity::speed_t newSpeed = this->mSpeed + ramp;
        // Check whether we are over or under wanted speed
        if((newSpeed > this->mDesSpeed) && (this->mSpeed < this->mDesSpeed))
            this->mSpeed = this->mDesSpeed;
        else if((newSpeed < this->mDesSpeed) && (this->mSpeed > this->mDesSpeed))
            this->mSpeed = this->mDesSpeed;
        else
            this->mSpeed = newSpeed;
    }

    void Motor::slowRamp()
    {
        // Set gradient for how fast to ramp
        const int grad = 1000;
        // Calculate change in terms of speed_t @todo: May change to not include the delay time
        units::velocity::speed_t ramp(grad*this->mDelay.count());
        // Check whether we are ascending/descending
        if(this->mDesSpeed-this->mSpeed < 0_sd) ramp = ramp * -1;
        // Calculate new speed
        units::velocity::speed_t newSpeed = this->mSpeed + ramp;
        // Check whether we are over or under wanted speed
        if((newSpeed > this->mDesSpeed) && (this->mSpeed < this->mDesSpeed))
            this->mSpeed = this->mDesSpeed;
        else if((newSpeed < this->mDesSpeed) && (this->mSpeed > this->mDesSpeed))
            this->mSpeed = this->mDesSpeed;
        else
            this->mSpeed = newSpeed;
    }


}
