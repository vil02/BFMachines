classdef OutputStream < handle
    properties (GetAccess = public, SetAccess = private)
        data;
    end

    methods
        function obj = OutputStream()
            obj.data = java.util.ArrayList;
        end

        function put(obj, in_value)
            obj.data.add(in_value);
        end
        function res = get_data(obj)
            res = cell2mat(cell((obj.data.toArray())));
        end
    end
end
