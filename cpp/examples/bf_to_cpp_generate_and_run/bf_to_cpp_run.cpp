#include <iostream>
#include <vector>
#include <array>
#include <sstream>

#include "../../BFMachineLib/BFMachineLib.hpp"

#include "tmp_hello_world_bf.hpp"
#include "tmp_fibonacci_bf.hpp"

template <typename ValueType>
using vector_memory = bfm::memory_types::VectorMemory<std::vector<ValueType> >;

template <typename ValueType>
[[nodiscard]] constexpr ValueType fibonacci(const ValueType& in_num)
{
    ValueType val_a = 0;
    ValueType val_b = 1;
    for (ValueType cur_num = 0; cur_num < in_num; ++cur_num)
    {
        const auto tmp_val = val_a+val_b;
        val_a = val_b;
        val_b = tmp_val;
    }
    return val_a;
}

int main()
{
    int result_value = 0;
    std::stringstream ss;
    tmp_hello_world_bf<vector_memory<char>, decltype(std::cin), std::stringstream>(std::cin, ss);
    if (ss.str() != "Hello World!\n")
    {
        result_value = 2;
    }

    using value_type = std::uint64_t;
    const value_type value_limit = 94;
    for (value_type cur_n = 0; cur_n < value_limit; ++cur_n)
    {
        using i_stream_type = bfm::streams::InputStream<std::array<value_type, 1> >;
        using o_stream_type = bfm::streams::OutputArrayStream<std::array<value_type, 1> >;
        auto i_stream = i_stream_type({cur_n});
        auto o_stream = o_stream_type();
        tmp_fibonacci_bf<vector_memory<value_type>, i_stream_type, o_stream_type>(
            i_stream, o_stream);
        const auto res_data = o_stream.get_data();
        if (res_data[0] != fibonacci(cur_n))
        {
            result_value = 2;
        }
    }

    return result_value;
}
