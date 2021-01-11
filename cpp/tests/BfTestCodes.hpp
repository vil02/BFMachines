#ifndef BFTESTCODES_HPP_INCLUDED
#define BFTESTCODES_HPP_INCLUDED

namespace bf_test_codes
{
    template <typename CodeType>
    [[nodiscard]] constexpr CodeType bf_print_countdown()
    {
        return CodeType(",[.-]");
    }

    template <typename CodeType>
    [[nodiscard]] constexpr CodeType bf_plus()
    {
        return CodeType(",>,<[->+<]>.");
    }

    template <typename CodeType>
    [[nodiscard]] constexpr CodeType bf_times()
    {
        return CodeType(",>,<[>[->+>+<<]>>[-<<+>>]<[->>+<<]<<-]>>>>.");
    }
    template <typename CodeType>
    [[nodiscard]] constexpr CodeType bf_fibonacci()
    {
        return CodeType(",>>+<<[->>[->+>+<<]>>[-<<+>>]<<<[->+<]>>[-<<+>>]<<< ]>.");
    }
    template <typename CodeType>
    [[nodiscard]] constexpr CodeType bf_factorial()
    {
        return CodeType("+>>,[>>]<<"
                        "[[->+>+<<]>>[-<<+>>]<-]<<"
                        "[[>[->+>+<<]>>[-<<+>>]<[->>+<<]<<-]>>>>[-<<<<+>>>>]<<<<>[-]<<]>.");
    }
}

#endif // BFTESTCODES_HPP_INCLUDED
