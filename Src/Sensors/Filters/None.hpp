#pragma once

namespace Copter::Sensors::Filters
{
    /**
     * @brief A filter which just mirrors what is sent in for when you don't want to use a filter.
     * @tparam T
     */
    class None
    {
    public:
        template <typename T>
        constexpr T operator()(T rawData)
        {
            return rawData;
        }
    };
} // namespace Copter::Sensors::Filters