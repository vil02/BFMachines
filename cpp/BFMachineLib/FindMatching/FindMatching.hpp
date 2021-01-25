#ifndef FINDMATCHING_HPP_INCLUDED
#define FINDMATCHING_HPP_INCLUDED

#include <stdexcept>
#include <exception>

namespace bfm
{
    template <typename CodeType, typename InstructionSet>
    [[nodiscard]] constexpr typename CodeType::size_type find_matching(
            const CodeType& in_code,
            const typename CodeType::size_type start_pos)
    {
        static_assert(
            std::is_same<typename CodeType::value_type,
                         typename InstructionSet::instruction_type>::value);
        if (in_code[start_pos] != InstructionSet::begin_loop &&
            in_code[start_pos] != InstructionSet::end_loop)
        {
            throw std::invalid_argument("in_code[start_pos] must begin or end loop symbol.");
        }
        const auto target_char =
            in_code[start_pos] == InstructionSet::begin_loop ?
            InstructionSet::end_loop : InstructionSet::begin_loop;
        const typename CodeType::difference_type search_dir =
            in_code[start_pos] == InstructionSet::begin_loop ? 1 : -1;

        typename CodeType::difference_type cur_count = 0;
        typename CodeType::size_type cur_pos = start_pos+search_dir;
        while (cur_pos < in_code.size())
        {
            if (cur_count == 0 && in_code[cur_pos] == target_char)
            {
                break;
            }
            else if (in_code[cur_pos] == InstructionSet::begin_loop)
            {
                ++cur_count;
            }
            else if (in_code[cur_pos] == InstructionSet::end_loop)
            {
                --cur_count;
            }
            cur_pos += search_dir;
        }
        if (cur_pos >= in_code.size())
        {
            throw std::invalid_argument("Could not find matching loop symbol.");
        }
        return cur_pos;
    }
}

#endif // FINDMATCHING_HPP_INCLUDED
