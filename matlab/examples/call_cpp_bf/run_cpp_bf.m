% this file illustrates how to use the C++ BFMachineLib from MATLAB
% run this script with current MATLAB folder/pwd being
% .../BFMachines/matlab
% after running the script add_examples.m

%% make sure that we are in the right place
assert(endsWith(pwd, 'matlab'));
assert(...
    exist(strrep('../cpp/BFMachineLib/BFMachineLib.hpp', '/', filesep), ...
          'file') == 2);

%% prepare mex-file
mex CXXFLAGS="$CXXFLAGS -std=c++17" ...
    examples/call_cpp_bf/cpp_bf.cpp ...
    -outdir ./examples/call_cpp_bf

%% displayig Hello World!
HELLO_BF = ...
    ['++++++++[>++++[>++>+++>+++>+<<<<-]>+>+>->>+[<]<-]>>.>' ...
     '---.+++++++..+++.>>.<-.<.+++.------.--------.>>+.>++.'];
fprintf('%s', cpp_bf(HELLO_BF, char.empty()));

%% adding two numbers using brainf*ck
PLUS_BF = ',>,<[->+<]>.';
INPUT_DATA = [uint64(23), uint64(10)];
OUTPUT_DATA = cpp_bf(PLUS_BF, INPUT_DATA);
assert(OUTPUT_DATA == sum(INPUT_DATA));

%% in order to avoid problem with deleting the mex-file use the line below
clear functions
