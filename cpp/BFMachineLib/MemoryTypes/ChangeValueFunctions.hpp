#ifndef CHANGEVALUEFUNCTIONS_HPP_INCLUDED
#define CHANGEVALUEFUNCTIONS_HPP_INCLUDED

namespace bfm::memory_types
{
    template<typename MemoryType>
    constexpr void increase_value(
            MemoryType& in_memory,
            const typename MemoryType::position_type& in_position)
    {
        in_memory.set_value(in_position, in_memory.get_value(in_position)+1);
    }
    template<typename MemoryType>
    constexpr void decrease_value(
            MemoryType& in_memory,
            const typename MemoryType::position_type& in_position)
    {
        in_memory.set_value(in_position, in_memory.get_value(in_position)-1);
    }
}

#endif // CHANGEVALUEFUNCTIONS_HPP_INCLUDED
