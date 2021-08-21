#ifndef USEDTESTTYPES_HPP_INCLUDED
#define USEDTESTTYPES_HPP_INCLUDED

#include "../BFMachineLib/BFMachineLib.hpp"

#include <boost/mp11.hpp>
#include <vector>
#include <deque>
#include <map>
#include <unordered_map>

namespace utt::inner
{
    struct FlippedInstructions
    {
        using instruction_type = typename bfm::StandardInstructions::instruction_type;
        static const constexpr instruction_type move_left = bfm::StandardInstructions::move_right;
        static const constexpr instruction_type move_right = bfm::StandardInstructions::move_left;
        static const constexpr instruction_type increase_value = bfm::StandardInstructions::increase_value;
        static const constexpr instruction_type decrease_value = bfm::StandardInstructions::decrease_value;
        static const constexpr instruction_type read_value = bfm::StandardInstructions::read_value;
        static const constexpr instruction_type print_value = bfm::StandardInstructions::print_value;
        static const constexpr instruction_type begin_loop = bfm::StandardInstructions::begin_loop;
        static const constexpr instruction_type end_loop = bfm::StandardInstructions::end_loop;
    };

    using position_types = boost::mp11::mp_list<int, long long int>;
    using value_types = boost::mp11::mp_list<int, long long int, unsigned, unsigned long long int, short int, short unsigned>;
    using char_value_types = boost::mp11::mp_list<char, unsigned char>;
    using bool_values = boost::mp11::mp_list<std::true_type, std::false_type>;
    using instruction_types = boost::mp11::mp_list<bfm::StandardInstructions, FlippedInstructions>;

    template <typename ValueTypes>
    using vector_containers = boost::mp11::mp_append<
        boost::mp11::mp_transform<std::vector, ValueTypes>,
        boost::mp11::mp_transform<std::deque, ValueTypes> >;

    template <typename ValueTypes>
    using map_containers = boost::mp11::mp_append<
        boost::mp11::mp_product<std::map, position_types, ValueTypes>,
        boost::mp11::mp_product<std::unordered_map, position_types, ValueTypes> >;

    template <typename Container>
    using vector_memory = bfm::memory_types::VectorMemory<Container>;

    template <typename Container, typename RemoveDefaultValuesType>
    using map_memory = bfm::memory_types::MapMemory<Container, RemoveDefaultValuesType::value>;

    template <typename MemoryType, typename InstructionSet>
    using bfm_type = bfm::BFMachine<
        MemoryType,
        bfm::streams::InputStream<std::vector<typename MemoryType::value_type> >,
        bfm::streams::OutputVectorStream<std::vector<typename MemoryType::value_type> >,
        InstructionSet>;

    template <typename MemoryType, typename InstructionSet>
    using bfm_stringstream_no_input_type = bfm::BFMachine<
        MemoryType,
        bfm::streams::InputStream<std::array<typename MemoryType::value_type, 0> >,
        std::stringstream,
        InstructionSet>;

    template <typename ValueTypes>
    using memory_types = boost::mp11::mp_append<
        boost::mp11::mp_transform<inner::vector_memory, inner::vector_containers<ValueTypes> >,
        boost::mp11::mp_product<inner::map_memory, inner::map_containers<ValueTypes>, inner::bool_values> >;

    template <typename ValueTypes>
    using bfm_types = boost::mp11::mp_product<bfm_type, memory_types<ValueTypes>, instruction_types>;

    template <typename ValueTypes>
    using bfm_stringstream_no_input_types = boost::mp11::mp_product<
        bfm_stringstream_no_input_type, memory_types<ValueTypes>, instruction_types>;
  }

namespace utt
{
    using memory_types = inner::memory_types<inner::value_types>;
    using char_memory_types = inner::memory_types<inner::char_value_types>;
    using bfm_types = inner::bfm_types<inner::value_types>;
    using bfm_stringstream_no_input_type = inner::bfm_stringstream_no_input_types<inner::char_value_types>;
}
#endif //USEDTESTTYPES_HPP_INCLUDED
