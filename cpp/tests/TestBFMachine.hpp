#ifndef TESTBFMACHINE_HPP_INCLUDED
#define TESTBFMACHINE_HPP_INCLUDED

#include <boost/test/included/unit_test.hpp>
#include <boost/mpl/list.hpp>

#include <vector>
#include <map>
#include <unordered_set>
#include <random>
#include <string_view>
#include <sstream>
#include "../BFMachineLib/BFMachineLib.hpp"
#include "BfTestCodes.hpp"
BOOST_AUTO_TEST_CASE(find_matching_test)
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

struct FlippedInstructions
{
    using instruction_type = typename bfm::StandardInstructions::instruction_type;
    static const instruction_type move_left = bfm::StandardInstructions::move_right;
    static const instruction_type move_right = bfm::StandardInstructions::move_left;
    static const instruction_type increase_value = bfm::StandardInstructions::increase_value;
    static const instruction_type decrease_value = bfm::StandardInstructions::decrease_value;
    static const instruction_type read_value = bfm::StandardInstructions::read_value;
    static const instruction_type print_value = bfm::StandardInstructions::print_value;
    static const instruction_type begin_loop = bfm::StandardInstructions::begin_loop;
    static const instruction_type end_loop = bfm::StandardInstructions::end_loop;
};

typedef boost::mpl::list<
    bfm::BFMachine<bfm::memory_types::VectorMemory<std::vector<int> >,
        bfm::streams::InputStream<std::vector<int> >,
        bfm::streams::OutputVectorStream<std::vector<int> > >,
    bfm::BFMachine<bfm::memory_types::VectorMemory<std::vector<unsigned> >,
        bfm::streams::InputStream<std::vector<unsigned> >,
        bfm::streams::OutputVectorStream<std::vector<unsigned> > >,
    bfm::BFMachine<bfm::memory_types::VectorMemory<std::vector<int> >,
        bfm::streams::InputStream<std::vector<int> >,
        bfm::streams::OutputVectorStream<std::vector<int> >,
        FlippedInstructions>,
    bfm::BFMachine<bfm::memory_types::MapMemory<std::map<int, int> >,
        bfm::streams::InputStream<std::vector<int> >,
        bfm::streams::OutputVectorStream<std::vector<int> > >,
    bfm::BFMachine<bfm::memory_types::MapMemory<std::map<int, int>, false>,
        bfm::streams::InputStream<std::vector<int> >,
        bfm::streams::OutputVectorStream<std::vector<int> > >,
    bfm::BFMachine<bfm::memory_types::MapMemory<std::unordered_map<int, int> >,
        bfm::streams::InputStream<std::vector<int> >,
        bfm::streams::OutputVectorStream<std::vector<int> > >,
    bfm::BFMachine<bfm::memory_types::MapMemory<std::unordered_map<int, int>, false>,
        bfm::streams::InputStream<std::vector<int> >,
        bfm::streams::OutputVectorStream<std::vector<int> > >
                        > bfm_types;

typedef boost::mpl::list<
    bfm::memory_types::VectorMemory<std::vector<char> >,
    bfm::memory_types::VectorMemory<std::vector<unsigned char> >,
    bfm::memory_types::MapMemory<std::map<int, char> >,
    bfm::memory_types::MapMemory<std::map<int, unsigned char> >,
    bfm::memory_types::MapMemory<std::unordered_map<int, char> >,
    bfm::memory_types::MapMemory<std::unordered_map<int, unsigned char> >
                        > char_memory_types;


template<typename BFMType, typename CodeType>
void check_interpreted_bf_computation(
        const CodeType& bf_code,
        const std::vector<typename BFMType::value_type>& input_vector,
        const typename BFMType::value_type& result_value)
{
    using value_type = typename BFMType::value_type;
    bfm::streams::InputStream<std::vector<value_type> > input_stream(input_vector);
    bfm::streams::OutputVectorStream<std::vector<value_type> > output_stream;
    BFMType bf_machine(input_stream, output_stream);
    bf_machine.execute(bf_code);
    BOOST_CHECK_EQUAL(output_stream.get_data().size(), 1);
    BOOST_CHECK_EQUAL(output_stream.get_data()[0], result_value);
}

template<typename BFMType, typename CodeType>
void check_optimized_bf_computation(
        const CodeType& bf_code,
        const std::vector<typename BFMType::value_type>& input_vector,
        const typename BFMType::value_type& result_value)
{
    using value_type = typename BFMType::value_type;
    bfm::streams::InputStream<std::vector<value_type> > input_stream(input_vector);
    bfm::streams::OutputVectorStream<std::vector<value_type> > output_stream;
    BFMType bf_machine(input_stream, output_stream);
    bf_machine.execute_optimized(bf_code);
    BOOST_CHECK_EQUAL(output_stream.get_data().size(), 1);
    BOOST_CHECK_EQUAL(output_stream.get_data()[0], result_value);
}

