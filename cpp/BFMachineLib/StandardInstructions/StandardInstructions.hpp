#ifndef STANDARDINSTRUCTIONS_HPP_INCLUDED
#define STANDARDINSTRUCTIONS_HPP_INCLUDED

namespace bfm
{
    struct [[nodiscard]] StandardInstructions
    {
        using instruction_type = char;
        static const constexpr instruction_type move_left = '<';
        static const constexpr instruction_type move_right = '>';
        static const constexpr instruction_type increase_value = '+';
        static const constexpr instruction_type decrease_value = '-';
        static const constexpr instruction_type read_value = ',';
        static const constexpr instruction_type print_value = '.';
        static const constexpr instruction_type begin_loop = '[';
        static const constexpr instruction_type end_loop = ']';
    };
}

#endif // STANDARDINSTRUCTIONS_HPP_INCLUDED
