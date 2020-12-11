#ifndef ARRAYMEMORY_HPP_INCLUDED
#define ARRAYMEMORY_HPP_INCLUDED

namespace bfm::memory_types
{
    template<typename ContainerType,
         typename ContainerType::size_type starting_position = 0,
         typename ContainerType::value_type default_value = 0>
    class ArrayMemory
    {
        public:
            using value_type = typename ContainerType::value_type;
            using position_type = typename ContainerType::size_type;
        private:
            ContainerType data;
        public:
            constexpr ArrayMemory() :
                data()
            {
                data.fill(default_value);
            }
            [[nodiscard]] constexpr value_type get_value(
                    const position_type& in_position) const noexcept
            {
                return this->data[in_position];
            }
            [[nodiscard]] constexpr position_type get_starting_position() const noexcept
            {
                return starting_position;
            }
            void constexpr set_value(const position_type& in_position, const value_type& in_value)
            {
                this->data[in_position] = in_value;
            }
    };
}

#endif // ARRAYMEMORY_HPP_INCLUDED
