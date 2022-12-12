#ifndef MAPMEMORY_HPP_INCLUDED
#define MAPMEMORY_HPP_INCLUDED

namespace bfm::memory_types
{
    template<typename ContainerType,
             bool remove_default_values = true,
             typename ContainerType::mapped_type default_value = 0>
    class [[nodiscard]] MapMemory
    {
        public:
            using value_type = typename ContainerType::mapped_type;
            using position_type = typename ContainerType::key_type;
        private:
            ContainerType data = {};
        public:
            constexpr MapMemory() = default;
            [[nodiscard]] constexpr value_type get_value(
                    const position_type& in_position) const noexcept
            {
                const auto value_it = this->data.find(in_position);
                return value_it != this->data.end() ? value_it->second : default_value;
            }
            [[nodiscard]] constexpr static position_type get_starting_position() noexcept
            {
                return 0;
            }
            constexpr void set_value(const position_type& in_position, const value_type& in_value)
            {
                [[maybe_unused]] const auto value_it = this->data.insert_or_assign(in_position, in_value).first;
                if constexpr (remove_default_values)
                {
                    if (in_value == default_value)
                    {
                        this->data.erase(value_it);
                    }
                }
            }
    };
}

#endif // MAPMEMORY_HPP_INCLUDED