template<typename BFMType, typename CodeType>
void check_bf_computation(
        const CodeType& bf_code,
        const std::vector<typename BFMType::value_type>& input_vector,
        const typename BFMType::value_type& result_value)
{
    check_interpreted_bf_computation<BFMType, CodeType>(bf_code, input_vector, result_value);
    check_optimized_bf_computation<BFMType, CodeType>(bf_code, input_vector, result_value);
}

template<typename BFMType, typename CodeType, typename TargetValue>
void check_bf_computation_2d_product(
        const CodeType& bf_code,
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

template<typename BFMType, typename CodeType, typename TargetValue>
void check_bf_computation_2d_random(
        const CodeType& bf_code,
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
    const auto bf_plus = bf_test_codes::bf_plus<std::string_view>();
    check_bf_computation_2d_product<BFMType, decltype(bf_plus), decltype(result_value)>(
        bf_plus, 45, result_value);
    check_bf_computation_2d_random<BFMType, decltype(bf_plus), decltype(result_value)>(
        bf_plus, 100, 400, result_value);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(times_test, BFMType, bfm_types)
{
    using value_type = typename BFMType::value_type;
    const auto result_value =
        [](const value_type& val_a, const value_type& val_b){return val_a*val_b;};
    const auto bf_times = bf_test_codes::bf_times<std::string_view>();
    check_bf_computation_2d_product<BFMType, decltype(bf_times), decltype(result_value)>(
        bf_times, 45, result_value);
    check_bf_computation_2d_random<BFMType, decltype(bf_times), decltype(result_value)>(
        bf_times, 300, 100, result_value);
}

template <typename ValueType>
[[nodiscard]] constexpr ValueType fibonacci(const ValueType& in_num)
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
    BOOST_REQUIRE_EQUAL(fibonacci(0), 0);
    BOOST_REQUIRE_EQUAL(fibonacci(1), 1);
    for (value_type n = 0; n < 20; ++n)
    {
        bfm::streams::InputStream<std::vector<value_type> > i_stream({n});
        bfm::streams::OutputVectorStream<std::vector<value_type> > o_stream;
        BFMType bf_machine(i_stream, o_stream);
        bf_machine.execute(bf_test_codes::bf_fibonacci<std::string>());
        BOOST_CHECK_EQUAL(o_stream.get_data().size(), 1);
        BOOST_CHECK_EQUAL(o_stream.get_data()[0], fibonacci(n));
        BOOST_REQUIRE_EQUAL(fibonacci(n)+fibonacci(n+1), fibonacci(n+2));
    }
}

template <typename ValueType>
[[nodiscard]] constexpr ValueType factorial(const ValueType& in_num)
{
    ValueType res = 1;
    for (ValueType cur_num = 1; cur_num <= in_num; ++cur_num)
    {
        res *= cur_num;
    }
    return res;
}

BOOST_AUTO_TEST_CASE_TEMPLATE(factorial_test, BFMType, bfm_types)
{
    using value_type = typename BFMType::value_type;
    BOOST_REQUIRE_EQUAL(factorial(0), 1);
    BOOST_REQUIRE_EQUAL(factorial(1), 1);
    for (value_type n = 0; n < 8; ++n)
    {
        bfm::streams::InputStream<std::vector<value_type> > i_stream({n});
        bfm::streams::OutputVectorStream<std::vector<value_type> > o_stream;
        BFMType bf_machine(i_stream, o_stream);
        bf_machine.execute(bf_test_codes::bf_factorial<std::string_view>());
        BOOST_CHECK_EQUAL(o_stream.get_data().size(), 1);
        BOOST_CHECK_EQUAL(o_stream.get_data()[0], factorial(n));
        BOOST_REQUIRE_EQUAL(factorial(n)*(n+1), factorial(n+1));
    }
}

BOOST_AUTO_TEST_CASE_TEMPLATE(print_countdown_test, BFMType, bfm_types)
{
    using value_type = typename BFMType::value_type;
    const value_type  test_size = 40;
    for (value_type n = 0; n < test_size; ++n)
    {
        bfm::streams::InputStream<std::vector<value_type> > i_stream({n});
        bfm::streams::OutputVectorStream<std::vector<value_type> > o_stream;
        BFMType bf_machine(i_stream, o_stream);
        bf_machine.execute(bf_test_codes::bf_print_countdown<std::string_view>());
        BOOST_CHECK_EQUAL(o_stream.get_data().size(), n);
        value_type target_val = n;
        for (const value_type res_val : o_stream.get_data())
        {
            BOOST_CHECK_EQUAL(target_val, res_val);
            --target_val;
        }
    }
    for (value_type n = 0; n < test_size; ++n)
    {
        bfm::streams::InputStream<std::vector<value_type> > i_stream({n});
        bfm::streams::OutputVectorStream<std::vector<value_type> > o_stream;
        BFMType bf_machine(i_stream, o_stream);
        bf_machine.execute_optimized(bf_test_codes::bf_print_countdown<std::string_view>());
        BOOST_CHECK_EQUAL(o_stream.get_data().size(), n);
        value_type target_val = n;
        for (const value_type res_val : o_stream.get_data())
        {
            BOOST_CHECK_EQUAL(target_val, res_val);
            --target_val;
        }
    }
}

BOOST_AUTO_TEST_CASE_TEMPLATE(syntax_error_test, BFMType, bfm_types)
{
    using std::string_literals::operator""s;
    using value_type = typename BFMType::value_type;
    const std::vector<std::string> wrong_bf_codes =
        {"[+++++"s,
         "+]+++"s,
        };
    for (const auto& cur_code : wrong_bf_codes)
    {
        bfm::streams::InputStream<std::vector<value_type> > i_stream({});
        bfm::streams::OutputVectorStream<std::vector<value_type> > o_stream;
        BOOST_CHECK_THROW(BFMType(i_stream, o_stream).execute(cur_code), std::invalid_argument);
    }
}

BOOST_AUTO_TEST_CASE_TEMPLATE(hello_world_test, MemoryType, char_memory_types)
{
    std::stringstream ss;
    using input_stream_type =
        typename bfm::streams::InputStream<std::array<typename MemoryType::value_type, 0> >;
    using output_stream_type = decltype(ss);
    using bfm_type =
        typename bfm::BFMachine<MemoryType, input_stream_type, output_stream_type>;
    input_stream_type i_stream = input_stream_type({});
    std::string_view hello_world_bf_code =
        "++++++++[>++++[>++>+++>+++>+<<<<-]>+>+>->>+[<]<-]>>.>"
        "---.+++++++..+++.>>.<-.<.+++.------.--------.>>+.>++.";
    bfm_type(i_stream, ss).execute(hello_world_bf_code);
    BOOST_CHECK_EQUAL(ss.str(), "Hello World!\n");
    ss.str(std::string());
    bfm_type(i_stream, ss).execute_optimized(hello_world_bf_code);
    BOOST_CHECK_EQUAL(ss.str(), "Hello World!\n");
}

BOOST_AUTO_TEST_CASE(bfm_with_array_memory_test)
{
    using value_type = int;
    constexpr std::size_t memory_size = 2;
    using memory_type =
        typename bfm::memory_types::ArrayMemory<
            std::array<value_type, memory_size> >;
    using input_stream_type = bfm::streams::InputStream<std::vector<value_type> >;
    using output_stream_type = bfm::streams::OutputVectorStream<std::vector<value_type> >;
    using bf_machine_type = bfm::BFMachine<memory_type, input_stream_type, output_stream_type>;
    const auto bf_plus = bf_test_codes::bf_plus<std::string_view>();
    const auto result_value =
        [](const value_type& val_a, const value_type& val_b){return val_a+val_b;};
    check_bf_computation_2d_product<bf_machine_type, decltype(bf_plus), decltype(result_value)>(
        bf_plus, 45, result_value);
    check_bf_computation_2d_random<bf_machine_type, decltype(bf_plus), decltype(result_value)>(
        bf_plus, 100, 400, result_value);

    auto i_stream = input_stream_type({});
    auto o_stream = output_stream_type();
    using std::string_literals::operator""s;
    BOOST_CHECK_THROW(bf_machine_type(i_stream, o_stream).execute(">>>+"s), std::out_of_range);
    BOOST_CHECK_THROW(bf_machine_type(i_stream, o_stream).execute(">>>-"s), std::out_of_range);
    BOOST_CHECK_THROW(bf_machine_type(i_stream, o_stream).execute("<+"s), std::out_of_range);
    BOOST_CHECK_THROW(bf_machine_type(i_stream, o_stream).execute("<-"s), std::out_of_range);
}

#endif // TESTBFMACHINE_HPP_INCLUDED
