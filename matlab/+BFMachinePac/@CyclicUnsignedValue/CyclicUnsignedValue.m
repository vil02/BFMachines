classdef CyclicUnsignedValue
    properties (GetAccess = public, SetAccess = private)
        value;
        limit_value;
        computation_type;
    end
    methods
        function obj = CyclicUnsignedValue(in_value, varargin)
            if ~isequal(class(in_value), 'BFMachinePac.CyclicUnsignedValue')
                input_parser = inputParser;
                input_parser.addRequired('in_value')
                input_parser.addParameter(...
                    'limit_value', 256, ...
                    @(s) isnumeric(s) && isscalar(s) && s > 0);
                input_parser.addParameter(...
                    'computation_type', 'double',...
                    @(s) isa(s, 'char'));
                input_parser.parse(in_value, varargin{:});
                obj.limit_value = input_parser.Results.limit_value;
                obj.computation_type = ...
                    input_parser.Results.computation_type;
                obj.value = ...
                    mod(cast(in_value, obj.computation_type), ...
                        cast(obj.limit_value, obj.computation_type));
            else
                obj.value = in_value.value;
                obj.limit_value = in_value.limit_value;
                obj.computation_type = in_value.computation_type;
            end
        end
        function res = plus(obj, value_change)
            raw_res_val = ...
                obj.to_computation_type(obj.value)+...
                obj.to_computation_type(value_change);
            res = obj.to_same_type(raw_res_val);
        end
        function res = minus(obj, value_change)
            raw_res_val = ...
                obj.to_computation_type(obj.value)-...
                obj.to_computation_type(value_change);
            res = obj.to_same_type(raw_res_val);
        end
        function res = eq(obj, other_value)
            if isequal('BFMachinePac.CyclicUnsignedValue', class(other_value))
                res = ...
                    obj.value == other_value.value && ...
                    obj.limit_value == other_value.limit_value && ...
                    isequal(obj.computation_type, other_value.computation_type);
            else
                res = obj == obj.to_same_type(other_value);
            end
        end
        function res = ne(obj, other_value)
            res = ~(obj == other_value);
        end
    end

    methods (Access = private)
        function res = to_same_type(obj, in_value)
            res = BFMachinePac.CyclicUnsignedValue(...
                in_value, ...
                'limit_value', obj.limit_value, ...
                'computation_type', obj.computation_type);
        end
        function res = to_computation_type(obj, in_value)
            res = cast(in_value, obj.computation_type);
        end
    end
end
