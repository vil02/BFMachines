#ifndef BFMACHINE_HPP_INCLUDED
#define BFMACHINE_HPP_INCLUDED

#include "BFMState.hpp"
#include "../FindMatching/FindMatching.hpp"
#include "../BFParser/BFCodeToOperationSeq.hpp"
#include "../BFOperations/BFOperations.hpp"
#include "../StandardInstructions/StandardInstructions.hpp"

#include <map>
#include <string>
#include <type_traits>

namespace bfm
{
    template<typename MemoryType,
             typename InputStream,
             typename OutputStream,
             typename InstructionSet = StandardInstructions>
    class [[nodiscard]] BFMachine
    {
        public:
            using memory_type = MemoryType;
            using value_type = typename memory_type::value_type;
            using position_type = typename memory_type::position_type;
            using input_stream_type = InputStream;
            using output_stream_type = OutputStream;
        private:
            BFMState<memory_type, input_stream_type, output_stream_type> state;
            template <typename CodeType>
            [[nodiscard]] constexpr typename CodeType::size_type execute_single_instruction(
                    const CodeType& in_code,
                    const typename CodeType::size_type in_code_position)
            {
                static_assert(
                    std::is_same_v<typename CodeType::value_type,
                                   typename InstructionSet::instruction_type>);
                typename CodeType::size_type char_num = in_code_position;
                switch (in_code[char_num])
                {
                    case InstructionSet::move_right:
                        this->state.move_right();
                        ++char_num;
                        break;
                    case InstructionSet::move_left:
                        this->state.move_left();
                        ++char_num;
                        break;
                    case InstructionSet::decrease_value:
                        this->state.decrease_value();
                        ++char_num;
                        break;
                    case InstructionSet::increase_value:
                        this->state.increase_value();
                        ++char_num;
                        break;
                    case InstructionSet::print_value:
                        this->state.print_value();
                        ++char_num;
                        break;
                    case InstructionSet::read_value:
                        {
                            this->state.read_value();
                            ++char_num;
                            break;
                        }
                    case InstructionSet::begin_loop:
                        if (this->state.is_current_value_not_zero())
                        {
                            ++char_num;
                        }
                        else
                        {
                            char_num = find_matching<CodeType, InstructionSet>(in_code, char_num);
                            ++char_num;
                        }
                        break;
                    case InstructionSet::end_loop:
                        if (this->state.is_current_value_not_zero())
                        {
                            char_num = find_matching<CodeType, InstructionSet>(in_code, char_num);
                        }
                        else
                        {
                            ++char_num;
                        }
                        break;
                    default:
                        ++char_num;
                        break;
                }
                return char_num;
            }
        public:
            constexpr BFMachine(
                    input_stream_type& in_input_stream,
                    output_stream_type& in_output_stream) :
                state(in_input_stream, in_output_stream)
            {}

            [[nodiscard]] constexpr memory_type get_memory() const
            {
                return this->state.get_memory();
            }

            [[nodiscard]] constexpr position_type get_memory_position() const
            {
                return this->state.get_memory_position();
            }

            template <typename CodeType>
            constexpr void execute(const CodeType& in_code)
            {
                typename CodeType::size_type char_num = 0;
                while (char_num < in_code.size())
                {
                    char_num = this->execute_single_instruction(in_code, char_num);
                }
            }

            template <typename ShowDebugData, typename CodeType>
            constexpr void execute(
                    const CodeType& in_code,
                    ShowDebugData& show_debug_data)
            {
                typename CodeType::size_type char_num = 0;
                while (char_num < in_code.size())
                {
                    show_debug_data(in_code, char_num, *this);
                    char_num = this->execute_single_instruction(in_code, char_num);
                }
            }

            template <typename BFOperationSeq>
            constexpr void execute_seq(const BFOperationSeq& in_bf_operation_seq)
            {
                bfm::bfo::execute_seq(in_bf_operation_seq, this->state);
            }

            template <typename CodeType>
            constexpr void execute_optimized(const CodeType& in_code)
            {
                const auto bf_code_to_operation_seq = bfm::parser::bf_code_to_operation_seq<
                    CodeType,
                    typename std::make_signed_t<position_type>,
                    typename std::make_signed_t<value_type>,
                    InstructionSet>;
                this->execute_seq(bf_code_to_operation_seq(in_code));
            }
    };
}

#endif // BFMACHINE_HPP_INCLUDED
