#ifndef INPUTVECTORSTREAM_HPP_INCLUDED
#define INPUTVECTORSTREAM_HPP_INCLUDED

#include <exception>

namespace bfm::streams
{
   template <typename ContainerType>
   class InputVectorStream
   {
        private:
            ContainerType data;
            std::size_t cur_position;
        public:
            explicit InputVectorStream(const ContainerType& in_data) :
                data(in_data),
                cur_position(0)
            {}
            InputVectorStream& operator>>(typename ContainerType::value_type& out_value)
            {
                if (this->cur_position < this->data.size())
                {
                    out_value = this->data[this->cur_position];
                    ++this->cur_position;
                }
                else
                {
                    throw std::out_of_range("Out of range.");
                }
                return *this;
            }
   };
}

#endif // INPUTVECTORSTREAM_HPP_INCLUDED
