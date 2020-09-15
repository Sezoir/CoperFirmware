#pragma once

namespace Copter::Sensors::Filters
{
    template <typename T>
    class HighPass
    {
    public:
        HighPass(float alpha, T initPrevValue, T initPrevRawValue)
            : mAlpha(alpha)
            , mPrevValue(initPrevValue)
            , mPrevRawValue(initPrevRawValue)
        {
        }

        T operator()(T rawData)
        {
            T returnData = ((1 - mAlpha) * mPrevValue) + ((1 - mAlpha) * (rawData - mPrevRawValue));
            mPrevValue = returnData;
            mPrevRawValue = rawData;
            return returnData;
        }

    private:
        const float mAlpha;
        T mPrevValue;
        T mPrevRawValue;
    };
} // namespace Copter::Sensors::Filters