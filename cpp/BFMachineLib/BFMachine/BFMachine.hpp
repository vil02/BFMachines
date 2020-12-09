#ifndef BFMACHINE_HPP_INCLUDED
#define BFMACHINE_HPP_INCLUDED

#include <string>
#include <exception>

namespace BFM
{
    namespace Inner
    {
        [[nodiscard]] std::size_t findMatching(
                const std::string& inCode,
                const std::size_t startPos)
        {
            if (inCode[startPos] != '[' && inCode[startPos] != ']')
            {
                std::cout<<inCode<<std::endl;
                throw std::invalid_argument("inCode[startPos] must be '[' or ']'.");
            }
            const char targetChar = inCode[startPos] == '[' ? ']' : '[';
            const std::ptrdiff_t searchDir = inCode[startPos] == '[' ? 1 : -1;

            std::ptrdiff_t curCount = 0;
            std::size_t curPos = startPos+searchDir;
            while (curPos < inCode.size())
            {
                if (inCode[curPos] == targetChar && curCount == 0)
                {
                    break;
                }
                else if (inCode[curPos] == '[')
                {
                    ++curCount;
                }
                else if (inCode[curPos] == ']')
                {
                    --curCount;
                }
                curPos += searchDir;
            }
            if (curPos >= inCode.size())
            {
                curPos = std::string::npos;
            }
            return curPos;
        }
    }
    template<typename MemoryType, typename InputStream, typename OutputStream>
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
            [[nodiscard]] constexpr std::size_t executeSingleCommand(
                    const std::string& inCode,
                    const std::size_t inCodePosition)
            {
                std::size_t charNum = inCodePosition;
                switch (inCode[charNum])
                {
                    case '>':
                        ++this->curPosition;
                        ++charNum;
                        break;
                    case '<':
                        --this->curPosition;
                        ++charNum;
                        break;
                    case '-':
                        decreaseValue(this->memory, this->curPosition);
                        ++charNum;
                        break;
                    case '+':
                        increaseValue(this->memory, this->curPosition);
                        ++charNum;
                        break;
                    case '.':
                        this->outputStream<<this->memory.getValue(this->curPosition);
                        ++charNum;
                        break;
                    case ',':
                        ValueType newVal;
                        this->inputStream>>newVal;
                        this->memory.setValue(this->curPosition, newVal);
                        ++charNum;
                        break;
                    case '[':
                        if (this->memory.getValue(this->curPosition) != ValueType(0))
                        {
                            ++charNum;
                        }
                        else
                        {
                            charNum = Inner::findMatching(inCode, charNum);
                            if (charNum == std::string::npos)
                            {
                                throw std::invalid_argument("Could not find matching ].");
                            }
                            ++charNum;
                        }
                        break;
                    case ']':
                        charNum = Inner::findMatching(inCode, charNum);
                        if (charNum == std::string::npos)
                        {
                            throw std::invalid_argument("Could not find matching [.");
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
            constexpr void execute(const std::string& inCode)
            {
                std::size_t charNum = 0;
                while (charNum < inCode.size())
                {
                    charNum = this->executeSingleCommand(inCode, charNum);
                }
            }
            template <typename ShowDebugData>
            constexpr void execute(
                    const std::string& inCode,
                    ShowDebugData& showDebugData)
            {
                std::size_t charNum = 0;
                while (charNum < inCode.size())
                {
                    showDebugData(inCode, charNum, *this);
                    charNum = this->executeSingleCommand(inCode, charNum);
                }
            }
    };
}

#endif // BFMACHINE_HPP_INCLUDED
