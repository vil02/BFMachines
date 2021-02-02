classdef MemoryTypesTest < matlab.unittest.TestCase
     methods (Test)
         function test_get_value(testCase)
             cellfun(@test_single, testCase.get_memory_constructor_list());
             function test_single(in_memory_constructor)
                 memory = in_memory_constructor();
                 for cur_pos = -10:1:10
                     testCase.assertEqual(...
                         memory.get_value(cur_pos), ...
                         memory.cast_to_value_type(0));
                 end
             end
         end
         function test_set_get_value(testCase)
             rng(1);
             cellfun(...
                 @test_single_memory, ...
                 testCase.get_memory_constructor_list());
             function test_single_memory(in_memory_constructor)
                 memory = in_memory_constructor();
                 min_pos = -200;
                 max_pos = 200;
                 test_data = populate_memory(memory, min_pos, max_pos, 200);
                 arrayfun(...
                     @(s) check_single_position(testCase, s, memory, test_data), ...
                     min_pos:1:max_pos);
             end
         end
         function test_change_value(testCase)
             rng(2);
             cellfun(...
                 @test_single_memory, ...
                 testCase.get_memory_constructor_list());
             function test_single_memory(in_memory_constructor)
                 memory = in_memory_constructor();
                 min_pos = -20;
                 max_pos = -min_pos;
                 test_data = populate_memory(memory, min_pos, max_pos, 30);
                 cellfun(@proc_single_position, test_data.keys());
                 arrayfun(...
                     @(s) check_single_position(testCase, s, memory, test_data), ...
                     min_pos:1:max_pos);
                 function proc_single_position(in_position)
                     for change_num = 1:1:20
                         value_change = randi([-10, 10]);
                         BFMachinePac.MemoryTypes.change_value(...
                             memory, in_position, value_change);
                         test_data(in_position) = ...
                             test_data(in_position)+value_change;
                         if ~isequal(class(test_data(in_position)), ...
                                 memory.get_value_type())
                             test_data(in_position) = ...
                                 memory.cast_to_value_type(test_data(in_position));
                         end
                         check_single_position(...
                             testCase, in_position, memory, test_data);
                     end
                 end
             end
         end
     end

     methods
         function res = get_memory_constructor_list(~)
             res = ...
                 {@() BFMachinePac.MemoryTypes.VectorMemory(), ...
                  @() BFMachinePac.MemoryTypes.VectorMemory('default_value', int8(0)), ...
                  @() BFMachinePac.MemoryTypes.VectorMemory('default_value', char(0)), ...
                  @() BFMachinePac.MemoryTypes.VectorMemory(...
                      'default_value', BFMachinePac.CyclicUnsignedValue(0), ...
                      'cast_to_value_type', @(s) BFMachinePac.CyclicUnsignedValue(s)), ...
                  @() BFMachinePac.MemoryTypes.VectorMemory(...
                      'extend_data_fun', ...
                      @BFMachinePac.MemoryTypes.extend_by_doubling), ...
                  @() BFMachinePac.MemoryTypes.MapMemory(), ...
                  @() BFMachinePac.MemoryTypes.MapMemory(...
                      'remove_default_values', false), ...
                  @() BFMachinePac.MemoryTypes.MapMemory(...
                      'position_type', 'double', ...
                      'default_value', char(0), ...
                      'remove_default_values', true), ...
                  @() BFMachinePac.MemoryTypes.MapMemory(...
                      'position_type', 'double', ...
                      'default_value', char(0), ...
                      'remove_default_values', false), ...
                  @() BFMachinePac.MemoryTypes.MapMemory(...
                      'default_value', BFMachinePac.CyclicUnsignedValue(0), ...
                      'cast_to_value_type', @(s) BFMachinePac.CyclicUnsignedValue(s)), ...
                 };
         end
         function check_single_position(...
                     testCase, in_position, in_memory, in_test_data)
                 if in_test_data.isKey(in_position)
                     testCase.assertEqual(...
                         in_memory.get_value(in_position), ...
                         in_test_data(in_position));
                 else
                     testCase.assertEqual(...
                         in_memory.get_value(in_position), ...
                         in_memory.default_value);
                 end
                 testCase.assertTrue(...
                     isa(in_memory.get_value(in_position), ...
                         in_memory.get_value_type()));
         end
     end
end

 function res = populate_memory(...
         memory, in_min_pos, in_max_pos, in_size)
     res = ...
         containers.Map('KeyType', ...
                        memory.get_position_type(), ...
                        'ValueType', ...
                        'any');
     for i = 1:1:in_size
         cur_position = ...
             cast(randi([in_min_pos, in_max_pos]), ...
             memory.get_position_type());
         cur_value = memory.cast_to_value_type(randi(200));
         memory.set_value(cur_position, cur_value);
         res(cur_position) = cur_value;
     end
 end
