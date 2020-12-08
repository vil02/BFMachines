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

BOOST_AUTO_TEST_CASE(findMaching_test)
{
    BOOST_REQUIRE_EQUAL(BFM::Inner::findMaching("[]", 0), 1);
    BOOST_REQUIRE_EQUAL(BFM::Inner::findMaching("[]", 1), 0);
    BOOST_REQUIRE_EQUAL(BFM::Inner::findMaching("[[]]", 0), 3);
    BOOST_REQUIRE_EQUAL(BFM::Inner::findMaching("[[]]", 1), 2);
    BOOST_REQUIRE_EQUAL(BFM::Inner::findMaching("[[]]", 2), 1);
    BOOST_REQUIRE_EQUAL(BFM::Inner::findMaching("[[]]", 3), 0);
    BOOST_REQUIRE_EQUAL(BFM::Inner::findMaching("[[][]]", 0), 5);
    BOOST_REQUIRE_EQUAL(BFM::Inner::findMaching("[[][]]", 3), 4);
    BOOST_REQUIRE_EQUAL(BFM::Inner::findMaching("[[][]]", 4), 3);
    BOOST_REQUIRE_EQUAL(BFM::Inner::findMaching("[[][]]", 5), 0);
    BOOST_REQUIRE_EQUAL(BFM::Inner::findMaching("[[][[]]]", 3), 6);
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
