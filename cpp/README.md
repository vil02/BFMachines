[![cpp_build_ubuntu](https://github.com/vil02/BFMachines/actions/workflows/cpp_build_ubuntu.yml/badge.svg)](https://github.com/vil02/BFMachines/actions/workflows/cpp_build_ubuntu.yml)
[![cpp_build_windows](https://github.com/vil02/BFMachines/actions/workflows/cpp_build_windows.yml/badge.svg)](https://github.com/vil02/BFMachines/actions/workflows/cpp_build_windows.yml)
[![cpp_build_macos](https://github.com/vil02/BFMachines/actions/workflows/cpp_build_macos.yml/badge.svg)](https://github.com/vil02/BFMachines/actions/workflows/cpp_build_macos.yml) \
[![cpp_static_analysis](https://github.com/vil02/BFMachines/actions/workflows/cpp_static_analysis.yml/badge.svg)](https://github.com/vil02/BFMachines/actions/workflows/cpp_static_analysis.yml)
[![cpp_address_sanitizer](https://github.com/vil02/BFMachines/actions/workflows/cpp_address_sanitizer.yml/badge.svg)](https://github.com/vil02/BFMachines/actions/workflows/cpp_address_sanitizer.yml)
[![Quality Gate Status](https://sonarcloud.io/api/project_badges/measure?project=vil02_BFMachines&metric=alert_status)](https://sonarcloud.io/summary/new_code?id=vil02_BFMachines)
# [BF](https://esolangs.org/wiki/Brainfuck) interpreter in C++

`BFMachineLib` provides the basic functionality to execute brainfuck code, namely:
* the class template [`bfm::BFMachine`](BFMachineLib/BFMachine/BFMachine.hpp) having
    * the overloaded method `execute`, which interprets given bf code,
    * the method `execute_optimized`, which parses/optimizes the given bf code into a _sequence_ of equivalent operations and them executes them,
    * the method `execute_seq`, which executes already parsed bf code.

    The `MemoryType` and `Input/OutputStreamTypes` are some of the template arguments
* [_memory models_](BFMachineLib/MemoryTypes), which are useful for brainf*ck. The basic examples are:
    * unbounded memory:
        * [`bfm::memory_types::VectorMemory`](BFMachineLib/MemoryTypes/VectorMemory.hpp), where data is stored in two `std::vector`-like containers (first for the _positive_ and second for the _negative direction_),
        * [`bfm::memory_types::MapMemory`](BFMachineLib/MemoryTypes/MapMemory.hpp), where data is stored in `std::map` or `std::unordered_map`-like containers.
    * bounded memory:
        * [`bfm::memory_types::ArrayMemory`](BFMachineLib/MemoryTypes/ArrayMemory.hpp), where data is stored in `std::array`-like container. Also useful for [compile time computations](examples/compile_time_arithmetic/compile_time_arithmetic.cpp).
* [input/output streams types](BFMachineLib/Streams), which can be used for testing or in any other case, when one does not want to use `std::cin` or `std::cout`. Examples are:
    * [`bfm::streams::InputStream`](BFMachineLib/Streams/InputStream.hpp), where data is being send from some `std::vector` or `std::array`-like container,
    * [`bfm::streams::OutputVectorStream`](BFMachineLib/Streams/OutputVectorStream.hpp), where data is being send to some `std::vector`-like container (unknown size at compile time),
    * [`bfm::streams::OutputArrayStream`](BFMachineLib/Streams/OutputArrayStream.hpp), where data is being send to some `std::array`-like container (known size at compile time),
* [BF operations](BFMachineLib/BFOperations/BFOperations.hpp), which represent optimised pieces of BF code.
    * `bfm::bfo::BFWrite`,
    * `bfm::bfo::BFRead`,
    * `bfm::bfo::BFBlock`, which corresponds to any block of _simple commands_, i.e. commands like `<`, `>`, `+`, `-`,
    * `bfm::bfo::BFSimpleLoop`, which corresponds to any bf loop containing only _simple commands_, such that the _total shift_ (i.e. the number of `>` minus the number of `<`) is zero and the value of the memory cell, which is checked at the beginning of the loop decreases by one. Examples are `[->+<]`, `[->++>--->+++<<<]` or `[<++>>--<-]`,
    * `bfm::bfo::BFLoop`, which represents any BF loop.

    The function [`bfm::parser::bf_code_to_operation_seq`](BFMachineLib/BFParser/BFCodeToOperationSeq.hpp) _translates_ the given BF code into a _sequence_ of the operations described above. The result is represented as a `std::vector<std::variant<BFRead, BFWrite, BFBlock, BFSimpleLoop, BFLoop > >`. The example ['bf_to_cpp'](examples/bf_to_cpp/bf_to_cpp.cpp) can be used to ilustrate which BF code is _translated_ into which _sequence_. By default, the container storing the memory change information is `std::map`. Other types (for example `std::unordered_map`) can be specified by using the function template [`bfm::parser::general_parser::bf_code_to_operation_seq`](BFMachineLib/BFParser/BFCodeToOperationSeq.hpp).
    A _sequence_ of [BF operations](BFMachineLib/BFOperations/BFOperations.hpp) can be translated to other [programming languages](BFMachineLib/BFOperations/translators).

The folder [examples](examples) contains some code snippets, which might be useful to look at the beginning. Start with [**simple_bf_interpreter**](examples/simple_bf_interpreter/simple_bf_interpreter.cpp), [performance_test](examples/memory_types_performance_comparison/memory_types_performance_comparison.cpp) and [simple_bf_debugger](examples/simple_bf_debugger/simple_bf_debugger.cpp).

# Requirements, dependencies, ...

The code is basically written in the spirit of C++17 ([example `compile_time_arithmetic`](examples/compile_time_arithmetic/compile_time_arithmetic.cpp) requires C++20).

[Tests](tests) use Boost.Test.
