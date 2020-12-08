#ifndef TESTBFMACHINE_HPP_INCLUDED
#define TESTBFMACHINE_HPP_INCLUDED

#include <boost/test/included/unit_test.hpp>
#include <boost/mpl/list.hpp>

#include <vector>
#include <map>
#include <unordered_set>
#include <random>
#include <exception>

#include "../BFMachineLib/BFMachineLib.hpp"

BOOST_AUTO_TEST_CASE(findMatching_test)
{
    BOOST_REQUIRE_EQUAL(BFM::Inner::findMatching("[]", 0), 1);
    BOOST_REQUIRE_EQUAL(BFM::Inner::findMatching("[]", 1), 0);
    BOOST_REQUIRE_EQUAL(BFM::Inner::findMatching("[[]]", 0), 3);
    BOOST_REQUIRE_EQUAL(BFM::Inner::findMatching("[[]]", 1), 2);
    BOOST_REQUIRE_EQUAL(BFM::Inner::findMatching("[[]]", 2), 1);
    BOOST_REQUIRE_EQUAL(BFM::Inner::findMatching("[[]]", 3), 0);
    BOOST_REQUIRE_EQUAL(BFM::Inner::findMatching("[[][]]", 0), 5);
    BOOST_REQUIRE_EQUAL(BFM::Inner::findMatching("[[][]]", 3), 4);
    BOOST_REQUIRE_EQUAL(BFM::Inner::findMatching("[[][]]", 4), 3);
    BOOST_REQUIRE_EQUAL(BFM::Inner::findMatching("[[][]]", 5), 0);
    BOOST_REQUIRE_EQUAL(BFM::Inner::findMatching("[[][[]]]", 3), 6);
}

typedef boost::mpl::list<BFM::BFMachine<BFM::MemoryTypes::VectorMemory<std::vector<int> >,
                                        BFM::Streams::InputVectorStream<std::vector<int> >,
                                        BFM::Streams::OutputVectorStream<std::vector<int> > >,
                         BFM::BFMachine<BFM::MemoryTypes::MapMemory<std::map<int, int> >,
                                        BFM::Streams::InputVectorStream<std::vector<int> >,
                                        BFM::Streams::OutputVectorStream<std::vector<int> > >,
                         BFM::BFMachine<BFM::MemoryTypes::MapMemory<std::map<int, int>, false>,
                                        BFM::Streams::InputVectorStream<std::vector<int> >,
                                        BFM::Streams::OutputVectorStream<std::vector<int> > >,
                         BFM::BFMachine<BFM::MemoryTypes::MapMemory<std::unordered_map<int, int> >,
                                        BFM::Streams::InputVectorStream<std::vector<int> >,
                                        BFM::Streams::OutputVectorStream<std::vector<int> > >,
                         BFM::BFMachine<BFM::MemoryTypes::MapMemory<std::unordered_map<int, int>, false>,
                                        BFM::Streams::InputVectorStream<std::vector<int> >,
                                        BFM::Streams::OutputVectorStream<std::vector<int> > >
                        > BFMTypes;

BOOST_AUTO_TEST_CASE_TEMPLATE(plus_test, BFMType, BFMTypes)
{
    using ValueType = typename BFMType::ValueType;
    std::mt19937 randomEngine(0);
    std::uniform_int_distribution<ValueType> valueDist{0, 300};
    for (std::size_t tryNum = 0; tryNum < 100; ++tryNum)
    {
        const ValueType valA = valueDist(randomEngine);
        const ValueType valB = valueDist(randomEngine);
        BFM::Streams::InputVectorStream<std::vector<int> > inputStream({valA, valB});
        BFM::Streams::OutputVectorStream<std::vector<int> > outputStream;
        BFMType bfMachine(inputStream, outputStream);
        bfMachine.execute(",>,<[->+<]>.");
        BOOST_CHECK_EQUAL(outputStream.getData().size(), 1);
        BOOST_CHECK_EQUAL(outputStream.getData()[0], valA+valB);
    }
}

template <typename ValueType>
constexpr ValueType fibonacci(const ValueType& inNum)
{
    ValueType valA = 0;
    ValueType valB = 1;
    for (ValueType curNum = 0; curNum < inNum; ++curNum)
    {
        const auto tmpVal = valA+valB;
        valA = valB;
        valB = tmpVal;
    }
    return valA;
}

BOOST_AUTO_TEST_CASE_TEMPLATE(fibonacci_test, BFMType, BFMTypes)
{
    using ValueType = typename BFMType::ValueType;
    BOOST_CHECK_EQUAL(fibonacci(0), 0);
    BOOST_CHECK_EQUAL(fibonacci(1), 1);
    for (ValueType n = 0; n < 20; ++n)
    {
        BFM::Streams::InputVectorStream<std::vector<int> > inputStream({n});
        BFM::Streams::OutputVectorStream<std::vector<int> > outputStream;
        BFMType bfMachine(inputStream, outputStream);
        bfMachine.execute(",>>+<<[->>[->+>+<<]>>[-<<+>>]<<<[->+<]>>[-<<+>>]<<< ]>.");
        BOOST_CHECK_EQUAL(outputStream.getData().size(), 1);
        BOOST_CHECK_EQUAL(outputStream.getData()[0], fibonacci(n));
        BOOST_CHECK_EQUAL(fibonacci(n)+fibonacci(n+1), fibonacci(n+2));
    }
}

BOOST_AUTO_TEST_CASE_TEMPLATE(syntax_error_test, BFMType, BFMTypes)
{
    const std::vector<std::string> wrongBfCodes =
        {"[+++++",
         "]+++",
        };
    for (const auto curCode : wrongBfCodes)
    {
        BFM::Streams::InputVectorStream<std::vector<int> > inputStream({});
        BFM::Streams::OutputVectorStream<std::vector<int> > outputStream;
        BFMType bfMachine(inputStream, outputStream);
        BOOST_CHECK_THROW(bfMachine.execute(curCode), std::invalid_argument);
    }
}

#endif // TESTBFMACHINE_HPP_INCLUDED
