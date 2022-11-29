#ifndef BFOPERATIONS_HPP_INCLUDED
#define BFOPERATIONS_HPP_INCLUDED

#include <stdexcept>
#include <variant>
#include <vector>
#include <utility>
#include <type_traits>

#include "BFOperationsUtilFunctions.hpp"

namespace bfm::bfo::inner
{
    template <typename DataChangeType, typename BFMState>
    constexpr void check()
    {
        static_assert(std::is_same_v<
            typename DataChangeType::shift_type,
            typename std::make_signed_t<typename BFMState::position_type> >);
        static_assert(std::is_same_v<
            typename DataChangeType::value_change_type,
            typename std::make_signed_t<typename BFMState::value_type> >);
    }
}
namespace bfm::bfo
{
    template <typename BFOperationSeq, typename BFMState>
    void execute_seq(const BFOperationSeq& in_bf_operation_seq, BFMState& bfm_state)
    {
        for (const auto& cur_operation : in_bf_operation_seq)
        {
            std::visit([&bfm_state](const auto x){x.execute(bfm_state);}, cur_operation);
        }
    }

    struct [[nodiscard]] BFWrite
    {
        template<typename BFMState>
        constexpr static void execute(BFMState& bfm_state)
        {
            bfm_state.print_value();
        }
    };

    struct [[nodiscard]] BFRead
    {
        template<typename BFMState>
        constexpr static void execute(BFMState& bfm_state)
        {
            bfm_state.read_value();
        }
    };

    template<typename DataChangeType>
    struct [[nodiscard]] BFBlock
    {
        DataChangeType data_change;
        using shift_type = typename DataChangeType::shift_type;
        explicit constexpr BFBlock(DataChangeType in_data_change) :
            data_change(std::move(in_data_change))
        {}
        template<typename BFMState>
        constexpr void execute(BFMState& bfm_state) const
        {
            inner::check<DataChangeType, BFMState>();
            for (const auto& [cur_shift, cur_value_change] : this->data_change.memory_change)
            {
                bfm_state.change_value_at_shift(cur_value_change, cur_shift);
            }
            bfm_state.shift_position(this->data_change.total_shift);
        }
    };

    template<typename DataChangeType>
    struct [[nodiscard]] BFSimpleLoop
    {
        DataChangeType data_change;
        using shift_type = typename DataChangeType::shift_type;
        explicit constexpr BFSimpleLoop(const DataChangeType& in_data_change) :
            data_change(in_data_change)
        {
            if (in_data_change.total_shift != 0)
            {
                throw std::invalid_argument("total_shift has to be 0.");
            }
            if (in_data_change.memory_change.empty())
            {
                throw std::invalid_argument("memory_change cannot be trivial.");
            }
            if (in_data_change.memory_change.find(0) == in_data_change.memory_change.end())
            {
                throw std::invalid_argument(
                    "memory_change at cur_pos/with zero shift cannot be trivial.");
            }
        }
        template<typename BFMState>
        constexpr void execute(BFMState& bfm_state) const
        {
            inner::check<DataChangeType, BFMState>();
            const auto cur_value = bfm_state.get_cur_value();
            if (cur_value != 0)
            {
                using value_change_type = typename DataChangeType::value_change_type;
                if (util::mod(
                        value_change_type(cur_value),
                        this->data_change.memory_change.at(0)) != 0)
                {
                    throw std::invalid_argument(
                        "cur_value must be a multiple of value_change with 0 shift");
                }
                if (value_change_type(cur_value)*this->data_change.memory_change.at(0) >= 0)
                {
                    throw std::invalid_argument("different signs needed!");
                }
                const auto multiplier = value_change_type(
                    value_change_type(cur_value)/-this->data_change.memory_change.at(0));
                for (const auto& [cur_shift, cur_value_change] : this->data_change.memory_change)
                {
                    if (cur_shift != 0)
                    {
                        bfm_state.change_value_at_shift(
                              value_change_type(multiplier*cur_value_change),
                              cur_shift);
                    }
                }
                bfm_state.set_value(0);
            }
        }
    };

    template<typename DataChangeType>
    struct [[nodiscard]] BFLoop
    {
        using variant_type = typename std::variant<
            BFRead,
            BFWrite,
            BFBlock<DataChangeType>,
            BFSimpleLoop<DataChangeType>,
            BFLoop<DataChangeType> >;
        using operation_seq_type = typename std::vector<variant_type>;
        operation_seq_type operation_seq;
        explicit constexpr BFLoop(operation_seq_type in_operation_seq):
            operation_seq(std::move(in_operation_seq))
        {}
        template <typename BFMState>
        constexpr void execute(BFMState& bfm_state) const
        {
            inner::check<DataChangeType, BFMState>();
            while (bfm_state.is_current_value_not_zero())
            {
                execute_seq<operation_seq_type, BFMState>(this->operation_seq, bfm_state);
            }
        }
    };

    template <typename DataChangeType>
    using variant_type = typename BFLoop<DataChangeType>::variant_type;

    template <typename DataChangeType>
    using operation_seq_type = typename BFLoop<DataChangeType>::operation_seq_type;
}
#endif // BFOPERATIONS_HPP_INCLUDED
