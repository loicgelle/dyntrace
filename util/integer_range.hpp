/**
 * Integer ranges types. Useful to represent addresses or offset ranges
 */

#ifndef DYNTRACE_UTIL_RANGE_HPP_
#define DYNTRACE_UTIL_RANGE_HPP_

#include <limits>

#include "util.hpp"

namespace dyntrace
{

    template<typename Int>
    struct integer_range
    {
        Int start;
        Int end;

        constexpr integer_range(Int _start, Int _end) noexcept
            : start{_start}, end{_end}
        {
            if(start > end)
                std::swap(start, end);
        }

        constexpr integer_range() noexcept
            : integer_range{std::numeric_limits<Int>::min(), std::numeric_limits<Int>::max()} {}

        /**
         * True if v >= start and v <= end
         */
        constexpr bool contains(Int v) const noexcept
        {
            return v >= start && v < end;
        }

        /**
         * True if r is completely contained in the range
         */
        template<typename Int2>
        constexpr bool contains(const integer_range<Int2> &r) const noexcept
        {
            return r.start >= start && r.end <= end;
        }

        /**
         * True if r is partially contained in the range
         */
        template<typename Int2>
        constexpr bool crosses(const integer_range<Int2>& r) const noexcept
        {
            return (r.start < start && r.end > start) || (r.start < end && r.end > end);
        }

        constexpr Int size() const noexcept
        {
            return end - start;
        }

        constexpr integer_range<Int> intersection(const integer_range<Int>& r) const noexcept
        {
            if(!contains(r) && !r.contains(*this) && !crosses(r))
                return {0, 0};
            return {max(start, r.start), min(end, r.end)};
        }

        /**
         * True if the range is not empty
         */
        explicit constexpr operator bool() const noexcept
        {
            return size() != 0;
        }
    };

    /**
     * Creates a range around center of size size. The range is [center - size/2, center + size/2)
     */
    template<typename Int>
    constexpr auto integer_range_around(Int center, Int size) noexcept
    {
        // TODO overflow and underflow safe
        return integer_range<Int>{center - (size >> 1), center + (size >> 1)};
    }

    using address_range = integer_range<uintptr_t>;
    constexpr auto address_range_around = integer_range_around<uintptr_t>;
}

#endif