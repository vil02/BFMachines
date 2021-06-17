classdef StaticAnalysisTest < matlab.unittest.TestCase
     methods (Test)
         function test_with_checkcode(testCase)
             dir_data = testCase.get_all_matlab_files();
             arrayfun(@check_single, dir_data);
             function check_single(in_dir)
                 testCase.assertGreaterThan(in_dir.bytes, 0);
                 checkcode_res =  checkcode(to_file_path(in_dir));
                 if isequal(in_dir.name, 'run_cpp_bf.m')
                     testCase.assertEqual(length(checkcode_res), 1);
                     testCase.assertEqual(checkcode_res.message, ...
                         ['Using CLEAR FUNCTIONS usually decreases ' ...
                          'code performance and is often unnecessary.']);
                 else
                     testCase.assertEmpty(checkcode_res);
                 end
             end
         end
         function test_white_spaces(testCase)
             are_all_good = true;
             arrayfun(@proc_single_file, testCase.get_all_matlab_files());
             testCase.assertTrue(are_all_good);
             function proc_single_file(in_dir_data)
                 cur_file_path = to_file_path(in_dir_data);
                 line_list = read_to_lines(cur_file_path);
                 if ~all(cellfun(@is_line_good, line_list))
                     are_all_good = false;
                     fprintf('trailing space in: %s\n', cur_file_path);
                 end
                 if ~isempty(line_list{end})
                     are_all_good = false;
                     fprintf('no blank line at the end of: %s\n', cur_file_path);
                 end
                 function res = is_line_good(line_str)
                     res = ~(length(line_str) >= 1 && line_str(end) == ' ');
                 end
                 function res = read_to_lines(in_file_path)
                     res = cellfun(...
                         @proc_single_line, ...
                         strsplit(fileread(in_file_path), newline), ...
                         'UniformOutput', false);
                     function res = proc_single_line(in_line)
                         res = in_line;
                         if length(res) > 1 && res(end) == sprintf('\r')
                             res = res(1:end-1);
                         end
                     end
                 end
             end
         end         
     end
     methods (Access = private)
         function dir_data = get_all_matlab_files(testCase)
             dir_data = dir(['**', filesep, '*.m']);
             testCase.assertNotEmpty(dir_data);
         end
     end
end
 
function res = to_file_path(in_dir_data)
    res = [in_dir_data.folder, filesep, in_dir_data.name];
end

