#ifndef BFMSTATE_HPP_INCLUDED
#define BFMSTATE_HPP_INCLUDED

#include <type_traits>

namespace bfm
{
    template <typename MemoryType, typename InputStreamType, typename OutputStreamType>
    class [[nodiscard]] BFMState
    {
    public:
        using memory_type = MemoryType;
        using value_type = typename memory_type::value_type;
        using position_type = typename memory_type::position_type;
        using shift_type = std::make_signed_t<position_type>;
        using i_stream_type = InputStreamType;
        using o_stream_type = OutputStreamType;

    private:
        position_type cur_position = memory_type::get_starting_position();
        memory_type memory = {};
        i_stream_type& input_stream;
        o_stream_type& output_stream;

    public:
        constexpr BFMState(
                i_stream_type& i_stream,
                o_stream_type& o_stream) :
            input_stream(i_stream),
            output_stream(o_stream)
        {}

        [[nodiscard]] constexpr memory_type get_memory() const
        {
            return this->memory;
        }

        [[nodiscard]] constexpr position_type get_memory_position() const
        {
            return this->cur_position;
        }

        constexpr void shift_position(const shift_type in_shift)
        {
            this->cur_position += static_cast<position_type>(in_shift);
        }

        constexpr void move_right()
        {
            this->shift_position(1);
        }

        constexpr void move_left()
        {
            this->shift_position(-1);
        }

        constexpr void increase_value()
        {
            bfm::memory_types::increase_value(this->memory, this->cur_position);
        }

        constexpr void decrease_value()
        {
            bfm::memory_types::decrease_value(this->memory, this->cur_position);
        }

        constexpr void change_value_at_shift(
              const std::make_signed_t<value_type>& in_value_change,
              const shift_type in_shift)
        {
            bfm::memory_types::change_value(
                this->memory, this->cur_position+static_cast<position_type>(in_shift), in_value_change);
        }

        constexpr void change_value(const std::make_signed_t<value_type>& in_value_change)
        {
            change_value_at_shift(in_value_change, 0);
        }

        constexpr void set_value(const value_type& new_val)
        {
            this->memory.set_value(this->cur_position, new_val);
        }


        constexpr void print_value()
        {
            this->output_stream<<this->memory.get_value(this->cur_position);
        }

        constexpr void read_value()
        {
            value_type new_val = value_type();
            this->input_stream>>new_val;
            this->set_value(new_val);
        }

        [[nodiscard]] constexpr value_type get_cur_value() const
        {
            return this->memory.get_value(this->cur_position);
        }

        [[nodiscard]] constexpr bool is_current_value_not_zero() const
        {
            return this->get_cur_value() != value_type(0);
        }
    };
}

#endif // BFMSTATE_HPP_INCLUDED
