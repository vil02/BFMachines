//example showing how to create a bf interpreter
//and use it to run a code displaying Hello World! in the standard output

#include "../../BFMachineLib/BFMachineLib.hpp"

#include <iostream>
#include <string_view>
#include <unordered_map>

//#include <map>
//#include <vector>
int main()
{
    //we want to use an unbounded memory, where each cell is a char
    //(int is used to store the addresses/positions in the memory)
    using memory_type = typename bfm::memory_types::MapMemory<std::unordered_map<int, char> >;

    //we could also use (remember about the proper includes for std::map or std::vector):
    //using memory_type = typename bfm::memory_types::MapMemory<std::map<int, char> >;
    //using memory_type = typename bfm::memory_types::VectorMemory<std::vector<char> >;

    //we want our bf_machine to use the standard input/output stream
    using i_stream_type = decltype(std::cin);
    using o_stream_type = decltype(std::cout);

    //now we can define the exact type of out bf_machine
    using bfm_type = typename bfm::BFMachine<memory_type, i_stream_type, o_stream_type>;
    //this is the bf code printing "Hello World!"
    const std::string_view bf_code =
        "++++++++[>++++[>++>+++>+++>+<<<<-]>+>+>->>+[<]<-]>>.>"
        "---.+++++++..+++.>>.<-.<.+++.------.--------.>>+.>++.";
    //we create a temporary bf machine and execute given bf code
    bfm_type(std::cin, std::cout).execute(bf_code);
    //we can also use the execute_optimized for better performance,
    //cf. example ../execute_optimized/execute_optimized.hpp
    //bfm_type(std::cin, std::cout).execute_optimized(bf_code);
    return 0;
}
