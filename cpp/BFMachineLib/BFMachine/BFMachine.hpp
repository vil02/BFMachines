#ifndef BFMACHINE_HPP_INCLUDED
#define BFMACHINE_HPP_INCLUDED

#include <string>
#include <exception>
#include <type_traits>
namespace bfm
{
    namespace inner
    {
        template <typename CodeType, typename InstructionSet>
        [[nodiscard]] constexpr typename CodeType::size_type find_matching(
                const CodeType& in_code,
                const typename CodeType::size_type start_pos)
        {
            static_assert(
                std::is_same<typename CodeType::value_type,
                             typename InstructionSet::instruction_type>::value);
            if (in_code[start_pos] != InstructionSet::begin_loop &&
                in_code[start_pos] != InstructionSet::end_loop)
            {
                throw std::invalid_argument("in_code[start_pos] must begin or end loop symbol.");
            }
            const char target_char =
                in_code[start_pos] == InstructionSet::begin_loop ?
                InstructionSet::end_loop : InstructionSet::begin_loop;
            const typename CodeType::difference_type search_dir =
                in_code[start_pos] == InstructionSet::begin_loop ? 1 : -1;

            typename CodeType::difference_type cur_count = 0;
            typename CodeType::size_type cur_pos = start_pos+search_dir;
            while (cur_pos < in_code.size())
            {
                if (in_code[cur_pos] == target_char && cur_count == 0)
                {
                    break;
                }
                else if (in_code[cur_pos] == InstructionSet::begin_loop)
                {
                    ++cur_count;
                }
                else if (in_code[cur_pos] == InstructionSet::end_loop)
                {
                    --cur_count;
                }
                cur_pos += search_dir;
            }
            if (cur_pos >= in_code.size())
            {
                throw std::invalid_argument("Could not find matching loop symbol.");
            }
            return cur_pos;
        }
    }
    struct StandardInstructions
    {
        using instruction_type = char;
        static const instruction_type move_left = '<';
        static const instruction_type move_right = '>';
        static const instruction_type increase_value = '+';
        static const instruction_type decrease_value = '-';
        static const instruction_type read_value = ',';
        static const instruction_type print_value = '.';
        static const instruction_type begin_loop = '[';
        static const instruction_type end_loop = ']';
    };
    template<typename MemoryType,
             typename InputStream,
             typename OutputStream,
             typename InstructionSet = StandardInstructions>
    class BFMachine
    {
        public:
            using value_type = typename MemoryType::value_type;
            using position_type = typename MemoryType::position_type;
        private:
            MemoryType memory;
            position_type cur_position;
            InputStream& input_stream;
            OutputStream& output_stream;
            template <typename CodeType>
            [[nodiscard]] constexpr typename CodeType::size_type execute_single_instruction(
                    const CodeType& in_code,
                    const typename CodeType::size_type in_code_position)
            {
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
                        value_type new_val;
                        this->input_stream>>new_val;
                        this->memory.set_value(this->cur_position, new_val);
                        ++char_num;
                        break;
                    case InstructionSet::begin_loop:
                        if (this->memory.get_value(this->cur_position) != value_type(0))
                        {
                            ++char_num;
                        }
                        else
                        {
                            char_num =
                                inner::find_matching<CodeType, InstructionSet>(in_code, char_num);
                            ++char_num;
                        }
                        break;
                    case InstructionSet::end_loop:
                        if (this->memory.get_value(this->cur_position) != value_type(0))
                        {
                            char_num =
                                inner::find_matching<CodeType, InstructionSet>(in_code, char_num);
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
            constexpr BFMachine(InputStream& in_input_stream, OutputStream& in_output_stream) :
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
    };
}

#endif // BFMACHINE_HPP_INCLUDED
