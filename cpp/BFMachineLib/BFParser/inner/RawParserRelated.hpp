#ifndef RAWPARSERRELATED_HPP_INCLUDED
#define RAWPARSERRELATED_HPP_INCLUDED

#include "BfOperationDataChangeRelated.hpp"

#include <type_traits>
#include <exception>
#include <stdexcept>
#include <algorithm>

namespace bfm::parser::inner
{
    template<typename InstructionSet>
    [[nodiscard]] constexpr bool is_simple(
            const typename InstructionSet::instruction_type& in_instruction)
    {
        return in_instruction == InstructionSet::move_left ||
            in_instruction == InstructionSet::move_right ||
            in_instruction == InstructionSet::increase_value ||
            in_instruction == InstructionSet::decrease_value;
    }

    template<typename CodeType, typename InstructionSet>
    [[nodiscard]] constexpr bool is_branch_simple(const CodeType& in_str)
    {
        using instruction_type = typename InstructionSet::instruction_type;
        static_assert(std::is_same_v<typename CodeType::value_type, instruction_type>);

        const auto is_instruction =
            [](const instruction_type& in_char)
            {
                return is_simple<InstructionSet>(in_char) ||
                    in_char == InstructionSet::begin_loop ||
                    in_char == InstructionSet::end_loop ||
                    in_char == InstructionSet::read_value ||
                    in_char == InstructionSet::print_value;
            };
        const auto check_single =
            [&is_instruction](const instruction_type& in_char)
            {
                return !is_instruction(in_char) || is_simple<InstructionSet>(in_char);
            };
        return std::all_of(in_str.begin(), in_str.end(), check_single);
    }

    template<typename CodeType, typename InstructionSet>
    constexpr void extract_branch(
            const CodeType& in_str,
            typename CodeType::size_type& code_pos, CodeType& out_str)
    {
        using code_position_type = typename CodeType::size_type;
        code_position_type tmp_code_pos =
            bfm::find_matching<CodeType, InstructionSet>(in_str, code_pos);
        out_str = in_str.substr(code_pos+1, tmp_code_pos-code_pos-1);
        code_pos = tmp_code_pos+1;
    }
}

#endif // RAWPARSERRELATED_HPP_INCLUDED
