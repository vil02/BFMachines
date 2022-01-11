//This example shows that BFMachineLib is suitable for compile time computations.
//Here sum and product of two unsigned values are evaluated.
//Because brainfuck is Turing complete,
//one can also view this example as a proof that compile time C++ is Turing complete.
//This example requires C++20 (I tested it on GCC 10.2 with -std=c++20).
#include "../../BFMachineLib/BFMachineLib.hpp"

#include <array>

template<std::size_t memory_size, typename CodeType, typename InputDataType>
constexpr typename InputDataType::value_type
compute(const CodeType& bf_code, InputDataType input_data)
{
    using value_type = typename InputDataType::value_type;
    using memory_type =
        typename bfm::memory_types::ArrayMemory<std::array<value_type, memory_size> >;
    using input_stream_type = bfm::streams::InputStream<InputDataType>;
    using output_stream_type = bfm::streams::OutputArrayStream<std::array<value_type, 1> >;
    using bf_machine_type =
        typename bfm::BFMachine<memory_type, input_stream_type, output_stream_type>;
    input_stream_type i_stream(input_data);
    output_stream_type o_stream;
    bf_machine_type bf_machine(i_stream, o_stream);
    bf_machine.execute(bf_code);
    return o_stream.get_data()[0];
}

template<typename ValueType>
constexpr ValueType bf_add(const ValueType& val_a, const ValueType& val_b)
{
    return compute<2>(
        std::string_view(",>,<[->+<]>."),
        std::array<ValueType, 2>({val_a, val_b}));
}

template<typename ValueType>
constexpr ValueType bf_multiply(const ValueType& val_a, const ValueType& val_b)
{
    return compute<5>(
        std::string_view(",>,<[>[->+>+<<]>>[-<<+>>]<[->>+<<]<<-]>>>>."),
        std::array<ValueType, 2>({val_a, val_b}));
}

int main()
{
    const unsigned valA = 19;
    const unsigned valB = 11;
    static_assert(bf_add(valA, valB) == valA+valB);
    static_assert(bf_multiply(valA, valB) == valA*valB);
    return 0;
}
