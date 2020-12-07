#ifndef TESTMEMORYTYPES_HPP_INCLUDED
#define TESTMEMORYTYPES_HPP_INCLUDED

#include <boost/test/included/unit_test.hpp>
#include <boost/mpl/list.hpp>

#include <vector>
#include <map>
#include <unordered_set>
#include <random>

#include "../../BFMachineLib/BFMachineLib.hpp"

typedef boost::mpl::list<BFM::MemoryTypes::VectorMemory<std::vector<int> >,
                         BFM::MemoryTypes::VectorMemory<std::vector<long long int> >,
                         BFM::MemoryTypes::MapMemory<std::map<int, int> >,
                         BFM::MemoryTypes::MapMemory<std::map<int, int>, false >,
                         BFM::MemoryTypes::MapMemory<std::unordered_map<int, int> >,
                         BFM::MemoryTypes::MapMemory<std::unordered_map<int, int>, false >
                        > MemoryTypes;

BOOST_AUTO_TEST_CASE_TEMPLATE(get_value_test, MemoryType, MemoryTypes)
{
    MemoryType curMemory;
    for (typename MemoryType::PositionType i = -100; i < 100; ++i)
    {
        const auto curVal = curMemory.getValue(i);
        BOOST_CHECK_EQUAL(curVal, 0);
    }
}

BOOST_AUTO_TEST_CASE_TEMPLATE(set_value_test, MemoryType, MemoryTypes)
{
    MemoryType curMemory;
    const typename MemoryType::PositionType minInd = -500, maxInd = 500;
    for (typename MemoryType::PositionType i = minInd; i < maxInd; ++i)
    {
        if (i%3 == 0)
        {
            curMemory.setValue(i, -i);
            const auto curVal = curMemory.getValue(i);
            BOOST_CHECK_EQUAL(curVal, -i);
        }
    }
    for (typename MemoryType::PositionType i = minInd; i < maxInd; ++i)
    {
        const typename MemoryType::ValueType curVal = curMemory.getValue(i);
        const typename MemoryType::ValueType targetVal = (i%3 == 0) ? -i : 0;
        BOOST_CHECK_EQUAL(curVal, targetVal);
    }
}

BOOST_AUTO_TEST_CASE_TEMPLATE(incr_decr_value_test, MemoryType, MemoryTypes)
{
    using ValueType = typename MemoryType::ValueType;
    using PositionType = typename MemoryType::PositionType;
    MemoryType curMemory;
    std::mt19937 randomEngine(0);
    std::uniform_int_distribution<PositionType> positionDist{-100, 100};
    std::uniform_int_distribution<std::size_t> numDist{1, 100};
    std::uniform_int_distribution<ValueType> valueDist{-300, 300};

    for (std::size_t testNumber = 0; testNumber < 100; ++testNumber)
    {
        const auto curPosition = positionDist(randomEngine);
        const auto startValue = valueDist(randomEngine);
        const auto plusNum = numDist(randomEngine);
        const auto minusNum = numDist(randomEngine);

        curMemory.setValue(curPosition, startValue);
        for (std::size_t i = 0; i < plusNum; ++i)
        {
            curMemory.increaseValue(curPosition);
        }
        const auto resA = curMemory.getValue(curPosition);
        BOOST_CHECK_EQUAL(resA, startValue+plusNum);

        for (std::size_t i = 0; i < minusNum; ++i)
        {
            curMemory.decreaseValue(curPosition);
        }
        const auto resB = curMemory.getValue(curPosition);
        BOOST_CHECK_EQUAL(resB, startValue+plusNum-ValueType(minusNum));
    }
}

#endif // TESTMEMORYTYPES_HPP_INCLUDED
