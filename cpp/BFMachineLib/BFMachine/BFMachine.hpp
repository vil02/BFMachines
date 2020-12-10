#ifndef BFMACHINE_HPP_INCLUDED
#define BFMACHINE_HPP_INCLUDED

#include <string>
#include <exception>
#include <type_traits>
namespace BFM
{
    namespace Inner
    {
        template <typename CodeType, typename InstructionSet>
        [[nodiscard]] constexpr typename CodeType::size_type findMatching(
                const CodeType& inCode,
                const typename CodeType::size_type startPos)
        {
            static_assert(
                std::is_same<typename CodeType::value_type,
                             typename InstructionSet::InstructionType>::value);
            if (inCode[startPos] != InstructionSet::BeginLoop &&
                inCode[startPos] != InstructionSet::EndLoop)
            {
                throw std::invalid_argument("inCode[startPos] must begin or end loop symbol.");
            }
            const char targetChar =
                inCode[startPos] == InstructionSet::BeginLoop ?
                InstructionSet::EndLoop : InstructionSet::BeginLoop;
            const typename CodeType::difference_type searchDir =
                inCode[startPos] == InstructionSet::BeginLoop ? 1 : -1;

            typename CodeType::difference_type curCount = 0;
            typename CodeType::size_type curPos = startPos+searchDir;
            while (curPos < inCode.size())
            {
                if (inCode[curPos] == targetChar && curCount == 0)
                {
                    break;
                }
                else if (inCode[curPos] == InstructionSet::BeginLoop)
                {
                    ++curCount;
                }
                else if (inCode[curPos] == InstructionSet::EndLoop)
                {
                    --curCount;
                }
                curPos += searchDir;
            }
            if (curPos >= inCode.size())
            {
                throw std::invalid_argument("Could not find matching loop symbol.");
            }
            return curPos;
        }
    }
    struct StandardInstructions
    {
        using InstructionType = char;
        static const InstructionType MoveLeft = '<';
        static const InstructionType MoveRight = '>';
        static const InstructionType IncreaseValue = '+';
        static const InstructionType DecreaseValue = '-';
        static const InstructionType ReadValue = ',';
        static const InstructionType PrintValue = '.';
        static const InstructionType BeginLoop = '[';
        static const InstructionType EndLoop = ']';
    };
    template<typename MemoryType,
             typename InputStream,
             typename OutputStream,
             typename InstructionSet = StandardInstructions>
    class BFMachine
    {
        public:
            using ValueType = typename MemoryType::ValueType;
            using PositionType = typename MemoryType::PositionType;
        private:
            MemoryType memory;
            PositionType curPosition;
            InputStream& inputStream;
            OutputStream& outputStream;
            template <typename CodeType>
            [[nodiscard]] constexpr typename CodeType::size_type executeSingleCommand(
                    const CodeType& inCode,
                    const typename CodeType::size_type inCodePosition)
            {
                typename CodeType::size_type charNum = inCodePosition;
                switch (inCode[charNum])
                {
                    case InstructionSet::MoveRight:
                        ++this->curPosition;
                        ++charNum;
                        break;
                    case InstructionSet::MoveLeft:
                        --this->curPosition;
                        ++charNum;
                        break;
                    case InstructionSet::DecreaseValue:
                        decreaseValue(this->memory, this->curPosition);
                        ++charNum;
                        break;
                    case InstructionSet::IncreaseValue:
                        increaseValue(this->memory, this->curPosition);
                        ++charNum;
                        break;
                    case InstructionSet::PrintValue:
                        this->outputStream<<this->memory.getValue(this->curPosition);
                        ++charNum;
                        break;
                    case InstructionSet::ReadValue:
                        ValueType newVal;
                        this->inputStream>>newVal;
                        this->memory.setValue(this->curPosition, newVal);
                        ++charNum;
                        break;
                    case InstructionSet::BeginLoop:
                        if (this->memory.getValue(this->curPosition) != ValueType(0))
                        {
                            ++charNum;
                        }
                        else
                        {
                            charNum =
                                Inner::findMatching<CodeType, InstructionSet>(inCode, charNum);
                            ++charNum;
                        }
                        break;
                    case InstructionSet::EndLoop:
                        if (this->memory.getValue(this->curPosition) != ValueType(0))
                        {
                            charNum =
                                Inner::findMatching<CodeType, InstructionSet>(inCode, charNum);
                        }
                        else
                        {
                            ++charNum;
                        }
                        break;
                    default:
                        ++charNum;
                        break;
                }
                return charNum;
            }
        public:
            constexpr BFMachine(InputStream& _inputStream, OutputStream& _outputStream) :
                memory(),
                curPosition(memory.getStartingPosition()),
                inputStream(_inputStream),
                outputStream(_outputStream)
            {}
            [[nodiscard]] constexpr MemoryType getMemory() const
            {
                return this->memory;
            }
            [[nodiscard]] constexpr PositionType getMemoryPosition() const
            {
                return this->curPosition;
            }
            template <typename CodeType>
            constexpr void execute(const CodeType& inCode)
            {
                typename CodeType::size_type charNum = 0;
                while (charNum < inCode.size())
                {
                    charNum = this->executeSingleCommand(inCode, charNum);
                }
            }
            template <typename ShowDebugData, typename CodeType>
            constexpr void execute(
                    const CodeType& inCode,
                    ShowDebugData& showDebugData)
            {
                typename CodeType::size_type charNum = 0;
                while (charNum < inCode.size())
                {
                    showDebugData(inCode, charNum, *this);
                    charNum = this->executeSingleCommand(inCode, charNum);
                }
            }
    };
}

#endif // BFMACHINE_HPP_INCLUDED
