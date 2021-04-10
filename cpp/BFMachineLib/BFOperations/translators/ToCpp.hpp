#ifndef TOCPP_HPP_INCLUDED
#define TOCPP_HPP_INCLUDED

#include <type_traits>
#include <string>
#include <sstream>
#include <map>

#include "../BFOperations.hpp"
#include "../../BFParser/DataChange.hpp"
#include "../../StandardInstructions/StandardInstructions.hpp"

namespace bfm::bfo::translators::general_to_cpp
{
    struct [[nodiscard]] StandardCppNameSet
    {
        using name_type = const char * const;
        static const constexpr name_type memory = "memory";
        static const constexpr name_type cur_pos = "cur_position";
        static const constexpr name_type input_stream = "input_stream";
        static const constexpr name_type output_stream = "output_stream";
        static const constexpr name_type input_value = "input_value";
        static const constexpr name_type value_change_type = "value_change_type";
        static const constexpr name_type multiplier = "multiplier";
    };

    template <std::size_t TabSize = 4>
    constexpr auto indent(const std::size_t in_depth)
    {
        return std::string(in_depth*TabSize, ' ');
    }

    template <typename DataChangeType,
              typename NameSet>
    class [[nodiscard]] ToCpp
    {
        using bf_block = BFBlock<DataChangeType>;
        using bf_simple_loop = BFSimpleLoop<DataChangeType>;
        using bf_loop = BFLoop<DataChangeType>;
        [[nodiscard]] static auto get_cur_val()
        {
            std::stringstream ss;
            ss<<NameSet::memory<<".get_value("<<NameSet::cur_pos<<")";
            return ss.str();
        };
        template <typename ValueType>
        [[nodiscard]] static auto set_cur_val(const ValueType& in_val)
        {
            std::stringstream ss;
            ss<<NameSet::memory<<".set_value("<<NameSet::cur_pos<<", "<<in_val<<")";
            return ss.str();
        }
        template <typename PositionType, typename ChangeType>
        [[nodiscard]] static auto change_val_str(
                const PositionType& in_pos, const ChangeType& in_value_change)
        {
            std::stringstream ss;
            ss<<"change_value("<<NameSet::memory<<", "<<in_pos<<", "<<in_value_change<<")";
            return ss.str();
        }

        template <typename ShiftType>
        [[nodiscard]] static auto cur_pos_shift_to_str(const ShiftType& in_shift)
        {
            std::stringstream ss;
            ss<<NameSet::cur_pos;
            if (in_shift > 0)
            {
                ss<<"+"<<in_shift;
            }
            else if (in_shift < 0)
            {
                ss<<"-"<<-in_shift;
            }
            return ss.str();
        }

        template <typename ShiftType>
        [[nodiscard]] static auto print_change_cur_pos(
                const ShiftType& in_shift, const std::size_t in_depth = 0)
        {
            std::stringstream ss;
            if(in_shift != 0)
            {
                ss<<indent(in_depth);
                if (in_shift > 0)
                {
                    if (in_shift == 1)
                    {
                       ss<<"++"<<NameSet::cur_pos<<";\n";
                    }
                    else
                    {
                        ss<<NameSet::cur_pos<<" += "<<in_shift<<";\n";
                    }
                }
                else if (in_shift < 0)
                {
                    if (in_shift == -1)
                    {
                       ss<<"--"<<NameSet::cur_pos<<";\n";
                    }
                    else
                    {
                        ss<<NameSet::cur_pos<<" -= "<<-in_shift<<";\n";
                    }
                }
            }
            return ss.str();
        }
        template <typename ValueType>
        [[nodiscard]] static auto multip_str(const ValueType& in_cur_pos_value_change)
        {
            std::stringstream ss;
            ss<<"const auto "<<NameSet::multiplier<<" = ";
            if (in_cur_pos_value_change == 1)
            {
                ss<<NameSet::value_change_type<<"(-"<<ToCpp::get_cur_val()<<")";
            }
            else if (in_cur_pos_value_change == -1)
            {
                ss<<NameSet::value_change_type<<"("<<ToCpp::get_cur_val()<<")";
            }
            else
            {
                ss<<NameSet::value_change_type<<"("
                    <<ToCpp::get_cur_val()<<")/"
                    <<NameSet::value_change_type<<"("<<-in_cur_pos_value_change<<")";
            }
            ss<<";\n";
            return ss.str();
        }
        template <typename ValueType>
        [[nodiscard]] static auto multip_value_change(const ValueType& in_value)
        {
            std::stringstream ss;
            if (in_value > 0)
            {
                if (in_value == 1)
                {
                    ss<<NameSet::multiplier;
                }
                else
                {
                    ss<<NameSet::value_change_type<<"("<<NameSet::multiplier<<"*"<<in_value<<")";
                }
            }
            else
            {
                if (in_value == -1)
                {
                    ss<<"-"<<NameSet::multiplier;
                }
                else
                {
                    ss<<NameSet::value_change_type<<"("<<NameSet::multiplier<<"*("<<in_value<<"))";
                }
            }
            return ss.str();
        }

        public:
            [[nodiscard]] static auto print(
                    const BFWrite& /*unused*/, const std::size_t in_depth = 0)
            {
                std::stringstream ss;
                ss<<indent(in_depth)<<NameSet::output_stream<<"<<"<<get_cur_val()<<";\n";
                return ss.str();
            }

            [[nodiscard]] static auto print(
                    const BFRead& /*unused*/, const std::size_t in_depth = 0)
            {
                std::stringstream ss;
                ss<<indent(in_depth)<<"{\n"
                    <<indent(in_depth+1)
                    <<"typename "<<"decltype("<<NameSet::memory<<")"<<"::value_type "
                    <<NameSet::input_value<<" = {};\n"
                    <<indent(in_depth+1)<<NameSet::input_stream<<">>"<<NameSet::input_value<<";\n"
                    <<indent(in_depth+1)<<set_cur_val(NameSet::input_value)<<";\n"
                    <<indent(in_depth)<<"}\n";
                return ss.str();
            }

