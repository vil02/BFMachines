#ifndef OUTPUTVECTORSTREAM_HPP_INCLUDED
#define OUTPUTVECTORSTREAM_HPP_INCLUDED

namespace BFM::Streams
{
   template <typename ContainerType>
   class OutputVectorStream
   {
        private:
            ContainerType data;
        public:
            OutputVectorStream() :
                data()
            {}
            OutputVectorStream& operator<<(const typename ContainerType::value_type& inValue)
            {
                this->data.push_back(inValue);
                return *this;
            }
            [[nodiscard]] ContainerType getData() const noexcept
            {
                return this->data;
            }
   };
}


#endif // OUTPUTVECTORSTREAM_HPP_INCLUDED
