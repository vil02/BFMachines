# BFMachines
[BF](https://esolangs.org/wiki/Brainfuck) interpreters in various languages

# General idea
The purpose of this repository is to provide adjustable, reliable and "high performance" [brainfuck](https://esolangs.org/wiki/Brainfuck) interpreters.

# Available languages and implemented features

* [C++](cpp)
    * interpreter and debugger, code optimisation, translators
    * [![cpp_build_ubuntu](https://github.com/vil02/BFMachines/actions/workflows/cpp_build_ubuntu.yml/badge.svg)](https://github.com/vil02/BFMachines/actions/workflows/cpp_build_ubuntu.yml) [![cpp_build_windows_latest](https://github.com/vil02/BFMachines/actions/workflows/cpp_build_windows_latest.yml/badge.svg)](https://github.com/vil02/BFMachines/actions/workflows/cpp_build_windows_latest.yml) [![cpp_build_macos](https://github.com/vil02/BFMachines/actions/workflows/cpp_build_macos.yml/badge.svg)](https://github.com/vil02/BFMachines/actions/workflows/cpp_build_macos.yml)
    * [![cpp_static_analysis](https://github.com/vil02/BFMachines/actions/workflows/cpp_static_analysis.yml/badge.svg)](https://github.com/vil02/BFMachines/actions/workflows/cpp_static_analysis.yml) [![cpp_address_sanitizer](https://github.com/vil02/BFMachines/actions/workflows/cpp_address_sanitizer.yml/badge.svg)](https://github.com/vil02/BFMachines/actions/workflows/cpp_address_sanitizer.yml)
* [MATLAB](matlab) 
    * interpreter, also shown how to use the C++ [BFMachineLib](cpp/BFMachineLib) from MATLAB (cf. [`call_cpp_bf` example ](matlab/examples/call_cpp_bf/run_cpp_bf.m))

# Future languages

* python
* ?
