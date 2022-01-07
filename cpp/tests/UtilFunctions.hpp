#ifndef UTILFUNCTIONS_HPP_INCLUDED
#define UTILFUNCTIONS_HPP_INCLUDED

#include <boost/test/included/unit_test.hpp>

#include <algorithm>
#include <vector>
#include <random>
#include <type_traits>

namespace util_functions
{
    template <typename T>
    auto get_all_2d(const T& value_limit)
    {
        std::vector<std::vector<T> > res;
        res.reserve(std::size_t(value_limit)*std::size_t(value_limit));
        for (T val_a = 0; val_a < value_limit; ++val_a)
        {
            for (T val_b = 0; val_b < value_limit; ++val_b)
            {
                const typename decltype(res)::value_type cur_data{val_a, val_b};
                res.push_back(cur_data);
            }
        }
        return res;
    }

    template <typename T>
    auto get_random_2d(
            const T& value_limit,
            const std::size_t number_of_trials,
            const unsigned in_seed = 0)
    {
        std::vector<std::vector<T> > res;
        res.reserve(number_of_trials);
        std::mt19937 random_engine(in_seed);
        std::uniform_int_distribution<T> value_dist{0, value_limit};
        for (std::size_t trial_num = 0; trial_num < number_of_trials; ++trial_num)
        {
            const typename decltype(res)::value_type
                cur_data{value_dist(random_engine), value_dist(random_engine)};
            res.push_back(cur_data);
        }
        return res;
    }

    template <typename T>
    auto get_2d_test_data_set(
          const T& deterministic_value_limit,
          const T& random_value_limit,
          const std::size_t number_of_trials,
          const unsigned in_seed = 0)
    {
        const auto deterministic_data = get_all_2d(deterministic_value_limit);
        const auto random_data = get_random_2d(random_value_limit, number_of_trials, in_seed);
        static_assert(std::is_same_v<
            typename decltype(deterministic_data)::value_type,
            typename decltype(random_data)::value_type>);
        std::vector<typename decltype(deterministic_data)::value_type> res;
        std::set_intersection(
            deterministic_data.begin(), deterministic_data.end(),
            random_data.begin(), random_data.end(),
            res.begin());
        return res;
    }

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

    template<typename BFMType,
             typename CodeType,
             typename SeqType,
             typename ResultFun>
    void check_sequence(
            const CodeType& in_bf_code,
            const SeqType& in_data_seq,
            const ResultFun result_fun)
    {
        for (const auto& cur_data : in_data_seq)
        {
            const auto cur_result = result_fun(cur_data);
            check_bf_computation<BFMType, CodeType>(in_bf_code, cur_data, cur_result);
        }
    }

    template<typename BFMType,
             typename CodeType,
             typename ResultFun>
    void make_2d_test(
            const CodeType& in_bf_code,
            const typename BFMType::value_type& in_deterministic_value_limit,
            const typename BFMType::value_type& in_random_value_limit,
            const std::size_t in_random_test_size,
            const ResultFun& in_result_fun,
            unsigned in_seed = 0)
    {
        using value_type = typename BFMType::value_type;
        const auto test_data = get_2d_test_data_set<value_type>(
            in_deterministic_value_limit, in_random_value_limit, in_random_test_size, in_seed);
        check_sequence<
            BFMType, decltype(in_bf_code), decltype(test_data), decltype(in_result_fun)>(
            in_bf_code, test_data, in_result_fun);
    }

    template <typename T>
    auto get_sum_of_vector_fun()
    {
        return [](const std::vector<T> in_vec)
        {
            return static_cast<T>(std::accumulate(in_vec.begin(), in_vec.end(), T(0)));
        };
    }

    template <typename T>
    auto get_product_of_vector_fun()
    {
        return [](const std::vector<T> in_vec)
        {
            return static_cast<T>(std::accumulate(
                in_vec.begin(), in_vec.end(),
                T(1),
                std::multiplies<T>()));
        };
    }

    template <typename ValueType>
    [[nodiscard]] constexpr ValueType fibonacci(const ValueType& in_num)
    {
        ValueType val_a = 0;
        ValueType val_b = 1;
        for (ValueType cur_num = 0; cur_num < in_num; ++cur_num)
        {
            const auto tmp_val = static_cast<ValueType>(val_a+val_b);
            val_a = val_b;
            val_b = tmp_val;
        }
        return val_a;
    }

    template <typename ValueType>
    [[nodiscard]] constexpr ValueType factorial(const ValueType& in_num)
    {
        ValueType res = 1;
        for (ValueType cur_num = 1; cur_num <= in_num; ++cur_num)
        {
            res = static_cast<ValueType>(res*cur_num);
        }
        return res;
    }
}

#endif // UTILFUNCTIONS_HPP_INCLUDED
