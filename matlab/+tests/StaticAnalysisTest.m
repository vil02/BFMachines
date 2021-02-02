classdef StaticAnalysisTest < matlab.unittest.TestCase
     methods (Test)
         function test_with_checkcode(testCase)
             dir_data = dir(['**', filesep, '*.m']);
             testCase.assertFalse(isempty(dir_data));
             arrayfun(@check_single, dir_data);
             function check_single(in_dir)
                 testCase.assertGreaterThan(in_dir.bytes, 0);
                 checkcode_res = ...
                     checkcode([in_dir.folder, filesep, in_dir.name]);
                 if ~isequal(in_dir.name, 'run_cpp_bf.m')
                     testCase.assertEmpty(checkcode_res);
                 else
                     testCase.assertEqual(length(checkcode_res), 1);
                     testCase.assertEqual(checkcode_res.message, ...
                         'Using CLEAR FUNCTIONS usually decreases code performance and is often unnecessary.')
                 end
             end
         end
     end
 end
