#ifndef OUTPUTARRAYSTREAM_HPP_INCLUDED
#define OUTPUTARRAYSTREAM_HPP_INCLUDED

namespace bfm::streams
{
   template <typename ContainerType>
   class [[nodiscard]] OutputArrayStream
   {
        private:
            using position_type = typename ContainerType::size_type;
            ContainerType data = {};
            position_type cur_position = 0;
        public:
            constexpr OutputArrayStream() = default;
            constexpr OutputArrayStream& operator<<(
                    const typename ContainerType::value_type& in_value)
            {
                this->data.at(cur_position) = in_value;
                ++cur_position;
                return *this;
            }
            [[nodiscard]] constexpr ContainerType get_data() const noexcept
            {
                return this->data;
            }
            [[nodiscard]] constexpr position_type get_output_size() const noexcept
            {
                return this->cur_position;
            }
   };
}

#endif // OUTPUTARRAYSTREAM_HPP_INCLUDED
