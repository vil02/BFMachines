#ifndef BFMDATAREF_HPP_INCLUDED
#define BFMDATAREF_HPP_INCLUDED

namespace bfm
{
    template <typename MemoryType, typename InputStreamType, typename OutputStreamType>
    struct [[nodiscard]] BFMDataRef
    {
        using value_type = typename MemoryType::value_type;
        using position_type = typename MemoryType::position_type;
        using i_stream_type = InputStreamType;
        using o_stream_type = OutputStreamType;

        position_type& cur_position;
        MemoryType& memory;
        i_stream_type& input_stream;
        o_stream_type& output_stream;
        BFMDataRef(
                position_type& in_cur_position,
                MemoryType& in_memory,
                i_stream_type& i_stream,
                o_stream_type& o_stream) :
            cur_position(in_cur_position),
            memory(in_memory),
            input_stream(i_stream),
            output_stream(o_stream)
        {}
    };
}

#endif // BFMDATAREF_HPP_INCLUDED
