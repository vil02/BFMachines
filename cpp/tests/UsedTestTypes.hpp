#ifndef USEDTESTTYPES_HPP_INCLUDED
#define USEDTESTTYPES_HPP_INCLUDED

#include <boost/mp11.hpp>
#include <vector>
#include <deque>
#include <map>
#include <unordered_map>

namespace utt::inner
{
    using position_types = boost::mp11::mp_list<int, long long int>;
    using value_types = boost::mp11::mp_list<int, long long int, unsigned>;
    using char_value_types = boost::mp11::mp_list<char, unsigned char>;
    using bool_values = boost::mp11::mp_list<std::true_type, std::false_type>;

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

    template <typename ValueTypes>
    using memory_types = boost::mp11::mp_append<
        boost::mp11::mp_transform<inner::vector_memory, inner::vector_containers<ValueTypes> >,
        boost::mp11::mp_product<inner::map_memory, inner::map_containers<ValueTypes>, inner::bool_values> >;
  }

namespace utt
{
    using memory_types = inner::memory_types<inner::value_types>;
    using char_memory_types = inner::memory_types<inner::char_value_types>;
}
#endif //USEDTESTTYPES_HPP_INCLUDED
