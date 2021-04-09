#include <iostream>
#include <string>
#include <fstream>

#include "../../BFMachineLib/BFMachineLib.hpp"
#include "../../tests/BfTestCodes.hpp"

template <typename CodeType>
void proc_single(const CodeType& in_bf_code, const std::string& in_core_name)
{
    const auto bf_seq = bfm::parser::bf_code_to_operation_seq<
        CodeType,
        std::int64_t,
        std::int64_t>(in_bf_code);
    std::ofstream file_stream(in_core_name+"_bf.hpp");
    file_stream<<bfm::bfo::translators::ToCpp<>::print_as_function_in_header(
        bf_seq, in_core_name+"_bf");
    file_stream.close();
}

int main()
{
    using code_type = std::string;
    proc_single(bf_test_codes::bf_fibonacci<code_type>(), "tmp_fibonacci");
    proc_single(bf_test_codes::bf_factorial<code_type>(), "tmp_factorial");
    proc_single(bf_test_codes::bf_hello_world<code_type>(), "tmp_hello_world");
    return 0;
}
