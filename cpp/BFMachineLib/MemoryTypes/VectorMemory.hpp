#ifndef VECTORMEMORY_HPP_INCLUDED
#define VECTORMEMORY_HPP_INCLUDED

#include <utility>
#include <cstdint>
#include <cmath>
namespace bfm::memory_types
{
    namespace vm_inner
    {
        template<typename ContainerType>
        [[nodiscard]] constexpr auto get_value(
                const ContainerType& in_data,
                const typename ContainerType::size_type& in_position,
                const typename ContainerType::value_type& default_value) noexcept
        {
            return in_position < in_data.size() ? in_data[in_position] : default_value;
        }
        template<typename ContainerType>
        constexpr void set_value(
                ContainerType& in_data,
                const typename ContainerType::size_type& in_position,
                const typename ContainerType::value_type& in_value,
                const typename ContainerType::value_type& default_value)
        {
            if (in_position >= in_data.size())
            {
                if (in_value != default_value)
                {
                    in_data.resize(in_position+1, default_value);
                    in_data[in_position] = in_value;
                }
            }
            else
            {
                in_data[in_position] = in_value;
            }
        }
    }

    template<typename ContainerType, typename ContainerType::value_type default_value = 0>
    class VectorMemory
    {
        public:
            using value_type = typename ContainerType::value_type;
            using position_type = std::ptrdiff_t;
        private:
            ContainerType geq_data, less_data;
            using raw_position_type = std::pair<typename ContainerType::size_type, bool>;
            [[nodiscard]] constexpr raw_position_type get_raw_position(
                    const position_type& in_position) const noexcept
            {
                return in_position >= 0 ?
                    raw_position_type(in_position, true) :
                    raw_position_type(
                        typename ContainerType::size_type(std::abs(in_position+1)),
                        false);
            }
            [[nodiscard]] constexpr value_type get_value(
                    const raw_position_type& in_raw_position) const noexcept
            {
                return in_raw_position.second ?
                    vm_inner::get_value(this->geq_data, in_raw_position.first, default_value) :
                    vm_inner::get_value(this->less_data, in_raw_position.first, default_value);
            }
            void constexpr set_value(
                    const raw_position_type& in_raw_position,
                    const value_type& in_value)
            {
                return in_raw_position.second ?
                    vm_inner::set_value(
                        this->geq_data, in_raw_position.first, in_value, default_value) :
                    vm_inner::set_value(
                        this->less_data, in_raw_position.first, in_value, default_value);
            }
        public:
            constexpr VectorMemory() :
                geq_data(),
                less_data()
            {}
            [[nodiscard]] constexpr value_type get_value(
                    const position_type& in_position) const noexcept
            {
                return this->get_value(this->get_raw_position(in_position));
            }
            [[nodiscard]] constexpr position_type get_starting_position() const noexcept
            {
                return 0;
            }
            void constexpr set_value(const position_type& in_position, const value_type& in_value)
            {
                this->set_value(this->get_raw_position(in_position), in_value);
            }
    };
}

#endif // VECTORMEMORY_HPP_INCLUDED
