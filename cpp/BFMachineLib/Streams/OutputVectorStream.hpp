#ifndef OUTPUTVECTORSTREAM_HPP_INCLUDED
#define OUTPUTVECTORSTREAM_HPP_INCLUDED

namespace bfm::streams
{
   template <typename ContainerType>
   class [[nodiscard]] OutputVectorStream
   {
        private:
            ContainerType data;
        public:
            OutputVectorStream() :
                data()
            {}
            OutputVectorStream& operator<<(const typename ContainerType::value_type& in_value)
            {
                this->data.push_back(in_value);
                return *this;
            }
            [[nodiscard]] ContainerType get_data() const noexcept
            {
                return this->data;
            }
   };
}


#endif // OUTPUTVECTORSTREAM_HPP_INCLUDED
