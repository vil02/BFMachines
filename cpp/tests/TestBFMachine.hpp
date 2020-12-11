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
    const auto find_matching =
        bfm::inner::find_matching<
            typename std::basic_string<bfm::StandardInstructions::instruction_type>,
            bfm::StandardInstructions>;
    BOOST_REQUIRE_EQUAL(find_matching("[]"s, 0), 1);
    BOOST_REQUIRE_EQUAL(find_matching("[]"s, 1), 0);
    BOOST_REQUIRE_EQUAL(find_matching("[[]]"s, 0), 3);
    BOOST_REQUIRE_EQUAL(find_matching("[[]]"s, 1), 2);
    BOOST_REQUIRE_EQUAL(find_matching("[[]]"s, 2), 1);
    BOOST_REQUIRE_EQUAL(find_matching("[[]]"s, 3), 0);
    BOOST_REQUIRE_EQUAL(find_matching("[[][]]"s, 0), 5);
    BOOST_REQUIRE_EQUAL(find_matching("[[][]]"s, 3), 4);
    BOOST_REQUIRE_EQUAL(find_matching("[[][]]"s, 4), 3);
    BOOST_REQUIRE_EQUAL(find_matching("[[][]]"s, 5), 0);
    BOOST_REQUIRE_EQUAL(find_matching("[[][[]]]"s, 3), 6);
}

typedef boost::mpl::list<
    bfm::BFMachine<bfm::memory_types::VectorMemory<std::vector<int> >,
        bfm::streams::InputVectorStream<std::vector<int> >,
        bfm::streams::OutputVectorStream<std::vector<int> > >,
    bfm::BFMachine<bfm::memory_types::MapMemory<std::map<int, int> >,
        bfm::streams::InputVectorStream<std::vector<int> >,
        bfm::streams::OutputVectorStream<std::vector<int> > >,
    bfm::BFMachine<bfm::memory_types::MapMemory<std::map<int, int>, false>,
        bfm::streams::InputVectorStream<std::vector<int> >,
        bfm::streams::OutputVectorStream<std::vector<int> > >,
    bfm::BFMachine<bfm::memory_types::MapMemory<std::unordered_map<int, int> >,
        bfm::streams::InputVectorStream<std::vector<int> >,
        bfm::streams::OutputVectorStream<std::vector<int> > >,
    bfm::BFMachine<bfm::memory_types::MapMemory<std::unordered_map<int, int>, false>,
        bfm::streams::InputVectorStream<std::vector<int> >,
        bfm::streams::OutputVectorStream<std::vector<int> > >
                        > bfm_types;
template<typename BFMType>
void check_bf_computation(
        const std::string& bf_code,
        const std::vector<typename BFMType::value_type>& input_vector,
        const typename BFMType::value_type& result_value)
{
    using value_type = typename BFMType::value_type;
    bfm::streams::InputVectorStream<std::vector<value_type> > input_stream(input_vector);
    bfm::streams::OutputVectorStream<std::vector<value_type> > output_stream;
    BFMType bf_machine(input_stream, output_stream);
    bf_machine.execute(bf_code);
    BOOST_CHECK_EQUAL(output_stream.get_data().size(), 1);
    BOOST_CHECK_EQUAL(output_stream.get_data()[0], result_value);
}

template<typename BFMType, typename TargetValue>
void check_bf_computation_2d_product(
        const std::string& bf_code,
        const typename BFMType::value_type& value_limit,
        const TargetValue& target_value)
{
    using value_type = typename BFMType::value_type;
    for (value_type val_a = 0; val_a < value_limit; ++val_a)
    {
        for (value_type val_b = 0; val_b < value_limit; ++val_b)
        {
            check_bf_computation<BFMType>(bf_code, {val_a, val_b}, target_value(val_a, val_b));
        }
    }
}

