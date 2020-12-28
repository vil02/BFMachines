# [BF](https://esolangs.org/wiki/Brainfuck) interpreter in C++

`BFMachineLib` provides the basic functionality to execute brainf*ck code, namely:
* the class template [`bfm::BFMachine`](BFMachineLib/BFMachine/BFMachine.hpp) having the overloaded method `execute`, which interprets given bf code. The `MemoryType` and `Input/OutputStreamTypes` are some of the template arguments.
* [_memory models_](BFMachineLib/MemoryTypes), which are useful for brainf*ck. The basic examples are:
    * unbounded memory: 
        * [`bfm::memory_types::VectorMemory`](BFMachineLib/MemoryTypes/VectorMemory.hpp), where data is stored in two `std::vector`-like containers (first for the _positive_ and second for the _negative direction_),
        * [`bfm::memory_types::MapMemory`](BFMachineLib/MemoryTypes/MapMemory.hpp), where data is stored in `std::map` or `std::unordered_map`-like containers.
    * bounded memory:
        * [`bfm::memory_types::ArrayMemory`](BFMachineLib/MemoryTypes/ArrayMemory.hpp), where data is stored in `std::array`-like container. Also useful for compile time computations.
* [input/output streams types](BFMachineLib/Streams), which can be used for testing or in any other case, when one does not want to use `std::cin` or `std::cout`. Exampels are:
    * [`bfm::streams::InputStream`](BFMachineLib/Streams/InputStream.hpp), where data is being send from some `std::vector` or `std::array`-like container,
    * [`bfm::streams::OutputVectorStream`](BFMachineLib/Streams/OutputVectorStream.hpp), where data is being send to some `std::vector`-like container (unknown size at compile time),
    * [`bfm::streams::OutputArrayStream`](BFMachineLib/Streams/OutputArrayStream.hpp), where data is being send to some `std::array`-like container (known size at compile time),

The folder [examples](examples) contains some code snippets, which might be useful to look at the beginning. Start with [**simple_bf_interpreter**](cpp/examples/simple_bf_interpreter/simple_bf_interpreter.cpp), [performance_test](examples/memory_types_performance_comparison/memory_types_performance_comparison.cpp) and [simple_bf_debugger](examples/simple_bf_debugger/simple_bf_debugger.cpp)

# Requirements, dependencies, ...

The code is basically written in the spirit of C++17. I am using GNU GCC Compiler 9.2.
[Tests](tests) use Boost.Test. I am using Boost 1.72.
