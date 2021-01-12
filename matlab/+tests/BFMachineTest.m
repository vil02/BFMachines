classdef BFMachineTest < matlab.unittest.TestCase
    methods (Test)
        function test_plus(testCase)
            cellfun(@check_single, testCase.get_bfm_constructor_list());
            function check_single(in_bfm_constructor)
                sum_test_params.bfm_constructor = in_bfm_constructor;
                sum_test_params.bf_code = ',>,<[->+<]>.';
                sum_test_params.result_fun = @sum;
                sum_test_params.max_value = 10;
                sum_test_params.random_input_length = 2;
                sum_test_params.random_max_value = 100;
                sum_test_params.number_of_trials = 10;
                testCase.check_computation_2d(sum_test_params);
            end
        end

        function test_times(testCase)
            cellfun(@check_single, testCase.get_bfm_constructor_list());
            function check_single(in_bfm_constructor)
                prod_test_params.bfm_constructor = in_bfm_constructor;
                prod_test_params.bf_code = ...
                    ',>,<[>[->+>+<<]>>[-<<+>>]<[->>+<<]<<-]>>>>.';
                prod_test_params.result_fun = @prod;
                prod_test_params.max_value = 10;
                prod_test_params.random_input_length = 2;
                prod_test_params.random_max_value = 23;
                prod_test_params.number_of_trials = 10;
                testCase.check_computation_2d(prod_test_params);
            end
        end

        function test_factorial(testCase)
            cellfun(@check_single, testCase.get_bfm_constructor_list());
            function check_single(in_bfm_constructor)
                fac_test_params.bfm_constructor = in_bfm_constructor;
                fac_test_params.bf_code = ...
                    ['+>>,[>>]<<' ...
                     '[[->+>+<<]>>[-<<+>>]<-]<<' ...
                     '[[>[->+>+<<]>>[-<<+>>]<[->>+<<]<<-]>>>>[-<<<<+>>>>]<<<<>[-]<<]>.'];
                fac_test_params.result_fun = @factorial;
                for cur_n = 0:7
                    testCase.check_single_computation(...
                        cur_n, fac_test_params);
                end
            end
        end

        function test_hello_world(testCase)
            cellfun(...
                @check_single, testCase.get_char_bfm_constructor_list());
            function check_single(in_char_bfm_construction)
                hello_bf = ...
                   ['++++++++[>++++[>++>+++>+++>+<<<<-]>+>+>->>+[<]<-]>>.>' ...
                    '---.+++++++..+++.>>.<-.<.+++.------.--------.>>+.>++.'];
                bfm = in_char_bfm_construction();
                input_stream = BFMachinePac.InputStream([]);
                output_stream = BFMachinePac.OutputStream();
                bfm.execute(hello_bf, input_stream, output_stream);
                res_str = cellfun(@(s) char(s.value), output_stream.get_data());
                testCase.assertEqual(...
                    res_str, sprintf('%s\n', 'Hello World!'));
            end
        end
    end

    methods
        function res = get_bfm_constructor_list(~)
            res = ...
                {@() BFMachinePac.BFMachine(), ...
                 @() BFMachinePac.BFMachine(...
                     'memory', BFMachinePac.MemoryTypes.VectorMemory(...
                         'extend_data_fun', ...
                         @BFMachinePac.MemoryTypes.extend_by_doubling)), ...
                 @() BFMachinePac.BFMachine(...
                     'memory', BFMachinePac.MemoryTypes.MapMemory()), ...
                 @() BFMachinePac.BFMachine(...
                     'memory', BFMachinePac.MemoryTypes.MapMemory(...
                         'remove_default_values', false)), ...
                };
        end
        function res = get_char_bfm_constructor_list(~)
            res = ...
                {@() BFMachinePac.BFMachine(...
                    'memory', BFMachinePac.MemoryTypes.VectorMemory(...
                        'default_value', BFMachinePac.CyclicUnsignedValue(0), ...
                        'cast_to_value_type', @(s) BFMachinePac.CyclicUnsignedValue(s))), ...
                 @() BFMachinePac.BFMachine(...
                    'memory', BFMachinePac.MemoryTypes.MapMemory(...
                        'default_value', BFMachinePac.CyclicUnsignedValue(0), ...
                        'cast_to_value_type', @(s) BFMachinePac.CyclicUnsignedValue(s))), ...
                };
        end
        function check_computation_2d(testCase, test_params)
            testCase.check_all_2d(test_params);
            testCase.check_for_random_input(test_params);
        end
        function check_all_2d(testCase, in_test_params)
            for val_a = 0:in_test_params.max_value
                for val_b = 0:in_test_params.max_value
                    testCase.check_single_computation(...
                        [val_a, val_b], in_test_params);
                end
            end
        end
        function check_for_random_input(testCase, in_test_params)
            for trial_num = 1:in_test_params.number_of_trials
                cur_input = ...
                    randi([0, in_test_params.random_max_value], ...
                        1, in_test_params.random_input_length);
                testCase.check_single_computation(...
                    cur_input, in_test_params);
            end
        end
        function check_single_computation(...
                testCase, input_data, test_params)
            input_stream = BFMachinePac.InputStream(input_data);
            output_stream = BFMachinePac.OutputStream();
            bfm = test_params.bfm_constructor();
            bfm.execute(test_params.bf_code, input_stream, output_stream);
            res_data = output_stream.get_data();
            testCase.assertEqual(size(res_data), [1, 1]);
            testCase.assertEqual(...
                res_data{1}, test_params.result_fun(input_data));
        end
    end
end
