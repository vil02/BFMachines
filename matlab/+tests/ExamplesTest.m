classdef ExamplesTest < matlab.unittest.TestCase
    properties
        examplesPathList = genpath('examples');
    end
    methods(TestMethodSetup)
        function addExamplesPath(testCase)
            addpath(testCase.examplesPathList);
        end
    end

    methods(TestMethodTeardown)
        function removeExamplesPath(testCase)
            rmpath(testCase.examplesPathList);
        end
    end
    methods (Test)
        function test_hello_world_bf(testCase)
            testCase.assertEqual(...
                evalc('hello_world_bf'), sprintf('Hello World!\n'))
        end

        function test_run_cpp_bf(testCase)
            search_str = '*/*/cpp_bf.mexw64';
            dir_data = dir(search_str);
            testCase.assertLessThanOrEqual(length(dir_data), 1);
            if ~isempty(dir_data)
                mex_file_path = [dir_data.folder, filesep, dir_data.name];
                delete(mex_file_path);
            end
            testCase.assertEmpty(dir('*/*/cpp_bf.mexw64'));
            res_str = evalc('run_cpp_bf');
            testCase.assertTrue(...
                contains(res_str, 'MEX completed successfully.'));
            testCase.assertTrue(...
                contains(res_str, sprintf('Hello World!\n')));
            testCase.assertEqual(length(dir(search_str)), 1);
        end
    end
 end
