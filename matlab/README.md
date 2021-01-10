# [BF](https://esolangs.org/wiki/Brainfuck) interpreter in MATLAB

`BFMachinePac` provides the basic functionality to execute brainfuck code, namely:
* the class [`BFMachinePac::BFMachine`](%2BBFMachinePac/%40BFMachine/BFMachine.m) having the method `execute`, which interprets given bf code.
* [_memory models_](%2BBFMachinePac/%2BMemoryTypes), which are useful for brainf*ck. The basic examples are:
    * unbounded memory: 
        * [`BFMachinePac.MemoryTypes.VectorMemory`](%2BBFMachinePac/%2BMemoryTypes/%40VectorMemory/VectorMemory.m), where data is stored in two arrays (first for the indexes \(1, 2, 3, \ldots\) and second for the the indexes \(0, -1, -2, ... \ldots\)). The way how these arrays are extended can we specified while creating the `VectorMemory` object.
        * [`BFMachinePac.MemoryTypes.MapMemory`](%2BBFMachinePac/%2BMemoryTypes/%40VectorMemory/MapMemory.m), where data is stored in `containers.Map`,
* input/output streams types. Examples are:
    * [`BFMachinePac.InputStream`](%2BBFMachinePac/%40InputStream/InputStream.m), where data is being send from some specified array,
    * [`BFMachinePac.OutputStream`](%2BBFMachinePac/%40OutputStream/OutputStream.m), where data is being send to some cell array,
    * [`BFMachinePac.InputStream`](%2BBFMachinePac/%40IOStream/IOStream.m), where user can specify the read and write functions (useful while using the console),

The folder [examples](examples) contains some code snippets, which might be useful to look at the beginning. Start with [**add_bf**](matlab/examples/add_bf.m), [**hello_world_bf**](matlab/examples/hello_world_bf.m).

The example [call_cpp_bf](examples/call_cpp_bf) shows how to use the [C++ library](../cpp/BFMachineLib) from MATLAB.

Before running any of the examples, please make sure that you are in the [matlab](.) folder and execute the script [add_examples.m](add_examples.m). 

# Requirements, dependencies, ...

The code is written and tested in MATLAB R2018b
