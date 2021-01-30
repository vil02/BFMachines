#ifndef CHANGEVALUEFUNCTIONS_HPP_INCLUDED
#define CHANGEVALUEFUNCTIONS_HPP_INCLUDED

namespace bfm::memory_types
{
    template<typename MemoryType>
    constexpr void increase_value(
            MemoryType& in_memory,
            const typename MemoryType::position_type& in_position)
    {
        change_value(in_memory, in_position, 1);
    }
    template<typename MemoryType>
    constexpr void decrease_value(
            MemoryType& in_memory,
            const typename MemoryType::position_type& in_position)
    {
        using value_type = typename MemoryType::value_type;
        change_value(in_memory, in_position, value_type(-1));
    }
    template<typename MemoryType>
    constexpr void change_value(
            MemoryType& in_memory,
            const typename MemoryType::position_type& in_position,
            const typename MemoryType::value_type& in_value_change)
    {
        using value_type = typename MemoryType::value_type;
        in_memory.set_value(
            in_position,
            value_type(in_memory.get_value(in_position)+in_value_change));
    }
}

#endif // CHANGEVALUEFUNCTIONS_HPP_INCLUDED
