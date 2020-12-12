#ifndef INPUTSTREAM_HPP_INCLUDED
#define INPUTSTREAM_HPP_INCLUDED

#include <exception>

namespace bfm::streams
{
   template <typename ContainerType>
   class InputStream
   {
        private:
            ContainerType data;
            typename ContainerType::size_type cur_position;
        public:
            constexpr explicit InputStream(const ContainerType& in_data) :
                data(in_data),
                cur_position(0)
            {}
            constexpr InputStream& operator>>(typename ContainerType::value_type& out_value)
            {
                out_value = this->data.at(this->cur_position);
                ++this->cur_position;
                return *this;
            }
   };
}

#endif // INPUTVECTORSTREAM_HPP_INCLUDED
