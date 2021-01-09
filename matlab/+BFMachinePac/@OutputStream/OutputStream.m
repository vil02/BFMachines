classdef OutputStream < handle
    properties (GetAccess = public, SetAccess = private)
        data;
    end

    methods
        function obj = OutputStream()
            obj.data = {};
        end

        function put(obj, in_value)
            obj.data{end+1} = in_value;
        end
        function res = get_data(obj)
            res = obj.data;
        end
    end
end
