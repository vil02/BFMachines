%% Hello World! example

% this file illustrates how to use the BFMachinePac.BFMachine
% to execute bf code which:
% - uses the memory with character type cells (with 256 values),
% - uses console as an output (with printing values as characters)

% due to the fact, that in MATLAB
assert(char(0) == char(-1) && char(2^16) == char(2^16+1))
% we will use the class BFMachinePac.CyclicUnsignedValue
% to store the values in the memory

BF_MACHINE = BFMachinePac.BFMachine(...
    'memory', BFMachinePac.MemoryTypes.VectorMemory(...
        'default_value', BFMachinePac.CyclicUnsignedValue(0), ...
        'cast_to_value_type', @(s) BFMachinePac.CyclicUnsignedValue(s)));

IO_STREAM = ...
    BFMachinePac.IOStream('write_fun', @(s) fprintf('%s', char(s.value)));

HELLO_WORLD_BF = ...
    ['++++++++[>++++[>++>+++>+++>+<<<<-]>+>+>->>+[<]<-]>>.>' ...
     '---.+++++++..+++.>>.<-.<.+++.------.--------.>>+.>++.'];
BF_MACHINE.execute(HELLO_WORLD_BF, IO_STREAM, IO_STREAM);
