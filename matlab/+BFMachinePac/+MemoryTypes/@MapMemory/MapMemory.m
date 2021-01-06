classdef MapMemory < handle
    properties (GetAccess = public, SetAccess = private)
        data;
        default_value;
        remove_default_values;
        position_type;
    end

    methods
        function obj = MapMemory(...
                in_position_type, in_default_value, in_remove_devault_values)
            obj.data = containers.Map(...
                'KeyType', in_position_type, ...
                'ValueType', class(in_default_value));
            obj.default_value = in_default_value;
            assert(isequal(class(in_remove_devault_values), 'logical'));
            obj.remove_default_values = in_remove_devault_values;
            obj.position_type = in_position_type;
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
