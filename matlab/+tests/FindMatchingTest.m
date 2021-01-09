classdef FindMatchingTest < matlab.unittest.TestCase
    methods (Test)
        function test_basic_a(testCase)
            find_matching = ...
                BFMachinePac.FindMatching('12[456789]12[]567', '[', ']');
            testCase.assertEqual(find_matching.find(3), 10);
            testCase.assertEqual(find_matching.find(3), 10);
            testCase.assertEqual(find_matching.find(10), 3);
            testCase.assertEqual(find_matching.find(14), 13);
            testCase.assertEqual(find_matching.find(13), 14);
        end
        function test_basic_b(testCase)
            find_matching = ...
                BFMachinePac.FindMatching('12[45[7]]0', '[', ']');
            testCase.assertEqual(find_matching.find(3), 9);
            testCase.assertEqual(find_matching.find(8), 6);
            testCase.assertEqual(find_matching.find(9), 3);
            testCase.assertEqual(find_matching.find(6), 8);
        end
        function test_reference_behaviour(testCase)
            find_matching = ...
                BFMachinePac.FindMatching('12[45[7]]0', '[', ']');
            inner_fun(find_matching);
            testCase.assertEqual(find_matching.cache_data(3), 9);
            testCase.assertEqual(find_matching.cache_data(8), 6);
            testCase.assertEqual(find_matching.cache_data(9), 3);
            testCase.assertEqual(find_matching.cache_data(6), 8);
            function inner_fun(find_matching_obj)
                testCase.assertEqual(find_matching_obj.find(3), 9);
                testCase.assertEqual(find_matching_obj.find(8), 6);
                testCase.assertEqual(find_matching_obj.find(9), 3);
                testCase.assertEqual(find_matching_obj.find(6), 8);
            end
        end
    end
end
