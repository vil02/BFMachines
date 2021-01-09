classdef BFMachine < handle
    properties (GetAccess = public, SetAccess = private)
        memory;
        cur_position;
        instruction_set;
    end

    methods
        function obj = BFMachine(varargin)
            input_parser = inputParser;
            input_parser.addParameter(...
                'memory', ...
                BFMachinePac.MemoryTypes.VectorMemory());
            input_parser.addParameter(...
                'instruction_set', ...
                BFMachinePac.get_standard_bf_instruction_set())
            input_parser.parse(varargin{:});
            obj.memory = input_parser.Results.memory;
            obj.cur_position = obj.memory.get_starting_position();
            obj.instruction_set = input_parser.Results.instruction_set;
        end
        function execute(obj, in_code, input_stream, output_stream)
            cur_code_pos = 1;
            find_matching = ...
                BFMachinePac.FindMatching(...
                    in_code, ...
                    obj.instruction_set.begin_loop, ...
                    obj.instruction_set.end_loop);
            while cur_code_pos <= length(in_code)
                cur_code_pos = ...
                    obj.execute_single(...
                        in_code, ...
                        cur_code_pos, ...
                        find_matching, ...
                        input_stream, ...
                        output_stream);
            end
        end
    end

    methods (Access = private)
        function new_code_pos = execute_single(...
            obj, in_code, in_code_pos, ...
            find_matching, ...
            input_stream, output_stream)
            cur_instruction = in_code(in_code_pos);
            switch cur_instruction
                case obj.instruction_set.move_left
                    obj.cur_position = obj.cur_position-1;
                    new_code_pos = in_code_pos+1;
                case obj.instruction_set.move_right
                    obj.cur_position = obj.cur_position+1;
                    new_code_pos = in_code_pos+1;
                case obj.instruction_set.decrease_value
                    BFMachinePac.MemoryTypes.change_value(...
                        obj.memory, obj.cur_position, -1);
                    new_code_pos = in_code_pos+1;
                case obj.instruction_set.increase_value
                    BFMachinePac.MemoryTypes.change_value(...
                        obj.memory, obj.cur_position, 1);
                    new_code_pos = in_code_pos+1;
                case obj.instruction_set.read_value
                    obj.memory.set_value(obj.cur_position, input_stream.get());
                    new_code_pos = in_code_pos+1;
                case obj.instruction_set.print_value
                    output_stream.put(...
                        obj.memory.get_value(obj.cur_position));
                    new_code_pos = in_code_pos+1;
                case obj.instruction_set.begin_loop
                    if obj.memory.get_value(obj.cur_position) ~= 0
                        new_code_pos = in_code_pos+1;
                    else
                        new_code_pos = find_matching.find(in_code_pos)+1;
                    end
                case obj.instruction_set.end_loop
                    if obj.memory.get_value(obj.cur_position) ~= 0
                        new_code_pos = find_matching.find(in_code_pos)+1;
                    else
                        new_code_pos = in_code_pos+1;
                    end
                otherwise
                    new_code_pos = in_code_pos+1;
            end
        end
    end
end
