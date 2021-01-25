#ifndef BFMACHINE_HPP_INCLUDED
#define BFMACHINE_HPP_INCLUDED

#include "../BFMDataRef/BFMDataRef.hpp"

#include <map>
#include <string>
#include <type_traits>

namespace bfm
{
    template<typename MemoryType,
             typename InputStream,
             typename OutputStream,
             typename InstructionSet = StandardInstructions>
    class BFMachine
    {
        public:
            using value_type = typename MemoryType::value_type;
            using position_type = typename MemoryType::position_type;
            using input_stream_type = InputStream;
            using output_stream_type = OutputStream;
        private:
            MemoryType memory;
            position_type cur_position;
            input_stream_type& input_stream;
            output_stream_type& output_stream;
            template <typename CodeType>
            [[nodiscard]] constexpr typename CodeType::size_type execute_single_instruction(
                    const CodeType& in_code,
                    const typename CodeType::size_type in_code_position)
            {
                static_assert(
                    std::is_same<typename CodeType::value_type,
                                 typename InstructionSet::instruction_type>::value);
                typename CodeType::size_type char_num = in_code_position;
                switch (in_code[char_num])
                {
                    case InstructionSet::move_right:
                        ++this->cur_position;
                        ++char_num;
                        break;
                    case InstructionSet::move_left:
                        --this->cur_position;
                        ++char_num;
                        break;
                    case InstructionSet::decrease_value:
                        decrease_value(this->memory, this->cur_position);
                        ++char_num;
                        break;
                    case InstructionSet::increase_value:
                        increase_value(this->memory, this->cur_position);
                        ++char_num;
                        break;
                    case InstructionSet::print_value:
                        this->output_stream<<this->memory.get_value(this->cur_position);
                        ++char_num;
                        break;
                    case InstructionSet::read_value:
                        {
                            value_type new_val = value_type();
                            this->input_stream>>new_val;
                            this->memory.set_value(this->cur_position, new_val);
                            ++char_num;
                            break;
                        }
                    case InstructionSet::begin_loop:
                        if (this->memory.get_value(this->cur_position) != value_type(0))
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
                        if (this->memory.get_value(this->cur_position) != value_type(0))
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
                memory(),
                cur_position(memory.get_starting_position()),
                input_stream(in_input_stream),
                output_stream(in_output_stream)
            {}
            [[nodiscard]] constexpr MemoryType get_memory() const
            {
                return this->memory;
            }
            [[nodiscard]] constexpr position_type get_memory_position() const
            {
                return this->cur_position;
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
                auto cur_bf_state = bfm::BFMDataRef(
                    this->cur_position,
                    this->memory,
                    this->input_stream,
                    this->output_stream);
                bfm::bfo::execute_seq(in_bf_operation_seq, cur_bf_state);
            }
            template <typename CodeType>
            constexpr void execute_optimized(const CodeType& in_code)
            {
                const auto bf_code_to_operation_seq = bfm::parser::bf_code_to_operation_seq<
                    CodeType,
                    position_type,
                    value_type,
                    InstructionSet>;
                this->execute_seq(bf_code_to_operation_seq(in_code));
            }
    };
}

#endif // BFMACHINE_HPP_INCLUDED
