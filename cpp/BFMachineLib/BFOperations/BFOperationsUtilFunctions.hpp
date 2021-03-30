#ifndef BFOPERATIONSUTILFUNCTIONS_HPP_INCLUDED
#define BFOPERATIONSUTILFUNCTIONS_HPP_INCLUDED

#include <type_traits>

namespace bfm::bfo::util
{
    template <typename T>
    [[nodiscard]] constexpr auto abs(const T& in_val)
    {
        using result_type = typename std::make_unsigned<T>::type;
        return in_val >= T(0) ? result_type(in_val) : result_type(-in_val);
    }

    template <typename T>
    [[nodiscard]] constexpr auto mod(const T& in_val, const T& in_div)
    {
        using result_type = typename std::make_unsigned<T>::type;
        return result_type(abs(in_val)%abs(in_div));
    }
}

#endif // BFOPERATIONSUTILFUNCTIONS_HPP_INCLUDED
