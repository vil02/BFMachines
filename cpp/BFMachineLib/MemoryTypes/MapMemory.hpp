#ifndef MAPMEMORY_HPP_INCLUDED
#define MAPMEMORY_HPP_INCLUDED

namespace BFM::MemoryTypes
{
    template<typename ContainerType,
             bool removeDefaultValues = true,
             typename ContainerType::mapped_type defaultValue = 0>
    class MapMemory
    {
        public:
            using ValueType = typename ContainerType::mapped_type;
            using PositionType = typename ContainerType::key_type;
        private:
            ContainerType data;
        public:
            constexpr MapMemory() :
                data()
            {}
            [[nodiscard]] constexpr ValueType getValue(
                    const PositionType &inPosition) const noexcept
            {
                const auto valueIt = this->data.find(inPosition);
                return valueIt != this->data.end() ? valueIt->second : defaultValue;
            }
            [[nodiscard]] constexpr PositionType getStartingPosition() const noexcept
            {
                return 0;
            }
            constexpr void setValue(const PositionType &inPosition, const ValueType& inValue)
            {
                const auto valueIt = this->data.find(inPosition);
                if (valueIt != this->data.end())
                {
                    this->data[inPosition] = inValue;
                    if constexpr (removeDefaultValues)
                    {
                        if (inValue == defaultValue)
                        {
                            this->data.erase(valueIt);
                        }
                    }
                }
                else if (inValue != defaultValue)
                {
                    this->data[inPosition] = inValue;
                }
            }
            constexpr void increaseValue(const PositionType &inPosition)
            {
                this->setValue(inPosition, this->getValue(inPosition)+1);
            }
            constexpr void decreaseValue(const PositionType &inPosition)
            {
                this->setValue(inPosition, this->getValue(inPosition)-1);
            }
    };
}

#endif // MAPMEMORY_HPP_INCLUDED
