// this file illustrated the usage of the function
// bfm::bfo::translators::ToCpp<>::print
// note the functions:
// bfm::bfo::translators::ToCpp<>::ToCpp::print_as_function
// bfm::bfo::translators::ToCpp<>::print_as_function_in_header

#include <iostream>
#include <string>
#include <string_view>

#include "../../BFMachineLib/BFMachineLib.hpp"

template <typename CodeType>
[[nodiscard]] auto bf_to_cpp(const CodeType& in_bf_code)
{
    const auto bf_seq = bfm::parser::bf_code_to_operation_seq<
                        CodeType,
                        std::int64_t,
                        std::int64_t>(in_bf_code);
    return bfm::bfo::translators::ToCpp<>::print(bf_seq);
}

int main()
{
    int return_value = 0;
    const std::string_view bf_code = ",>,<[->++<]>.";
    try
    {
        std::cout<<"bf_code:\n"<<bf_code<<"\n\ncpp_code:\n"<<bf_to_cpp(bf_code);
    }
    catch (const std::exception& ex)
    {
        std::cout<<"Error: "<<ex.what()<<'\n';
        return_value = 1;
    }
    catch (...)
    {
        std::cout<<"Unknown error\n";
        return_value = 2;
    }
    return return_value;
}
