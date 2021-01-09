classdef VectorMemory < handle
    properties (GetAccess = public, SetAccess = private)
        default_value;
        starting_position;
        extend_data_fun;
        positive_data;
        notpositive_data;
    end

    methods
        function obj = VectorMemory(varargin)
            input_parser = inputParser;
            input_parser.addParameter('default_value', 0);
            input_parser.addParameter('starting_position', 0);
            input_parser.addParameter(...
                'extend_data_fun', ...
                @BFMachinePac.MemoryTypes.extend_by_appending_needed, ...
                @(s) isa(s, 'function_handle'));
            input_parser.parse(varargin{:});
            obj.default_value = input_parser.Results.default_value;
            obj.starting_position = input_parser.Results.starting_position;
            obj.extend_data_fun = input_parser.Results.extend_data_fun;
            obj.positive_data = repmat(obj.default_value, 0);
            obj.notpositive_data = repmat(obj.default_value, 0);
        end
        function res = get_value(obj, in_position)
            [is_in_positive, position_num] = get_raw_position(in_position);
            if is_in_positive
                res = ...
                    get_raw_value(...
                        obj.positive_data, ...
                        position_num, ...
                        obj.default_value);
            else
                res = ...
                    get_raw_value(...
                        obj.notpositive_data, ...
                        position_num, ...
                        obj.default_value);
            end
            function res = get_raw_value(...
                    in_vector, ...
                    in_position, ...
                    in_default_value)
                res = in_default_value;
                if in_position <= length(in_vector)
                    res = in_vector(in_position);
                end
            end
        end
        function set_value(obj, in_position, in_value)
            [is_in_positive, position_num] = get_raw_position(in_position);
            if is_in_positive
                obj.positive_data = ...
                    insert_value(...
                        obj.positive_data, ...
                        position_num, ...
                        in_value);
            else
                obj.notpositive_data = ...
                    insert_value(...
                        obj.notpositive_data, ...
                        position_num, ...
                        in_value);
            end
            function out_vector = insert_value(...
                    in_vector, ...
                    in_position, ...
                    in_value)
                if in_position <= length(in_vector)
                    out_vector = in_vector;
                    out_vector(in_position) = in_value;
                elseif in_value == obj.default_value
                    out_vector = in_vector;
                else
                    out_vector = ...
                        obj.extend_data_fun(...
                            in_vector, in_position, obj.default_value);
                     out_vector(in_position) = ...
                         cast(in_value, 'like', obj.default_value);
                end
            end
        end

        function res = get_starting_position(obj)
            res = obj.starting_position;
        end

        function res = get_value_type(obj)
            res = class(obj.default_value);
        end

        function res = get_position_type(obj)
            res = class(obj.starting_position);
        end
    end
end

function [is_in_positive, position_num] = get_raw_position(in_position)
    if in_position >= 1
        is_in_positive = true;
        position_num = in_position;
    else
        is_in_positive = false;
        position_num = -in_position+1;
    end
end
