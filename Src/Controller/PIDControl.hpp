#pragma once
// Externals libs
#include <mbed.h>
#include <units.h>

namespace Copter::Controller
{
    template <typename Type>
    class PIDControl
    {
    public:
        constexpr PIDControl(double pValue, double iValue, double dValue)
            : mP(pValue)
            , mI(iValue)
            , mD(dValue)
        {
            mPrevTime = Kernel::Clock::now().time_since_epoch().count();
        }

        Type update(Type curValue)
        {
            // Calculate error difference
            const Type error = mDesValue - curValue;
            const Type diffError = error - mPrevError;

            // Calculate time difference
            double curTime = Kernel::Clock::now().time_since_epoch().count();
            double diffTime = (curTime - mPrevTime) / 1000;
            mPrevTime = curTime;

            // Calculate pid values
            const Type pValue = error * mP;
            mIValue += error * diffTime;
            if(mIValue < -mWindUpGuard)
                mIValue = -mWindUpGuard;
            else if(mIValue > mWindUpGuard)
                mIValue = mWindUpGuard;

            const Type dValue = diffError / diffTime;

            // Update error
            mPrevError = error;

            return pValue + (mI * mIValue) + (mD * dValue);
        }

        void setDesValue(Type setpoint)
        {
            mDesValue = setpoint;
        }

        void setWindUp(Type windup)
        {
            mWindUpGuard = windup;
        }

    private:
        double mPrevTime = 0;
        Type mDesValue{0};
        Type mCurPoint{0};
        Type mIValue{0};
        Type mPrevError{0};
        Type mWindUpGuard{0};
        const double mP;
        const double mI;
        const double mD;
    };
} // namespace Copter::Controller
