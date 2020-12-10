#ifndef VECTORMEMORY_HPP_INCLUDED
#define VECTORMEMORY_HPP_INCLUDED

#include <utility>
#include <cstdint>
#include <cmath>
namespace BFM::MemoryTypes
{
    namespace VMInner
    {
        template<typename ContainerType>
        [[nodiscard]] constexpr auto getValue(
                const ContainerType& inData,
                const typename ContainerType::size_type& inPosition,
                const typename ContainerType::value_type& defaultValue) noexcept
        {
            return inPosition < inData.size() ? inData[inPosition] : defaultValue;
        }
        template<typename ContainerType>
        constexpr void setValue(
                ContainerType& inData,
                const typename ContainerType::size_type& inPosition,
                const typename ContainerType::value_type& inValue,
                const typename ContainerType::value_type& defaultValue)
        {
            if (inPosition >= inData.size())
            {
                if (inValue != defaultValue)
                {
                    inData.resize(inPosition+1, defaultValue);
                    inData[inPosition] = inValue;
                }
            }
            else
            {
                inData[inPosition] = inValue;
            }
        }
    }

    template<typename ContainerType, typename ContainerType::value_type defaultValue = 0>
    class VectorMemory
    {
        public:
            using ValueType = typename ContainerType::value_type;
            using PositionType = std::ptrdiff_t;
        private:
            ContainerType geqData, lessData;
            using RawPositionType = std::pair<typename ContainerType::size_type, bool>;
            [[nodiscard]] constexpr RawPositionType getRawPosition(
                    const PositionType& inPosition) const noexcept
            {
                return inPosition >= 0 ?
                    RawPositionType(inPosition, true) :
                    RawPositionType(
                        typename ContainerType::size_type(std::abs(inPosition+1)),
                        false);
            }
            [[nodiscard]] constexpr ValueType getValue(
                    const RawPositionType& inRawPosition) const noexcept
            {
                return inRawPosition.second ?
                    VMInner::getValue(this->geqData, inRawPosition.first, defaultValue) :
                    VMInner::getValue(this->lessData, inRawPosition.first, defaultValue);
            }
            void constexpr setValue(const RawPositionType& inRawPosition, const ValueType& inValue)
            {
                return inRawPosition.second ?
                    VMInner::setValue(this->geqData, inRawPosition.first, inValue, defaultValue) :
                    VMInner::setValue(this->lessData, inRawPosition.first, inValue, defaultValue);
            }
        public:
            constexpr VectorMemory() :
                geqData(),
                lessData()
            {}
            [[nodiscard]] constexpr ValueType getValue(
                    const PositionType& inPosition) const noexcept
            {
                return this->getValue(this->getRawPosition(inPosition));
            }
            [[nodiscard]] constexpr PositionType getStartingPosition() const noexcept
            {
                return 0;
            }
            void constexpr setValue(const PositionType& inPosition, const ValueType& inValue)
            {
                this->setValue(this->getRawPosition(inPosition), inValue);
            }
    };
}

#endif // VECTORMEMORY_HPP_INCLUDED
