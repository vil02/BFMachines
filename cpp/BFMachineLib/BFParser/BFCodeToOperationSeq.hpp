#ifndef BFCODETOOPERATIONSEQ_HPP_INCLUDED
#define BFCODETOOPERATIONSEQ_HPP_INCLUDED

#include <vector>
#include <map>
#include <type_traits>

#include "../BFOperations/BFOperations.hpp"
#include "./inner/RawParserRelated.hpp"
namespace bfm::parser::general_parser
{
    template <typename CodeType,
              typename DataChangeType,
              typename InstructionSet,
              bool MakeShrinkToFit>
    [[nodiscard]] std::vector<bfm::bfo::variant_type<DataChangeType> > bf_code_to_operation_seq(
            const CodeType& in_bf_code)
    {
        static_assert(std::is_same<
            typename CodeType::value_type, typename InstructionSet::instruction_type>::value);
        std::vector<bfm::bfo::variant_type<DataChangeType> > res;
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
                bfm::bfo::variant_type<DataChangeType> cur_operation =
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
                            bool is_simple_loop =
                                inner::is_branch_simple<decltype(new_branch), InstructionSet>(
                                    new_branch);
                            const auto block_data =
                                inner::proc_str_piece<CodeType, DataChangeType, InstructionSet>(
                                    new_branch);
                            if (is_simple_loop)
                            {
                                if (block_data.total_shift != 0 ||
                                    block_data.memory_change.size() == 0)
                                {
                                    is_simple_loop = false;
                                }
                            }
                            if (is_simple_loop)
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
              typename ValueType,
              typename PositionType,
              typename InstructionSet = bfm::StandardInstructions,
              bool MakeShrinkToFit = true>
    const auto bf_code_to_operation_seq = general_parser::bf_code_to_operation_seq<
        CodeType,
        bfm::parser::DataChange<std::map<ValueType, PositionType> >,
        InstructionSet,
        MakeShrinkToFit>;
}

#endif // BFCODETOOPERATIONSEQ_HPP_INCLUDED
