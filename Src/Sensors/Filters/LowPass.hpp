#pragma once

namespace Copter::Sensors::Filters
{
    template <typename T>
    class LowPass
    {
    public:
        LowPass(float alpha, T initPrevValue)
            : mAlpha(alpha)
            , mPrevValue(initPrevValue)
        {
        }

        T operator()(T rawData)
        {
            T returnData = ((1 - mAlpha) * rawData) + (mAlpha * mPrevValue);
            mPrevValue = returnData;
            return returnData;
        }

    private:
        const float mAlpha;
        T mPrevValue;
    };
} // namespace Copter::Sensors::Filters