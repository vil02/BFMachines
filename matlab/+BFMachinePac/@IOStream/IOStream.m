classdef IOStream
    properties
        read_fun;
        write_fun;
    end

    methods
        function obj = IOStream(varargin)
            input_parser = inputParser;
            input_parser.addParameter(...
                'read_fun', ...
                @() error('No input stream!'), ...
                @(s) isa(s, 'function_handle'));
            input_parser.addParameter(...
                'write_fun', ...
                @(~) error('No output stream'), ...
                @(s) isa(s, 'function_handle'));
            input_parser.parse(varargin{:});
            obj.read_fun = input_parser.Results.read_fun;
            obj.write_fun = input_parser.Results.write_fun;
        end

        function res = get(obj)
            res = obj.read_fun();
        end
        function put(obj, in_val)
            obj.write_fun(in_val);
        end
    end
end
