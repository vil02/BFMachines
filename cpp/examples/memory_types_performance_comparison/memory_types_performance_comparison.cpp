//this file compares the performance of different unbounded bfm::memory_types
//the bf code used for the benchmark, factorial computation, is "dense" in the memory usage

#include "../../BFMachineLib/BFMachineLib.hpp"

#include <chrono>
#include <iostream>
#include <map>
#include <string_view>
#include <unordered_map>
#include <vector>


template <typename CodeType>
[[nodiscard]] constexpr CodeType bf_factorial()
{
    //the resulting bf code computes factorial for given input n
    //at first the memory if populated with the values n, n-1, ... 1
    //the factorial is computed by multiplying the two numbers at the end
    //(until there are at least two values),
    //memory during the computation evolves as follows:
    //n, n-1, n-2, ... 4, 3, 2, 1
    //n, n-1, n-2, ... 4, 3, 2
    //n, n-1, n-2, ... 4, 6
    //n, n-1, n-2, ... 24
    //...
    //n, (n-1)*(n-2)*...*2*1
    //n!
    //the case for the input 0 is treated differently: if the input is 0, it is changed to 1
    return CodeType("+>>,[>>]<<"
                    "[[->+>+<<]>>[-<<+>>]<-]<<"
                    "[[>[->+>+<<]>>[-<<+>>]<[->>+<<]<<-]>>>>[-<<<<+>>>>]<<<<>[-]<<]>.");
}

template <typename ValueType>
[[nodiscard]] constexpr ValueType factorial(const ValueType& in_num)
{
    ValueType res = 1;
    for (ValueType cur_num = 1; cur_num <= in_num; ++cur_num)
    {
        res *= cur_num;
    }
    return res;
}

template <typename MemoryType>
[[nodiscard]] double time_bf_factorial(const typename MemoryType::value_type &in_num)
{
    using value_type = typename MemoryType::value_type;
    using i_stream_type = bfm::streams::InputStream<std::array<value_type, 1> >;
    using o_stream_type = bfm::streams::OutputArrayStream<std::array<value_type, 1> >;
    using bfm_type = bfm::BFMachine<MemoryType, i_stream_type, o_stream_type>;

    auto input_stream = i_stream_type({in_num});
    auto output_stream = o_stream_type();
    const auto start_time = std::chrono::steady_clock::now();
    bfm_type(input_stream, output_stream).execute(bf_factorial<std::string_view>());
    const auto end_time = std::chrono::steady_clock::now();
    const std::chrono::duration<double> run_time = end_time-start_time;
    if (output_stream.get_data().size() != 1)
    {
        throw std::logic_error("Wrong output size!");
    }
    if (output_stream.get_data()[0] != factorial(in_num))
    {
        throw std::logic_error("Wrong result value!");
    }
    return run_time.count();
}

int main()
{
    using value_type = unsigned;
    using memory_type_a = bfm::memory_types::MapMemory<std::map<int, value_type> >;
    using memory_type_b = bfm::memory_types::MapMemory<std::unordered_map<int, value_type> >;
    using memory_type_c = bfm::memory_types::VectorMemory<std::vector<value_type> >;
    const auto separator = '\t';
    std::cout<<"cur_n"
        <<separator<<"map [s]"
        <<separator<<"unordered_map [s]"
        <<separator<<"vector [s]"
        <<std::endl;
    const value_type test_size = 12;
    for (value_type cur_n = 0; cur_n < test_size; ++cur_n)
    {
        std::cout<<cur_n
            <<separator<<time_bf_factorial<memory_type_a>(cur_n)
            <<separator<<time_bf_factorial<memory_type_b>(cur_n)
            <<separator<<time_bf_factorial<memory_type_c>(cur_n)
            <<std::endl;
    }
    return 0;
}
