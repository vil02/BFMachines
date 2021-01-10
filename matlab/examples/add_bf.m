% in this example we run bf code adding two (non-negative) values
% different ways to handle input and output are presented

ADD_BF = ',>,<[->+<]>.';
%% using console as a input and output

IO_STREAM = BFMachinePac.IOStream(...
    'read_fun', @() input_unsigned('Enter an unsigned value: '), ...
    'write_fun', @(s) fprintf('Result: %d\n', s));

BFMachinePac.BFMachine().execute(ADD_BF, IO_STREAM, IO_STREAM);

%% using BFMAchinePac.InputStream and BFMAchinePac.OutputStream

VAL_A = 34;
VAL_B = 17;
INPUT_STREAM = BFMachinePac.InputStream([VAL_A, VAL_B]);
OUTPUT_STREAM = BFMachinePac.OutputStream();
BFMachinePac.BFMachine().execute(ADD_BF, INPUT_STREAM, OUTPUT_STREAM);

RESULT_DATA = OUTPUT_STREAM.get_data();
% we expect one value in the output ...
assert(isequal(size(RESULT_DATA), [1, 1]));
% ... with is equal to VAL_A+VAL_B
assert(RESULT_DATA{1} == VAL_A+VAL_B);

function res = input_unsigned(in_prompt)
    res = input(in_prompt);
    assert(res >= 0);
end
