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
    using std::string_literals::operator""s;
    const auto findMatching =
            BFM::Inner::findMatching<
                typename std::basic_string<BFM::StandardInstructions::InstructionType>,
                BFM::StandardInstructions>;
    BOOST_REQUIRE_EQUAL(findMatching("[]"s, 0), 1);
    BOOST_REQUIRE_EQUAL(findMatching("[]"s, 1), 0);
    BOOST_REQUIRE_EQUAL(findMatching("[[]]"s, 0), 3);
    BOOST_REQUIRE_EQUAL(findMatching("[[]]"s, 1), 2);
    BOOST_REQUIRE_EQUAL(findMatching("[[]]"s, 2), 1);
    BOOST_REQUIRE_EQUAL(findMatching("[[]]"s, 3), 0);
    BOOST_REQUIRE_EQUAL(findMatching("[[][]]"s, 0), 5);
    BOOST_REQUIRE_EQUAL(findMatching("[[][]]"s, 3), 4);
    BOOST_REQUIRE_EQUAL(findMatching("[[][]]"s, 4), 3);
    BOOST_REQUIRE_EQUAL(findMatching("[[][]]"s, 5), 0);
    BOOST_REQUIRE_EQUAL(findMatching("[[][[]]]"s, 3), 6);
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

template<typename BFMType>
void checkBfComputation(
        const std::string& bfCode,
        const std::vector<typename BFMType::ValueType>& inputVector,
        const typename BFMType::ValueType& resultValue)
{
    using ValueType = typename BFMType::ValueType;
    BFM::Streams::InputVectorStream<std::vector<ValueType> > inputStream(inputVector);
    BFM::Streams::OutputVectorStream<std::vector<ValueType> > outputStream;
    BFMType bfMachine(inputStream, outputStream);
    bfMachine.execute(bfCode);
    BOOST_CHECK_EQUAL(outputStream.getData().size(), 1);
    BOOST_CHECK_EQUAL(outputStream.getData()[0], resultValue);
}

template<typename BFMType, typename TargetValue>
void checkBfComputation2dProduct(
        const std::string& bfCode,
        const typename BFMType::ValueType& valueLimit,
        const TargetValue& targetValue)
{
    using ValueType = typename BFMType::ValueType;
    for (ValueType valA = 0; valA < valueLimit; ++valA)
    {
        for (ValueType valB = 0; valB < valueLimit; ++valB)
        {
            checkBfComputation<BFMType>(bfCode, {valA, valB}, targetValue(valA, valB));
        }
    }
}

template<typename BFMType, typename TargetValue>
void checkBfComputation2dRandom(
        const std::string& bfCode,
        const std::size_t numberOfTrials,
        const typename BFMType::ValueType& valueLimit,
        const TargetValue& targetValue)
{
    using ValueType = typename BFMType::ValueType;
    std::mt19937 randomEngine(0);
    std::uniform_int_distribution<ValueType> valueDist{0, valueLimit};
    for (std::size_t trialNumber = 0; trialNumber < numberOfTrials; ++trialNumber)
    {
        const ValueType valA = valueDist(randomEngine);
        const ValueType valB = valueDist(randomEngine);
        checkBfComputation<BFMType>(bfCode, {valA, valB}, targetValue(valA, valB));
    }
}

BOOST_AUTO_TEST_CASE_TEMPLATE(plus_test, BFMType, BFMTypes)
{
    using ValueType = typename BFMType::ValueType;
    const auto resultValue = [](const ValueType& valA, const ValueType& valB){return valA+valB;};
    using std::string_literals::operator""s;
    const std::string bfPlus = ",>,<[->+<]>."s;
    checkBfComputation2dProduct<BFMType, decltype(resultValue)>(bfPlus, 45, resultValue);
    checkBfComputation2dRandom<BFMType, decltype(resultValue)>(bfPlus, 100, 400, resultValue);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(times_test, BFMType, BFMTypes)
{
    using ValueType = typename BFMType::ValueType;
    const auto resultValue = [](const ValueType& valA, const ValueType& valB){return valA*valB;};
    using std::string_literals::operator""s;
    const std::string bfTimes = ",>,<[>[->+>+<<]>>[-<<+>>]<[->>+<<]<<-]>>>>."s;
    checkBfComputation2dProduct<BFMType, decltype(resultValue)>(bfTimes, 45, resultValue);
    checkBfComputation2dRandom<BFMType, decltype(resultValue)>(bfTimes, 300, 100, resultValue);
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
        using std::string_literals::operator""s;
        bfMachine.execute(",>>+<<[->>[->+>+<<]>>[-<<+>>]<<<[->+<]>>[-<<+>>]<<< ]>."s);
        BOOST_CHECK_EQUAL(outputStream.getData().size(), 1);
        BOOST_CHECK_EQUAL(outputStream.getData()[0], fibonacci(n));
        BOOST_CHECK_EQUAL(fibonacci(n)+fibonacci(n+1), fibonacci(n+2));
    }
}

BOOST_AUTO_TEST_CASE_TEMPLATE(syntax_error_test, BFMType, BFMTypes)
{
    using std::string_literals::operator""s;
    const std::vector<std::string> wrongBfCodes =
        {"[+++++"s,
         "+]+++"s,
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
