#ifndef TESTMEMORYTYPES_HPP_INCLUDED
#define TESTMEMORYTYPES_HPP_INCLUDED

#include "../BFMachineLib/BFMachineLib.hpp"

#include <boost/test/included/unit_test.hpp>
#include <boost/mpl/list.hpp>
#include <boost/mp11.hpp>

#include <vector>
#include <array>
#include <map>
#include <unordered_map>
#include <random>
#include <type_traits>

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

using memory_types = boost::mp11::mp_append<
    boost::mp11::mp_transform<vector_memory, vector_containers>,
    boost::mp11::mp_product<map_memory, map_containers, bool_values> >;


// typedef boost::mpl::list<bfm::memory_types::VectorMemory<std::vector<int> >,
//                          bfm::memory_types::VectorMemory<std::vector<long long int> >,
//                          bfm::memory_types::VectorMemory<std::vector<unsigned> >,
//                          bfm::memory_types::MapMemory<std::map<int, int> >,
//                          bfm::memory_types::MapMemory<std::map<int, int>, false >,
//                          bfm::memory_types::MapMemory<std::unordered_map<int, int> >,
//                          bfm::memory_types::MapMemory<std::unordered_map<int, unsigned> >,
//                          bfm::memory_types::MapMemory<std::unordered_map<int, int>, false >
//                         > memory_types;
//

BOOST_AUTO_TEST_CASE_TEMPLATE(get_value_test, MemoryType, memory_types)
{
    MemoryType cur_memory;
    using position_type = typename MemoryType::position_type;
    const position_type test_size = 100;
    for (position_type i = -test_size; i < test_size; ++i)
    {
        BOOST_CHECK_EQUAL(cur_memory.get_value(i), 0);
    }
}

BOOST_AUTO_TEST_CASE_TEMPLATE(set_value_test, MemoryType, memory_types)
{
    MemoryType cur_memory;
    using position_type = typename MemoryType::position_type;
    using value_type = typename MemoryType::value_type;
    const position_type min_ind = -500;
    const position_type max_ind = -min_ind;
    for (typename MemoryType::position_type i = min_ind; i < max_ind; ++i)
    {
        if (i%3 == 0)
        {
            cur_memory.set_value(i, value_type(-i));
            BOOST_CHECK_EQUAL(cur_memory.get_value(i), value_type(-i));
        }
    }
    for (typename MemoryType::position_type i = min_ind; i < max_ind; ++i)
    {
        const typename MemoryType::value_type target_val = (i%3 == 0) ? value_type(-i) : 0;
        BOOST_CHECK_EQUAL(cur_memory.get_value(i), target_val);
    }
}

BOOST_AUTO_TEST_CASE_TEMPLATE(incr_decr_value_test, MemoryType, memory_types)
{
    using value_type = typename MemoryType::value_type;
    using position_type = typename MemoryType::position_type;
    MemoryType cur_memory;
    std::mt19937 random_engine(0);
    const position_type position_range = 100;
    std::uniform_int_distribution<position_type> position_dist{-position_range, position_range};
    const std::size_t min_op_num = 1;
    const std::size_t max_op_num = 100;
    std::uniform_int_distribution<std::size_t> num_dist{min_op_num, max_op_num};
    const value_type value_range = 300;
    std::uniform_int_distribution<value_type> value_dist{-value_range, value_range};

    const std::size_t test_size = 100;
    for (std::size_t test_number = 0; test_number < test_size; ++test_number)
    {
        const auto cur_position = position_dist(random_engine);
        const auto start_value = value_dist(random_engine);
        const auto plus_num = num_dist(random_engine);
        const auto minus_num = num_dist(random_engine);

        cur_memory.set_value(cur_position, start_value);
        for (std::size_t i = 0; i < plus_num; ++i)
        {
            increase_value(cur_memory, cur_position);
        }
        BOOST_CHECK_EQUAL(cur_memory.get_value(cur_position),
                          start_value+value_type(plus_num));

        for (std::size_t i = 0; i < minus_num; ++i)
        {
            decrease_value(cur_memory, cur_position);
        }
        BOOST_CHECK_EQUAL(cur_memory.get_value(cur_position),
                          start_value+value_type(plus_num)+value_type(-minus_num));
    }
}

BOOST_AUTO_TEST_CASE(array_memory_test)
{
    using value_type = int;
    constexpr std::size_t memory_size = 5;
    constexpr std::size_t starting_position = 2;
    constexpr value_type default_value = -1;
    using memory_type =
        typename bfm::memory_types::ArrayMemory<
            std::array<value_type, memory_size>,
            starting_position,
            default_value>;
    using position_type = typename memory_type::position_type;
    memory_type cur_memory;
    BOOST_CHECK_EQUAL(cur_memory.get_starting_position(), starting_position);
    for (position_type cur_pos = 0; cur_pos < memory_size; ++cur_pos)
    {
        BOOST_CHECK_EQUAL(cur_memory.get_value(cur_pos), default_value);
    }
    const position_type special_pos = 4;
    cur_memory.set_value(4, default_value+10);
    for (position_type cur_pos = 0; cur_pos < memory_size; ++cur_pos)
    {
        if (cur_pos != special_pos)
        {
            BOOST_CHECK_EQUAL(cur_memory.get_value(cur_pos), default_value);
        }
    }
    BOOST_CHECK_EQUAL(cur_memory.get_value(special_pos), default_value+10);
    value_type tmp_variable = default_value;
    BOOST_CHECK_THROW(cur_memory.set_value(memory_size, tmp_variable), std::out_of_range);
    BOOST_CHECK_THROW(tmp_variable = cur_memory.get_value(memory_size), std::out_of_range);
    BOOST_CHECK_THROW(cur_memory.set_value(memory_size, tmp_variable+1), std::out_of_range);
    BOOST_CHECK_THROW(tmp_variable = cur_memory.get_value(memory_size+1), std::out_of_range);
}

#endif // TESTMEMORYTYPES_HPP_INCLUDED
