classdef InputStream < handle
    properties (GetAccess = public, SetAccess = private)
        data;
        cur_position;
    end

    methods
        function obj = InputStream(in_data)
            obj.data = in_data;
            obj.cur_position = 1;
        end

        function res = get(obj)
            res = obj.data(obj.cur_position);
            obj.cur_position = obj.cur_position+1;
        end
    end
end