template<typename BFMType, typename TargetValue>
void check_bf_computation_2d_random(
        const std::string& bf_code,
        const std::size_t number_of_trials,
        const typename BFMType::value_type& value_limit,
        const TargetValue& target_value)
{
    using value_type = typename BFMType::value_type;
    std::mt19937 random_engine(0);
    std::uniform_int_distribution<value_type> value_dist{0, value_limit};
    for (std::size_t trial_number = 0; trial_number < number_of_trials; ++trial_number)
    {
        const value_type val_a = value_dist(random_engine);
        const value_type val_b = value_dist(random_engine);
        check_bf_computation<BFMType>(bf_code, {val_a, val_b}, target_value(val_a, val_b));
    }
}

BOOST_AUTO_TEST_CASE_TEMPLATE(plus_test, BFMType, bfm_types)
{
    using value_type = typename BFMType::value_type;
    const auto result_value =
        [](const value_type& val_a, const value_type& val_b){return val_a+val_b;};
    using std::string_literals::operator""s;
    const std::string bf_plus = ",>,<[->+<]>."s;
    check_bf_computation_2d_product<BFMType, decltype(result_value)>(
        bf_plus, 45, result_value);
    check_bf_computation_2d_random<BFMType, decltype(result_value)>(
        bf_plus, 100, 400, result_value);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(times_test, BFMType, bfm_types)
{
    using value_type = typename BFMType::value_type;
    const auto result_value =
        [](const value_type& val_a, const value_type& val_b){return val_a*val_b;};
    using std::string_literals::operator""s;
    const std::string bf_times = ",>,<[>[->+>+<<]>>[-<<+>>]<[->>+<<]<<-]>>>>."s;
    check_bf_computation_2d_product<BFMType, decltype(result_value)>(bf_times, 45, result_value);
    check_bf_computation_2d_random<BFMType, decltype(result_value)>(bf_times, 300, 100, result_value);
}

template <typename ValueType>
constexpr ValueType fibonacci(const ValueType& in_num)
{
    ValueType val_a = 0;
    ValueType val_b = 1;
    for (ValueType cur_num = 0; cur_num < in_num; ++cur_num)
    {
        const auto tmp_val = val_a+val_b;
        val_a = val_b;
        val_b = tmp_val;
    }
    return val_a;
}

BOOST_AUTO_TEST_CASE_TEMPLATE(fibonacci_test, BFMType, bfm_types)
{
    using value_type = typename BFMType::value_type;
    BOOST_CHECK_EQUAL(fibonacci(0), 0);
    BOOST_CHECK_EQUAL(fibonacci(1), 1);
    for (value_type n = 0; n < 20; ++n)
    {
        bfm::streams::InputVectorStream<std::vector<int> > i_stream({n});
        bfm::streams::OutputVectorStream<std::vector<int> > o_stream;
        BFMType bf_machine(i_stream, o_stream);
        using std::string_literals::operator""s;
        bf_machine.execute(",>>+<<[->>[->+>+<<]>>[-<<+>>]<<<[->+<]>>[-<<+>>]<<< ]>."s);
        BOOST_CHECK_EQUAL(o_stream.get_data().size(), 1);
        BOOST_CHECK_EQUAL(o_stream.get_data()[0], fibonacci(n));
        BOOST_CHECK_EQUAL(fibonacci(n)+fibonacci(n+1), fibonacci(n+2));
    }
}

BOOST_AUTO_TEST_CASE_TEMPLATE(syntax_error_test, BFMType, bfm_types)
{
    using std::string_literals::operator""s;
    const std::vector<std::string> wrong_bf_codes =
        {"[+++++"s,
         "+]+++"s,
        };
    for (const auto cur_code : wrong_bf_codes)
    {
        bfm::streams::InputVectorStream<std::vector<int> > i_stream({});
        bfm::streams::OutputVectorStream<std::vector<int> > o_stream;
        BFMType bf_machine(i_stream, o_stream);
        BOOST_CHECK_THROW(bf_machine.execute(cur_code), std::invalid_argument);
    }
}

#endif // TESTBFMACHINE_HPP_INCLUDED
