#ifndef TESTMEMORYTYPES_HPP_INCLUDED
#define TESTMEMORYTYPES_HPP_INCLUDED

#include <boost/test/included/unit_test.hpp>
#include <boost/mpl/list.hpp>

#include <vector>
#include <map>
#include <unordered_set>
#include <random>

#include "../BFMachineLib/BFMachineLib.hpp"

typedef boost::mpl::list<bfm::memory_types::VectorMemory<std::vector<int> >,
                         bfm::memory_types::VectorMemory<std::vector<long long int> >,
                         bfm::memory_types::MapMemory<std::map<int, int> >,
                         bfm::memory_types::MapMemory<std::map<int, int>, false >,
                         bfm::memory_types::MapMemory<std::unordered_map<int, int> >,
                         bfm::memory_types::MapMemory<std::unordered_map<int, int>, false >
                        > memory_types;

BOOST_AUTO_TEST_CASE_TEMPLATE(get_value_test, MemoryType, memory_types)
{
    MemoryType cur_memory;
    for (typename MemoryType::position_type i = -100; i < 100; ++i)
    {
        BOOST_CHECK_EQUAL(cur_memory.get_value(i), 0);
    }
}

BOOST_AUTO_TEST_CASE_TEMPLATE(set_value_test, MemoryType, memory_types)
{
    MemoryType cur_memory;
    const typename MemoryType::position_type min_ind = -500, max_ind = 500;
    for (typename MemoryType::position_type i = min_ind; i < max_ind; ++i)
    {
        if (i%3 == 0)
        {
            cur_memory.set_value(i, -i);
            BOOST_CHECK_EQUAL(cur_memory.get_value(i), -i);
        }
    }
    for (typename MemoryType::position_type i = min_ind; i < max_ind; ++i)
    {
        const typename MemoryType::value_type target_val = (i%3 == 0) ? -i : 0;
        BOOST_CHECK_EQUAL(cur_memory.get_value(i), target_val);
    }
}

BOOST_AUTO_TEST_CASE_TEMPLATE(incr_decr_value_test, MemoryType, memory_types)
{
    using value_type = typename MemoryType::value_type;
    using position_type = typename MemoryType::position_type;
    MemoryType cur_memory;
    std::mt19937 random_engine(0);
    std::uniform_int_distribution<position_type> position_dist{-100, 100};
    std::uniform_int_distribution<std::size_t> num_dist{1, 100};
    std::uniform_int_distribution<value_type> value_dist{-300, 300};

    for (std::size_t test_number = 0; test_number < 100; ++test_number)
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
                          start_value+plus_num);

        for (std::size_t i = 0; i < minus_num; ++i)
        {
            decrease_value(cur_memory, cur_position);
        }
        BOOST_CHECK_EQUAL(cur_memory.get_value(cur_position),
                          start_value+plus_num-value_type(minus_num));
    }
}

#endif // TESTMEMORYTYPES_HPP_INCLUDED
