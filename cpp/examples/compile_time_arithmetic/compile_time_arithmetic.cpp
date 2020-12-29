//This example shows that BFMachineLib is suitable for compile time computations.
//Here sum and product of two unsigned values are evaluated.
//Because brainfuck is Turing complete,
//one can also view this example as a proof that compile time C++ is Turing complete.
//This example requires C++20 (I tested it on GCC 10.2 with -std=c++20).
#include <array>

#include "../../BFMachineLib/BFMachineLib.hpp"

template<typename ValueType>
constexpr ValueType bf_add(const ValueType& val_a, const ValueType& val_b)
{
    using memory_type = typename bfm::memory_types::ArrayMemory<std::array<ValueType, 2> >;
    using input_stream_type = bfm::streams::InputStream<std::array<ValueType, 2> >;
    using output_stream_type = bfm::streams::OutputArrayStream<std::array<ValueType, 1> >;
    using bf_machine_type =
        typename bfm::BFMachine<memory_type, input_stream_type, output_stream_type>;
    input_stream_type i_stream({val_a, val_b});
    output_stream_type o_stream;
    bf_machine_type bf_machine(i_stream, o_stream);
    std::string_view bf_code(",>,<[->+<]>.");
    bf_machine.execute(bf_code);
    return o_stream.get_data()[0];
}

template<typename ValueType>
constexpr ValueType bf_multiply(const ValueType& val_a, const ValueType& val_b)
{
    using memory_type = typename bfm::memory_types::ArrayMemory<std::array<ValueType, 5> >;
    using input_stream_type = bfm::streams::InputStream<std::array<ValueType, 2> >;
    using output_stream_type = bfm::streams::OutputArrayStream<std::array<ValueType, 1> >;
    using bf_machine_type =
        typename bfm::BFMachine<memory_type, input_stream_type, output_stream_type>;
    input_stream_type i_stream({val_a, val_b});
    output_stream_type o_stream;
    bf_machine_type bf_machine(i_stream, o_stream);
    std::string_view bf_code(",>,<[>[->+>+<<]>>[-<<+>>]<[->>+<<]<<-]>>>>.");
    bf_machine.execute(bf_code);
    return o_stream.get_data()[0];
}

int main()
{
    const unsigned valA = 19;
    const unsigned valB = 11;
    static_assert(bf_add(valA, valB) == valA+valB);
    static_assert(bf_multiply(valA, valB) == valA*valB);
    return 0;
}
