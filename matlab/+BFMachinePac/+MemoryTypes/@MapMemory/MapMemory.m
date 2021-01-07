classdef MapMemory < handle
    properties (GetAccess = public, SetAccess = private)
        default_value;
        data;
        remove_default_values;
        position_type;
    end

    methods
        function obj = MapMemory(varargin)
            input_parser = inputParser;
            input_parser.addParameter('default_value', 0);
            input_parser.addParameter('position_type', 'double', @ischar);
            input_parser.addParameter(...
                'remove_default_values', true, @islogical);
            input_parser.parse(varargin{:});

            obj.default_value = input_parser.Results.default_value;
            obj.data = containers.Map(...
                'KeyType', input_parser.Results.position_type, ...
                'ValueType', class(obj.default_value));
            obj.remove_default_values = ...
                input_parser.Results.remove_default_values;
            obj.position_type = input_parser.Results.position_type;
        end
        function res = get_value(obj, in_position)
            res = obj.default_value;
            if obj.data.isKey(in_position)
                res = obj.data(in_position);
            end
        end
        function set_value(obj, in_position, in_value)
            if in_value == obj.default_value
               if obj.data.isKey(in_position)
                   if obj.remove_default_values
                       obj.data.remove(in_position);
                   else
                       obj.data(in_position) = in_value;
                   end
               end
            else
                obj.data(in_position) = in_value;
            end
        end

        function res = get_starting_position(obj)
            res = cast(0, obj.position_type);
        end

        function res = get_value_type(obj)
            res = class(obj.default_value);
        end

        function res = get_position_type(obj)
            res = obj.position_type;
        end
    end
end
