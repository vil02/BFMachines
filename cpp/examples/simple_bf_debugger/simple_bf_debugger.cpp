//this example shows how to debug bf code
//using the method BFMachine::execute(const CodeType&, ShowDebugData&)
#include <iostream>
#include <limits>
#include <unordered_map>
#include <string>
#include <algorithm>
#include "../../BFMachineLib/BFMachineLib.hpp"

#include <cstdlib>

//we define a class displaying the current state of the BFMachine and current code
//objects of this class are used as the second argument of the method
//BFMachine::execute(const CodeType&, ShowDebugData&)
template <typename BFMachine>
class ConsoleDebugViewer
{
    public:
        using position_type = typename BFMachine::position_type;
    private:
        position_type min_pos, max_pos;
        std::size_t cur_step{0};
    public:
        ConsoleDebugViewer() :
            min_pos(std::numeric_limits<position_type>::max()),
            max_pos(std::numeric_limits<position_type>::min())
        {}
        void operator()(
                const std::string& in_code,
                const std::size_t in_code_position,
                const BFMachine& in_bf_machine)
        {
            this->min_pos = std::min(this->min_pos, in_bf_machine.get_memory_position());
            this->max_pos = std::max(this->max_pos, in_bf_machine.get_memory_position());
            std::cout<<"bf_bode:\n"<<in_code<<std::endl
                     <<std::string(in_code_position, ' ')
                     <<"^ in_code_pos: "<<in_code_position<<std::endl;
            std::cout<<"mem_pos: "<<std::endl;
            for (position_type cur_pos = this->min_pos; cur_pos <= max_pos; ++cur_pos)
            {
                if (cur_pos == in_bf_machine.get_memory_position())
                {
                    std::cout<<'('<<cur_pos<<')';
                }
                else
                {
                    std::cout<<cur_pos;
                }
                std::cout<<'\t';
            }
            std::cout<<std::endl;
            std::cout<<"mem_val: "<<std::endl;
            for (position_type cur_pos = this->min_pos; cur_pos <= max_pos; ++cur_pos)
            {
                if (cur_pos == in_bf_machine.get_memory_position())
                {
                    std::cout<<'('<<in_bf_machine.get_memory().get_value(cur_pos)<<')';
                }
                else
                {
                    std::cout<<in_bf_machine.get_memory().get_value(cur_pos);
                }
                std::cout<<'\t';
            }
            std::cout<<std::endl;
            switch (in_code[in_code_position])
            {
                case '.':
                    std::cout<<"Printing value: ";
                    break;
                case ',':
                    std::cout<<"Enter value: ";
                    break;
                default:
                    system("pause");
            }
        }
};

int main()
{
    using memory_type = typename bfm::memory_types::MapMemory<std::unordered_map<int, unsigned> >;
    using bfm_type = typename bfm::BFMachine<memory_type, decltype(std::cin), decltype(std::cout)>;
    auto debug_viewer = ConsoleDebugViewer<bfm_type>();
    //example bf code adding two numbers
    std::string bf_code = ",>,<[->+<]>.";
    bfm_type(std::cin, std::cout).execute(bf_code, debug_viewer);
    return 0;
}
