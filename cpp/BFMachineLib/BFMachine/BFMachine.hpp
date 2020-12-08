#ifndef BFMACHINE_HPP_INCLUDED
#define BFMACHINE_HPP_INCLUDED

#include <string>
#include <exception>

namespace BFM
{
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
        public:
            BFMachine(InputStream& _inputStream, OutputStream& _outputStream) :
                memory(),
                curPosition(memory.getStartingPosition()),
                inputStream(_inputStream),
                outputStream(_outputStream)
            {}
            void execute(const std::string& inCode)
            {
                std::size_t charNum = 0;
                while (charNum < inCode.size())
                {
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
                            this->memory.decreaseValue(this->curPosition);
                            ++charNum;
                            break;
                        case '+':
                            this->memory.increaseValue(this->curPosition);
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
                                charNum = inCode.find(']', charNum);
                                if (charNum == std::string::npos)
                                {
                                    throw std::invalid_argument("Could not find matching ].");
                                }
                                ++charNum;
                            }
                            break;
                        case ']':
                            charNum = inCode.rfind('[', charNum);
                            if (charNum == std::string::npos)
                            {
                                throw std::invalid_argument("Could not find matching [.");
                            }
                            break;
                        default:
                            break;
                    }
                }
            }
    };
}

#endif // BFMACHINE_HPP_INCLUDED
