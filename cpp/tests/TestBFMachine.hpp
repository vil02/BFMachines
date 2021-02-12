#ifndef TESTBFMACHINE_HPP_INCLUDED
#define TESTBFMACHINE_HPP_INCLUDED

#include "../BFMachineLib/BFMachineLib.hpp"
#include "BfTestCodes.hpp"

#include "UtilFunctions.hpp"

#include <boost/test/included/unit_test.hpp>
#include <boost/mpl/list.hpp>

#include <vector>
#include <map>
#include <unordered_map>
#include <string_view>
#include <sstream>
#include <numeric>

BOOST_AUTO_TEST_CASE(find_matching_test)
{
    using std::string_literals::operator""s;
    const auto find_matching = bfm::find_matching<
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

BOOST_AUTO_TEST_CASE_TEMPLATE(plus_test, BFMType, bfm_types)
{
    using value_type = typename BFMType::value_type;
    const auto result_fun = util_functions::get_sum_of_vector_fun<value_type>();
    const auto bf_code = bf_test_codes::bf_plus<std::string_view>();
    const value_type deterministic_value_limit = 45;
    const value_type random_value_limit = 400;
    const std::size_t random_test_size = 100;
    util_functions::make_2d_test<BFMType, decltype(bf_code), decltype(result_fun)>(
        bf_code,
        deterministic_value_limit,
        random_value_limit, random_test_size,
        result_fun);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(times_test, BFMType, bfm_types)
{
    using value_type = typename BFMType::value_type;
    const auto result_fun = util_functions::get_product_of_vector_fun<value_type>();
    const auto bf_code = bf_test_codes::bf_times<std::string_view>();
    const value_type deterministic_value_limit = 45;
    const value_type random_value_limit = 100;
    const std::size_t random_test_size = 300;
    util_functions::make_2d_test<BFMType, decltype(bf_code), decltype(result_fun)>(
        bf_code,
        deterministic_value_limit,
        random_value_limit, random_test_size,
        result_fun);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(fibonacci_test, BFMType, bfm_types)
{
    using value_type = typename BFMType::value_type;
    BOOST_REQUIRE_EQUAL(util_functions::fibonacci(0), 0);
    BOOST_REQUIRE_EQUAL(util_functions::fibonacci(1), 1);
    const value_type test_size = 20;
    for (value_type n = 0; n < test_size; ++n)
    {
        util_functions::check_bf_computation<BFMType>(
            bf_test_codes::bf_fibonacci<std::string_view>(),
            {n},
            util_functions::fibonacci(n));
        BOOST_REQUIRE_EQUAL(
            util_functions::fibonacci(n)+util_functions::fibonacci(n+1),
            util_functions::fibonacci(n+2));
    }
}

BOOST_AUTO_TEST_CASE_TEMPLATE(factorial_test, BFMType, bfm_types)
{
    using value_type = typename BFMType::value_type;
    BOOST_REQUIRE_EQUAL(util_functions::factorial(0), 1);
    BOOST_REQUIRE_EQUAL(util_functions::factorial(1), 1);
    const value_type test_size = 8;
    for (value_type n = 0; n < test_size; ++n)
    {
        util_functions::check_bf_computation<BFMType>(
            bf_test_codes::bf_factorial<std::string_view>(),
            {n},
            util_functions::factorial(n));
        BOOST_REQUIRE_EQUAL(util_functions::factorial(n)*(n+1), util_functions::factorial(n+1));
    }
}

BOOST_AUTO_TEST_CASE_TEMPLATE(print_countdown_test, BFMType, bfm_types)
{
    using value_type = typename BFMType::value_type;
    const value_type test_size = 40;
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
    auto i_stream = input_stream_type({});
    const auto hello_world_bf_code = bf_test_codes::bf_hello_world<std::string_view>();
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
        typename bfm::memory_types::ArrayMemory<std::array<value_type, memory_size> >;
    using input_stream_type = bfm::streams::InputStream<std::vector<value_type> >;
    using output_stream_type = bfm::streams::OutputVectorStream<std::vector<value_type> >;
    using bf_machine_type = bfm::BFMachine<memory_type, input_stream_type, output_stream_type>;
    const auto bf_code = bf_test_codes::bf_plus<std::string_view>();
    const auto result_fun = util_functions::get_sum_of_vector_fun<value_type>();
    const value_type deterministic_value_limit = 45;
    const value_type random_value_limit = 400;
    const std::size_t random_test_size = 100;
    util_functions::make_2d_test<bf_machine_type, decltype(bf_code), decltype(result_fun)>(
        bf_code,
        deterministic_value_limit,
        random_value_limit, random_test_size,
        result_fun);

    auto i_stream = input_stream_type({});
    auto o_stream = output_stream_type();
    using std::string_literals::operator""s;
    BOOST_CHECK_THROW(bf_machine_type(i_stream, o_stream).execute(">>>+"s), std::out_of_range);
    BOOST_CHECK_THROW(bf_machine_type(i_stream, o_stream).execute(">>>-"s), std::out_of_range);
    BOOST_CHECK_THROW(bf_machine_type(i_stream, o_stream).execute("<+"s), std::out_of_range);
    BOOST_CHECK_THROW(bf_machine_type(i_stream, o_stream).execute("<-"s), std::out_of_range);
}

#endif // TESTBFMACHINE_HPP_INCLUDED