            [[nodiscard]] static auto print(
                    const ToCpp::bf_block& in_operation, const std::size_t in_depth = 0)
            {
                std::stringstream ss;
                for (const auto& [cur_shift, cur_value_change] :
                     in_operation.data_change.memory_change)
                {
                    ss<<indent(in_depth)
                        <<ToCpp::change_val_str(cur_pos_shift_to_str(cur_shift), cur_value_change)
                        <<";\n";
                }
                ss<<ToCpp::print_change_cur_pos(in_operation.data_change.total_shift, in_depth);
                return ss.str();
            }

            [[nodiscard]] static auto print(
                    const ToCpp::bf_simple_loop& in_operation, const std::size_t in_depth = 0)
            {
                std::stringstream ss;
                if (in_operation.data_change.memory_change.size() > 1)
                {
                    ss<<indent(in_depth)<<"if ("<<ToCpp::get_cur_val()<<" != 0)\n"
                        <<indent(in_depth)<<"{\n"
                        <<indent(in_depth+1)<<"using "<<NameSet::value_change_type
                        <<" = typename std::make_signed<typename decltype("
                        <<NameSet::memory<<")::value_type>::type;\n"
                        <<indent(in_depth+1)
                        <<ToCpp::multip_str(in_operation.data_change.memory_change.at(0));
                    for (const auto& [cur_shift, cur_value_change] :
                         in_operation.data_change.memory_change)
                    {
                        if (cur_shift != 0)
                        {
                            ss<<indent(in_depth+1)<<ToCpp::change_val_str(
                                    cur_pos_shift_to_str(cur_shift),
                                    multip_value_change(cur_value_change))
                                <<";\n";
                        }
                        else
                        {
                            ss<<indent(in_depth+1)<<set_cur_val(0)<<";\n";
                        }
                    }
                    ss<<ToCpp::print_change_cur_pos(in_operation.data_change.total_shift, in_depth)
                        <<indent(in_depth)<<"}\n";
                }
                else
                {
                    ss<<indent(in_depth)<<set_cur_val(0)<<";\n";
                }
                return ss.str();
            }

            [[nodiscard]] static auto print(
                    const ToCpp::bf_loop& in_operation, const std::size_t in_depth = 0)
            {
                std::stringstream ss;
                ss<<indent(in_depth)<<"while ("<<get_cur_val()<<" != 0)\n"
                    <<indent(in_depth)<<"{\n"
                    <<ToCpp::print(in_operation.operation_seq, in_depth+1)
                    <<indent(in_depth)<<"}\n";
                return ss.str();
            }

            [[nodiscard]] static std::string print(
                    const bfm::bfo::operation_seq_type<DataChangeType>& in_bf_operation_seq,
                    const std::size_t in_depth = 0)
            {
                std::stringstream ss;
                for (const auto& cur_operation : in_bf_operation_seq)
                {
                    std::visit(
                        [&ss, &in_depth](const auto x) -> void {ss<<ToCpp::print(x, in_depth);},
                        cur_operation);
                }
                return ss.str();
            }
            [[nodiscard]] static std::string print_as_function(
                const bfm::bfo::operation_seq_type<DataChangeType>& in_bf_operation_seq,
                const std::string& function_name)
            {
                const auto bf_code = bfm::bfo::translators::ToBf<>::print(in_bf_operation_seq);
                std::stringstream ss;
                ss<<"template <typename MemoryType, "
                    <<"typename InputStreamType, "
                    <<"typename OutputStreamType>\n"
                    <<"void "<<function_name
                    <<"(InputStreamType& ";
                if (bf_code.find(bfm::StandardInstructions::read_value) != std::string::npos)
                {
                    ss<<NameSet::input_stream;
                }
                else
                {
                   ss<<"/*unused*/";
                }
                ss<<", OutputStreamType& ";
                if (bf_code.find(bfm::StandardInstructions::print_value) != std::string::npos)
                {
                    ss<<NameSet::output_stream;
                }
                else
                {
                   ss<<"/*unused*/";
                }
                ss<<")\n{\n"
                    <<indent(1)<<"auto "<<NameSet::memory<<" = MemoryType();\n"
                    <<indent(1)<<"auto "<<NameSet::cur_pos
                    <<" = "<<NameSet::memory<<".get_starting_position();\n"
                    <<ToCpp::print(in_bf_operation_seq, 1)
                    <<"}\n\n";
                return ss.str();
            }
            [[nodiscard]] static std::string print_as_function_in_header(
                const bfm::bfo::operation_seq_type<DataChangeType>& in_bf_operation_seq,
                const std::string& function_name)
            {
                std::stringstream ss;
                const auto include_guard =
                    function_name+"_BF_TO_CPP_AUTO_GEN_HPP_INCLUDED";
                ss<<"#ifndef "<<function_name<<include_guard<<'\n'
                    <<"#define "<<function_name<<include_guard<<"\n\n"
                    <<ToCpp::print_as_function(in_bf_operation_seq, function_name)
                    <<"#endif\n";
                return ss.str();
            }
    };
}

namespace bfm::bfo::translators
{
    template <typename PositionType = std::int64_t,
              typename ValueType = std::int64_t,
              typename NameSet = general_to_cpp::StandardCppNameSet>
    using ToCpp = typename general_to_cpp::ToCpp<
        bfm::parser::DataChange<std::map<PositionType, ValueType> >,
        NameSet>;
}

#endif // TOCPP_HPP_INCLUDED
