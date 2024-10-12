// this file illustrates how to simplify bf code by using the functions
// bfm::parser::bf_code_to_operation_seq
// bfm::bfo::translators::ToBf<>::print
// the resulting code does not contain "meaningless" operations like "<>" or "+-"

#include "../../BFMachineLib/BFMachineLib.hpp"

#include <iostream>
#include <string>

template <typename CodeType,
          typename PositionType = std::int64_t,
          typename ValueType = std::int64_t>
[[nodiscard]] auto simplify_bf_code(const CodeType& in_bf_code)
{
    const auto bf_seq = bfm::parser::bf_code_to_operation_seq<
        CodeType, PositionType, ValueType>(in_bf_code);
    return bfm::bfo::translators::ToBf<>::print(bf_seq);
}
int main()
{
    const std::string example_code = ",>-++-ABC<>,<><[->-++<><]>.";
    std::cout<<"Input code:\n"
        <<example_code<<'\n'
        <<"Simplified code:\n"
        <<simplify_bf_code(example_code)<<'\n';
    return 0;
}
