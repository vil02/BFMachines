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
    template <typename DataChangeType, typename BFMData>
    constexpr void check()
    {
        static_assert(std::is_same<
            typename DataChangeType::position_type, typename BFMData::position_type>::value);
        static_assert(
            std::is_same<
                typename DataChangeType::value_type,
                typename std::make_signed<typename BFMData::value_type>::type>::value);
    }
}
namespace bfm::bfo
{
    template <typename BFOperationSeq, typename BFMData>
    void execute_seq(const BFOperationSeq& in_bf_operation_seq, BFMData& bfm_data)
    {
        for (const auto& cur_operation : in_bf_operation_seq)
        {
            std::visit([&bfm_data](const auto x){x.execute(bfm_data);}, cur_operation);
        }
    }

    struct [[nodiscard]] BFWrite
    {
        template<typename BFMData>
        constexpr static void execute(BFMData& bfm_data)
        {
            bfm_data.output_stream<<bfm_data.memory.get_value(bfm_data.cur_position);
        }
    };

    struct [[nodiscard]] BFRead
    {
        template<typename BFMData>
        constexpr static void execute(BFMData& bfm_data)
        {
            using value_type = typename BFMData::value_type;
            value_type new_value = value_type();
            bfm_data.input_stream>>new_value;
            bfm_data.memory.set_value(bfm_data.cur_position, new_value);
        }
    };

    template<typename DataChangeType>
    struct [[nodiscard]] BFBlock
    {
        DataChangeType data_change;
        explicit constexpr BFBlock(DataChangeType in_data_change) :
            data_change(std::move(in_data_change))
        {}
        template<typename BFMData>
        constexpr void execute(BFMData& bfm_data) const
        {
            inner::check<DataChangeType, BFMData>();
            for (const auto& [cur_shift, cur_value_change] : this->data_change.memory_change)
            {
                change_value(bfm_data.memory, bfm_data.cur_position+cur_shift, cur_value_change);
            }
            bfm_data.cur_position += this->data_change.total_shift;
        }
    };

    template<typename DataChangeType>
    struct [[nodiscard]] BFSimpleLoop
    {
        DataChangeType data_change;
        explicit constexpr BFSimpleLoop(const DataChangeType& in_data_change) :
            data_change(in_data_change)
        {
            if (in_data_change.total_shift != 0)
            {
                throw std::invalid_argument("total_shift has to be 0.");
            }
            if (in_data_change.memory_change.empty())
            {
                throw std::invalid_argument("memory_change can not be trivial.");
            }
            if (in_data_change.memory_change.find(0) == in_data_change.memory_change.end())
            {
                throw std::invalid_argument(
                    "memory_change at cur_pos/with zero shift can not be trivial.");
            }
        }
        template<typename BFMData>
        constexpr void execute(BFMData& bfm_data) const
        {
            inner::check<DataChangeType, BFMData>();
            const auto cur_value = bfm_data.memory.get_value(bfm_data.cur_position);
            if (cur_value != 0)
            {
                using value_change_type = typename DataChangeType::value_type;
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
                const value_change_type multiplier =
                    value_change_type(cur_value)/
                    value_change_type((-this->data_change.memory_change.at(0)));
                for (const auto& [cur_shift, cur_value_change] : this->data_change.memory_change)
                {
                    if (cur_shift != 0)
                    {
                        change_value(
                            bfm_data.memory,
                            bfm_data.cur_position+cur_shift,
                            multiplier*cur_value_change);
                    }
                }
                bfm_data.memory.set_value(bfm_data.cur_position, 0);
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
        template <typename BFMData>
        constexpr void execute(BFMData& bfm_data) const
        {
            inner::check<DataChangeType, BFMData>();
            while (bfm_data.memory.get_value(bfm_data.cur_position) != 0)
            {
                execute_seq<operation_seq_type, BFMData>(this->operation_seq, bfm_data);
            }
        }
    };

    template <typename DataChangeType>
    using variant_type = typename BFLoop<DataChangeType>::variant_type;

    template <typename DataChangeType>
    using operation_seq_type = typename BFLoop<DataChangeType>::operation_seq_type;
}
#endif // BFOPERATIONS_HPP_INCLUDED
