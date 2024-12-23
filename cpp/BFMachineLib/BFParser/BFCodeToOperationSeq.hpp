#ifndef BFCODETOOPERATIONSEQ_HPP_INCLUDED
#define BFCODETOOPERATIONSEQ_HPP_INCLUDED

#include "../BFOperations/BFOperations.hpp"
#include "../StandardInstructions/StandardInstructions.hpp"
#include "./inner/RawParserRelated.hpp"
#include "DataChange.hpp"

#include <map>
#include <type_traits>
#include <vector>
#include <string>

namespace bfm::parser::general_parser
{
    template <typename CodeType,
              typename DataChangeType,
              typename InstructionSet,
              bool MakeShrinkToFit>
    [[nodiscard]] bfm::bfo::operation_seq_type<DataChangeType> bf_code_to_operation_seq(
            const CodeType& in_bf_code)
    {
        static_assert(std::is_same_v<
            typename CodeType::value_type, typename InstructionSet::instruction_type>);
        bfm::bfo::operation_seq_type<DataChangeType> res;
        typename CodeType::size_type cur_code_pos = 0;
        while (cur_code_pos < in_bf_code.size())
        {
            if (inner::is_simple<InstructionSet>(in_bf_code[cur_code_pos]))
            {
                decltype(cur_code_pos) cur_len = 1;
                while (cur_code_pos+cur_len < in_bf_code.size() &&
                       inner::is_simple<InstructionSet>(in_bf_code[cur_code_pos+cur_len]))
                {
                    ++cur_len;
                }
                const auto cur_operation =
                    bfm::bfo::BFBlock<DataChangeType>(
                        inner::proc_str_piece<CodeType, DataChangeType, InstructionSet>(
                            in_bf_code.substr(cur_code_pos, cur_len)));
                res.push_back(cur_operation);
                cur_code_pos += cur_len;
            }
            else
            {
                switch (in_bf_code[cur_code_pos])
                {
                    case InstructionSet::print_value:
                        res.push_back(bfm::bfo::variant_type<DataChangeType>(bfm::bfo::BFWrite()));
                        ++cur_code_pos;
                        break;
                    case InstructionSet::read_value:
                        res.push_back(bfm::bfo::variant_type<DataChangeType>(bfm::bfo::BFRead()));
                        ++cur_code_pos;
                        break;
                    case InstructionSet::begin_loop:
                        {
                            std::string new_branch;
                            inner::extract_branch<std::string, InstructionSet>(
                                std::string(in_bf_code), cur_code_pos, new_branch);
                            const auto block_data =
                                inner::proc_str_piece<CodeType, DataChangeType, InstructionSet>(
                                    new_branch);
                            if (const auto is_simple_loop =
                                inner::is_branch_simple<decltype(new_branch), InstructionSet>(
                                    new_branch)
                                && block_data.total_shift == 0
                                && !block_data.memory_change.empty(); is_simple_loop)
                            {
                                res.push_back(bfm::bfo::variant_type<DataChangeType>(
                                    bfm::bfo::BFSimpleLoop<DataChangeType>(block_data)));
                            }
                            else
                            {
                                const auto cur_bf_to_op_seq = bf_code_to_operation_seq<
                                            CodeType,
                                            DataChangeType,
                                            InstructionSet,
                                            MakeShrinkToFit>;
                                res.push_back(bfm::bfo::variant_type<DataChangeType>(
                                    bfm::bfo::BFLoop<DataChangeType>(
                                        cur_bf_to_op_seq(new_branch))));
                            }
                            break;
                        }
                    default:
                        ++cur_code_pos;
                }
            }
        }
        if constexpr (MakeShrinkToFit)
        {
            res.shrink_to_fit();
        }
        return res;
    }
}

namespace bfm::parser
{
    template <typename CodeType,
              typename ShiftType,
              typename ValueChangeType,
              typename InstructionSet = bfm::StandardInstructions,
              bool MakeShrinkToFit = true>
    const auto bf_code_to_operation_seq = general_parser::bf_code_to_operation_seq<
        CodeType,
        bfm::parser::DataChange<std::map<ShiftType, ValueChangeType> >,
        InstructionSet,
        MakeShrinkToFit>;
}

#endif // BFCODETOOPERATIONSEQ_HPP_INCLUDED
