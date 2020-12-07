#ifndef INPUTVECTORSTREAM_HPP_INCLUDED
#define INPUTVECTORSTREAM_HPP_INCLUDED

#include <exception>

namespace BFM::Streams
{
   template <typename ContainerType>
   class InputVectorStream
   {
        private:
            ContainerType data;
            std::size_t curPosition;
        public:
            explicit InputVectorStream(const ContainerType& inData) :
                data(inData),
                curPosition(0)
            {}
            InputVectorStream& operator>>(typename ContainerType::value_type& outValue)
            {
                if (this->curPosition < this->data.size())
                {
                    outValue = this->data[this->curPosition];
                    ++this->curPosition;
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
