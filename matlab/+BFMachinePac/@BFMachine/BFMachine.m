classdef BFMachine < handle
    properties
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
    end
end
