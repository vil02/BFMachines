#ifndef TOBF_HPP_INCLUDED
#define TOBF_HPP_INCLUDED

#include <string>
#include <sstream>
#include <cmath>
#include "../BFOperations.hpp"
//namespace bfm::bfo::translators::to_bf
namespace bfm::bfo
{
    template <typename DataChangeType, typename InstructionSet>
    struct ToBf
    {
        using bf_block = BFBlock<DataChangeType>;
        using bf_simple_loop = BFSimpleLoop<DataChangeType>;
        using bf_loop = BFLoop<DataChangeType>;

        template <typename Value, typename Symbol>
        static [[nodiscard]] auto repeat_symbol(
                const Value& in_val,
                const Symbol& negative_symbol, const Symbol& positive_symbol)
        {
            const auto cur_symbol = in_val < 0 ? negative_symbol : positive_symbol;
            return std::string(abs(in_val), cur_symbol);
        }

        template <typename ShiftType>
        static constexpr [[nodiscard]] auto shift_to_bf(const ShiftType& in_shift)
        {
            return repeat_symbol(in_shift, InstructionSet::move_left, InstructionSet::move_right);
        }

        template <typename ValueType>
        static constexpr [[nodiscard]] auto value_change_to_bf(const ValueType& in_value_change)
        {
            return repeat_symbol(
                in_value_change, InstructionSet::decrease_value, InstructionSet::increase_value);
        }

        static constexpr [[nodiscard]] auto to_bf(const BFWrite& in_operation)
        {
            return InstructionSet::print_value;
        }
        static constexpr [[nodiscard]] auto to_bf(const BFRead& in_operation)
        {
            return InstructionSet::read_value;
        }
        static [[nodiscard]] auto data_change_to_bf(const DataChangeType& in_data_change)
        {
            std::stringstream ss;
            decltype(in_data_change.total_shift) printed_shift = 0;
            for (const auto& [cur_shift, cur_value_change] : in_data_change.memory_change)
            {
                ss<<ToBf::shift_to_bf(cur_shift)<<ToBf::shift_to_bf(cur_value_change);
                printed_shift += cur_shift;
            }
            ss<<ToBf::shift_to_bf(in_data_change.total_shift-printed_shift);
            return ss.str();
        }

        static [[nodiscard]] auto to_bf(const ToBf::bf_block& in_operation)
        {
            return ToBf::data_change_to_bf(in_operation.data_change);
        }

        static [[nodiscard]] auto to_bf(const ToBf::bf_simple_loop& in_operation)
        {
            std::stringstream ss;
            ss<<InstructionSet::begin_loop
                <<ToBf::data_change_to_bf(in_operation.data_change)
                <<InstructionSet::end_loop;
            return ss.str();
        }
        static [[nodiscard]] auto to_bf(const ToBf::bf_loop& in_operation)
        {
            std::stringstream ss;
            ss<<InstructionSet::begin_loop
                <<ToBf::seq_to_bf(in_operation.operation_seq)
                <<InstructionSet::end_loop;
            return ss.str();
        }
        static [[nodiscard]] std::string seq_to_bf(const bfm::bfo::operation_seq_type<DataChangeType>& in_bf_operation_seq)
        {
            std::stringstream ss;
            for (const auto& cur_operation : in_bf_operation_seq)
            {
                std::visit([&ss](const auto x){ss<<to_bf(x);}, cur_operation);
            }
            return ss.str();
        }
    };
}

#endif // TOBF_HPP_INCLUDED
