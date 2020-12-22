#include <iostream>
#include <unordered_map>
#include <string_view>
#include "../../BFMachineLib/BFMachineLib.hpp"

int main()
{
    using memory_type = typename bfm::memory_types::MapMemory<std::unordered_map<int, char> >;
    using i_stream_type = decltype(std::cin);
    using o_stream_type = decltype(std::cout);
    using bfm_type = typename bfm::BFMachine<memory_type, i_stream_type, o_stream_type>;
    const std::string_view bf_code =
        "++++++++[>++++[>++>+++>+++>+<<<<-]>+>+>->>+[<]<-]>>.>"
        "---.+++++++..+++.>>.<-.<.+++.------.--------.>>+.>++.";
    bfm_type(std::cin, std::cout).execute(bf_code);
    return 0;
}
