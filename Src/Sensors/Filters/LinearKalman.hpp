#pragma once

namespace Copter::Sensors::Filters
{
    template <typename T>
    class LinearKalman
    {
    public:
        LinearKalman(T measurementError, T initEstimate, T initError)
            : mMeasureError(measurementError)
            , mEstError(initError)
            , mPreEstimate(initEstimate)
            , mKalminGain(1)
        {
        }

        T operator()(T rawData)
        {
            calculateKalmanGain();
            T est = updateEstimation(rawData);
            updateEstError();
            return est;
        }

    private:
        T updateEstimation(T rawData)
        {
            T estimate = mPreEstimate + mKalminGain * (rawData - mPreEstimate);
            mPreEstimate = estimate;
            return estimate;
        }

        void updateEstError()
        {
            T error = (1 - mKalminGain) * (mPreEstError);
            mPreEstError = mEstError;
            mEstError = error;
        }

        void calculateKalmanGain()
        {
            mKalminGain = mEstError / (mEstError + mMeasureError);
        }

        const T mMeasureError;
        T mEstError;
        T mPreEstError;

        T mPreEstimate;

        double mKalminGain;
    };

} // namespace Copter::Sensors::Filters
