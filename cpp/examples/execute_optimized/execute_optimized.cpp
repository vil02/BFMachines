// This example illustrated the performance gain when
// bfm::BFMachine::execute_optimized method is used.
// The values of factorials for consecutive integers is computed.
// Compare this file with the example
// ../memory_types_performance_comparison/memory_types_performance_comparison/cpp
// where the method bfm::BFMachine::execute_optimized is used.
#include <iostream>
#include <string_view>
#include <vector>
#include <array>
#include <chrono>
#include "../../BFMachineLib/BFMachineLib.hpp"

template <typename ValueType>
[[nodiscard]] constexpr ValueType factorial_bf(const ValueType& in_num)
{
    const std::string_view factorial_bf_code =
        "+>>,[>>]<<"
        "[[->+>+<<]>>[-<<+>>]<-]<<"
        "[[>[->+>+<<]>>[-<<+>>]<[->>+<<]<<-]>>>>[-<<<<+>>>>]<<<<>[-]<<]>.";
    using memory_type = bfm::memory_types::VectorMemory<std::vector<ValueType> >;
    using i_stream_type = typename bfm::streams::InputStream<std::array<ValueType, 1> >;
    using o_stream_type = typename bfm::streams::OutputArrayStream<std::array<ValueType, 1> >;
    using bfm_type = typename bfm::BFMachine<memory_type, i_stream_type, o_stream_type>;

    i_stream_type input_stream = i_stream_type({in_num});
    o_stream_type output_stream = o_stream_type();
    bfm_type(input_stream, output_stream).execute_optimized(factorial_bf_code);
    return output_stream.get_data()[0];
}

std::ostream& operator<<(std::ostream& stream, const __uint128_t& in_val)
{
    const unsigned cur_rem = in_val%10;
    const auto new_val = (in_val-cur_rem)/10;
    if (new_val != 0)
    {
        stream<<new_val;
    }
    stream<<cur_rem;
    return stream;
}

int main()
{
    using value_type = __uint128_t;
    value_type n_limit = 34; //factorial of 33 still fits into 128 bits
    // if for any reason you do not have access to __uint128_t use the lines below
    // using value_type = std::uint64_t;
    // value_type n_limit = 21; //factorial of 20 still fits into 64 bits

    const auto start_time = std::chrono::steady_clock::now();
    for (value_type cur_n = 0; cur_n < n_limit; ++cur_n)
    {
        std::cout<<cur_n<<'\t'<<factorial_bf(cur_n)<<std::endl;
    }
    const auto end_time = std::chrono::steady_clock::now();
    const std::chrono::duration<double> run_time = end_time-start_time;
    std::cout<<"Runtime: "<<run_time.count()<<" [s]"<<std::endl;
    return 0;
}
