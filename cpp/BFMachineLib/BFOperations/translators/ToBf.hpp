#ifndef TOBF_HPP_INCLUDED
#define TOBF_HPP_INCLUDED

#include <string>
#include <sstream>
#include "../BFOperations.hpp"

namespace bfm::bfo::translators
{
    namespace inner
    {
        template <typename T>
        T abs(const T& in_val)
        {
            return in_val >= T(0) ? in_val : -in_val;
        }
    }
    template <typename DataChangeType, typename InstructionSet>
    class [[nodiscard]] ToBf
    {
        public:
            using bf_block = BFBlock<DataChangeType>;
            using bf_simple_loop = BFSimpleLoop<DataChangeType>;
            using bf_loop = BFLoop<DataChangeType>;
        private:
            template <typename Value, typename Symbol>
            [[nodiscard]] static auto repeat_symbol(
                    const Value& in_val,
                    const Symbol& negative_symbol, const Symbol& positive_symbol)
            {
                const auto cur_symbol = in_val < Value(0) ? negative_symbol : positive_symbol;
                return std::string(std::size_t(inner::abs(in_val)), cur_symbol);
            }

            template <typename ShiftType>
            [[nodiscard]] static constexpr auto shift_to_bf(const ShiftType& in_shift)
            {
                return repeat_symbol(
                    in_shift, InstructionSet::move_left, InstructionSet::move_right);
            }

            template <typename ValueType>
            [[nodiscard]] static constexpr auto value_change_to_bf(const ValueType& in_value_change)
            {
                return repeat_symbol(
                    in_value_change,
                    InstructionSet::decrease_value, InstructionSet::increase_value);
            }

            [[nodiscard]] static auto data_change_to_bf(const DataChangeType& in_data_change)
            {
                std::stringstream ss;
                decltype(in_data_change.total_shift) cur_total_shift = 0;
                for (const auto& [cur_shift, cur_value_change] : in_data_change.memory_change)
                {
                    const auto shift_to_print = cur_shift-cur_total_shift;
                    ss<<ToBf::shift_to_bf(shift_to_print)
                        <<ToBf::value_change_to_bf(cur_value_change);
                    cur_total_shift += shift_to_print;
                }
                ss<<ToBf::shift_to_bf(in_data_change.total_shift-cur_total_shift);
                return ss.str();
            }
        public:
            [[nodiscard]] static constexpr auto print(const BFWrite&)
            {
                return InstructionSet::print_value;
            }

            [[nodiscard]] static constexpr auto print(const BFRead&)
            {
                return InstructionSet::read_value;
            }

            [[nodiscard]] static auto print(const ToBf::bf_block& in_operation)
            {
                return ToBf::data_change_to_bf(in_operation.data_change);
            }

            [[nodiscard]] static auto print(const ToBf::bf_simple_loop& in_operation)
            {
                std::stringstream ss;
                ss<<InstructionSet::begin_loop
                    <<ToBf::data_change_to_bf(in_operation.data_change)
                    <<InstructionSet::end_loop;
                return ss.str();
            }

            [[nodiscard]] static auto print(const ToBf::bf_loop& in_operation)
            {
                std::stringstream ss;
                ss<<InstructionSet::begin_loop
                    <<ToBf::print(in_operation.operation_seq)
                    <<InstructionSet::end_loop;
                return ss.str();
            }

            [[nodiscard]] static std::string print(
                    const bfm::bfo::operation_seq_type<DataChangeType>& in_bf_operation_seq)
            {
                std::stringstream ss;
                for (const auto& cur_operation : in_bf_operation_seq)
                {
                    std::visit([&ss](const auto x){ss<<ToBf::print(x);}, cur_operation);
                }
                return ss.str();
            }
    };
}

#endif // TOBF_HPP_INCLUDED
