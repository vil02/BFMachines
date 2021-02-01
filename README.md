# BFMachines
[BF](https://esolangs.org/wiki/Brainfuck) interpreters in various languages

# General idea
The purpose of this repository is to provide adjustable, reliable and "high performance" [brainfuck](https://esolangs.org/wiki/Brainfuck) interpreters.

# Available languages and implemented features

* [C++](cpp)
    * interpreter and debugger, code optimisation
    * ![Build ubuntu](https://github.com/vil02/BFMachines/workflows/cpp_build_ubuntu_latest/badge.svg) ![Build windows](https://github.com/vil02/BFMachines/workflows/cpp_build_windows_latest/badge.svg) ![Build macos](https://github.com/vil02/BFMachines/workflows/cpp_build_macos_latest/badge.svg)
    * ![Static analysis](https://github.com/vil02/BFMachines/workflows/cpp_static_analysis/badge.svg) ![Address sanitizer](https://github.com/vil02/BFMachines/workflows/cpp_address_sanitizer/badge.svg)
* [MATLAB](matlab) 
    * interpreter, also shown how to use the C++ [BFMachineLib](cpp/BFMachineLib) from MATLAB (cf. [`call_cpp_bf` example ](matlab/examples/call_cpp_bf/run_cpp_bf.m))

# Future languages

* python
* ?
