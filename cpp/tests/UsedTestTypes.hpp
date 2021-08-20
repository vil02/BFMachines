#ifndef USEDTESTTYPES_HPP_INCLUDED
#define USEDTESTTYPES_HPP_INCLUDED

#include <boost/mp11.hpp>
#include <vector>
#include <map>
#include <unordered_map>

namespace utt::inner
{
    using position_types = boost::mp11::mp_list<int, long long int>;
    using value_types = boost::mp11::mp_list<int, long long int, unsigned>;
    using bool_values = boost::mp11::mp_list<std::true_type, std::false_type>;

    using vector_containers = boost::mp11::mp_transform<std::vector, value_types>;

    template <typename Container>
    using vector_memory = bfm::memory_types::VectorMemory<Container>;

    template <typename Container, typename remove_default_values_type>
    using map_memory = bfm::memory_types::MapMemory<Container, remove_default_values_type::value>;

    using map_containers = boost::mp11::mp_append<
        boost::mp11::mp_product<std::map, position_types, value_types>,
        boost::mp11::mp_product<std::unordered_map, position_types, value_types> >;
  }

namespace utt
{
    using memory_types = boost::mp11::mp_append<
        boost::mp11::mp_transform<inner::vector_memory, inner::vector_containers>,
        boost::mp11::mp_product<inner::map_memory, inner::map_containers, inner::bool_values> >;
}
#endif //USEDTESTTYPES_HPP_INCLUDED
