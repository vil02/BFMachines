#ifndef TESTTRANSLATETOBF_HPP_INCLUDED
#define TESTTRANSLATETOBF_HPP_INCLUDED

#include "../BFMachineLib/BFMachineLib.hpp"
#include "BfTestCodes.hpp"
#include "UtilFunctions.hpp"
#include <boost/test/included/unit_test.hpp>
#include <string>
#include <sstream>
#include <map>
#include <vector>
#include <array>

template <typename CodeType,
          typename PositionType = std::int64_t,
          typename ValueType = std::int64_t>
[[nodiscard]] auto bf_to_bf(const CodeType& in_bf_code)
{
    const auto bf_seq = bfm::parser::bf_code_to_operation_seq<
        CodeType, PositionType, ValueType>(in_bf_code);
    return bfm::bfo::translators::ToBf<>::print(bf_seq);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(translate_bf_plus_test, BFMType, utt::bfm_types)
{
    using value_type = typename BFMType::value_type;
    const auto result_fun = util_functions::get_sum_of_vector_fun<value_type>();
    const auto bf_code = bf_to_bf(bf_test_codes::bf_plus<std::string_view>());
    const value_type deterministic_value_limit = 45;
    const value_type random_value_limit = 400;
    const std::size_t random_test_size = 100;
    util_functions::make_2d_test<BFMType, decltype(bf_code), decltype(result_fun)>(
        bf_code,
        deterministic_value_limit,
        random_value_limit, random_test_size,
        result_fun);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(translate_bf_times_test, BFMType, utt::bfm_types)
{
    using value_type = typename BFMType::value_type;
    const auto result_fun = util_functions::get_product_of_vector_fun<value_type>();
    const auto bf_code = bf_to_bf(bf_test_codes::bf_times<std::string>());
    const value_type deterministic_value_limit = 40;
    const value_type random_value_limit = 400;
    const std::size_t random_test_size = 100;
    util_functions::make_2d_test<BFMType, decltype(bf_code), decltype(result_fun)>(
        bf_code,
        deterministic_value_limit,
        random_value_limit, random_test_size,
        result_fun);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(translate_bf_fibonacci_test, BFMType, utt::bfm_types)
{
    using value_type = typename BFMType::value_type;
    const value_type test_size = 20;
    for (value_type n = 0; n < test_size; ++n)
    {
        util_functions::check_bf_computation<BFMType>(
            bf_to_bf(bf_test_codes::bf_fibonacci<std::string_view>()),
            {n},
            util_functions::fibonacci(n));
    }
}

BOOST_AUTO_TEST_CASE_TEMPLATE(translate_bf_factorial_test, BFMType, utt::bfm_types)
{
    using value_type = typename BFMType::value_type;
    const value_type test_size = 8;
    for (value_type n = 0; n < test_size; ++n)
    {
        util_functions::check_bf_computation<BFMType>(
            bf_to_bf(bf_test_codes::bf_factorial<std::string_view>()),
            {n},
            util_functions::factorial(n));
    }
}

BOOST_AUTO_TEST_CASE_TEMPLATE(translate_bf_hello_world_test, BFMType, utt::bfm_stringstream_no_input_type)
{
    using output_stream_type = typename BFMType::output_stream_type;
    using input_stream_type = typename BFMType::input_stream_type;
    output_stream_type o_stream;
    auto i_stream = input_stream_type({});
    const auto hello_world_bf_code = bf_to_bf(bf_test_codes::bf_hello_world<std::string_view>());
    BFMType(i_stream, o_stream).execute(hello_world_bf_code);
    BOOST_CHECK_EQUAL(o_stream.str(), "Hello World!\n");
    o_stream.str(std::string());
    BFMType(i_stream, o_stream).execute_optimized(hello_world_bf_code);
    BOOST_CHECK_EQUAL(o_stream.str(), "Hello World!\n");
}

#endif // TESTTRANSLATETOBF_HPP_INCLUDED
