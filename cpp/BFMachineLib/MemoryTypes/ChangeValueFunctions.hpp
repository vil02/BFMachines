#ifndef CHANGEVALUEFUNCTIONS_HPP_INCLUDED
#define CHANGEVALUEFUNCTIONS_HPP_INCLUDED

namespace BFM::MemoryTypes
{
    template<typename MemoryType>
    constexpr void increaseValue(
            MemoryType& inMemory,
            const typename MemoryType::PositionType& inPosition)
    {
        inMemory.setValue(inPosition, inMemory.getValue(inPosition)+1);
    }
    template<typename MemoryType>
    constexpr void decreaseValue(
            MemoryType& inMemory,
            const typename MemoryType::PositionType& inPosition)
    {
        inMemory.setValue(inPosition, inMemory.getValue(inPosition)-1);
    }
}

#endif // CHANGEVALUEFUNCTIONS_HPP_INCLUDED
