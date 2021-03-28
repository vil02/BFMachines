#ifndef TRANSLATORSUTILFUNCTIONS_HPP_INCLUDED
#define TRANSLATORSUTILFUNCTIONS_HPP_INCLUDED

namespace bfm::bfo::translators::util
{
    template <typename T>
    T abs(const T& in_val)
    {
        return in_val >= T(0) ? in_val : -in_val;
    }
}

#endif // TRANSLATORSUTILFUNCTIONS_HPP_INCLUDED
