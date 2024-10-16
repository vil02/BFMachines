//this example shows how to debug bf code
//using the method BFMachine::execute(const CodeType&, ShowDebugData&)
#include "../../BFMachineLib/BFMachineLib.hpp"

#include <algorithm>
#include <iostream>
#include <limits>
#include <string>
#include <unordered_map>
#include <exception>

//we define a class displaying the current state of the BFMachine and current code
//objects of this class are used as the second argument of the method
//BFMachine::execute(const CodeType&, ShowDebugData&)
template <typename BFMachine>
class [[nodiscard]] ConsoleDebugViewer
{
    public:
        using position_type = typename BFMachine::position_type;
    private:
        position_type min_pos = std::numeric_limits<position_type>::max();
        position_type max_pos = std::numeric_limits<position_type>::min();
    public:
        ConsoleDebugViewer() = default;
        void operator()(
                const std::string& in_code,
                const std::size_t in_code_position,
                const BFMachine& in_bf_machine)
        {
            this->min_pos = std::min(this->min_pos, in_bf_machine.get_memory_position());
            this->max_pos = std::max(this->max_pos, in_bf_machine.get_memory_position());
            std::cout<<"bf_bode:\n"<<in_code<<'\n'
                     <<std::string(in_code_position, ' ')
                     <<"^ in_code_pos: "<<in_code_position<<'\n';
            std::cout<<"mem_pos:\n";
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
            std::cout<<'\n';
            std::cout<<"mem_val:\n";
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
            std::cout<<'\n';
            switch (in_code[in_code_position])
            {
                case '.':
                    std::cout<<"Printing value: ";
                    break;
                case ',':
                    std::cout<<"Enter value: ";
                    break;
                default:
                    std::cout<<"Press enter to continue...";
                    std::cin.ignore();
            }
        }
};

int main()
{
    int return_value = 0;
    using memory_type = typename bfm::memory_types::MapMemory<std::unordered_map<int, unsigned> >;
    using bfm_type = typename bfm::BFMachine<memory_type, decltype(std::cin), decltype(std::cout)>;
    auto debug_viewer = ConsoleDebugViewer<bfm_type>();
    //example bf code adding two numbers
    const std::string bf_code = ",>,<[->+<]>.";
    try
    {
        bfm_type(std::cin, std::cout).execute(bf_code, debug_viewer);
    }
    catch (const std::exception& ex)
    {
        std::cout<<"Error: "<<ex.what()<<'\n';
        return_value = 1;
    }
    catch (...)
    {
        std::cout<<"Unknown error\n";
        return_value = 2;
    }
    return return_value;
}
