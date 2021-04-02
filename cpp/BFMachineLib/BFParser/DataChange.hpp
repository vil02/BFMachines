#ifndef DATACHANGE_HPP_INCLUDED
#define DATACHANGE_HPP_INCLUDED

#include <utility>

namespace bfm::parser
{
    template <typename MemoryChangeType>
    struct [[nodiscard]] DataChange
    {
        MemoryChangeType memory_change;
        using memory_change_type = MemoryChangeType;
        using shift_type = typename memory_change_type::key_type;
        using value_change_type = typename memory_change_type::mapped_type;
        DataChange::shift_type total_shift;
        DataChange(const MemoryChangeType in_memory_change, const shift_type& in_total_shift) :
            memory_change(std::move(in_memory_change)),
            total_shift(in_total_shift)
        {}
    };
}

#endif // DATACHANGE_HPP_INCLUDED
