#ifndef BFOPERATIONDATACHANGERELATED_HPP_INCLUDED
#define BFOPERATIONDATACHANGERELATED_HPP_INCLUDED

#include <type_traits>

namespace bfm::parser::inner
{
    template <typename MemoryChangeType>
    struct [[nodiscard]] DataChange
    {
        MemoryChangeType memory_change;
        using memory_change_type = MemoryChangeType;
        using position_type = typename memory_change_type::key_type;
        using value_type = typename memory_change_type::mapped_type;
        DataChange::position_type total_shift;
        DataChange(const MemoryChangeType in_memory_change, const position_type& in_total_shift) :
            memory_change(in_memory_change),
            total_shift(in_total_shift)
        {}
    };

    template <typename MemoryChangeType>
    void update_memory_change(
            MemoryChangeType& memory_change,
            const typename MemoryChangeType::key_type cur_shift,
            const typename MemoryChangeType::mapped_type& value_change)
    {
        memory_change[cur_shift] += value_change;
        if (memory_change[cur_shift] == 0)
        {
            memory_change.erase(cur_shift);
        }
    }

    template <typename CodeType, typename DataChangeType, typename InstructionSet>
    [[nodiscard]] DataChangeType proc_str_piece(const CodeType& in_str)
    {
        static_assert(std::is_same<
            typename CodeType::value_type, typename InstructionSet::instruction_type>::value);
        typename DataChangeType::position_type cur_shift = 0;
        typename DataChangeType::memory_change_type memory_change;
        for (const auto cur_char : in_str)
        {
            switch (cur_char)
            {
                case InstructionSet::move_right:
                    ++cur_shift;
                    break;
                case InstructionSet::move_left:
                    --cur_shift;
                    break;
                case InstructionSet::increase_value:
                    update_memory_change(memory_change, cur_shift, 1);
                    break;
                case InstructionSet::decrease_value:
                    update_memory_change(memory_change, cur_shift, -1);
                    break;
                default:
                    break;
            }
        }
        return DataChangeType(memory_change, cur_shift);
    }
}

#endif // BFOPERATIONDATACHANGERELATED_HPP_INCLUDED
