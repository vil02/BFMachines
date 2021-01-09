classdef FindMatching < handle
    properties (GetAccess = public, SetAccess = private)
        code;
        begin_symbol;
        end_symbol;
        cache_data;
    end

    methods
        function obj = FindMatching(...
                in_code, in_begin_symbol, in_end_symbol)
            assert(length(unique({...
                class(in_code), ...
                class(in_begin_symbol), ...
                class(in_end_symbol)})) == 1);
            assert(in_begin_symbol ~= in_end_symbol);
            obj.code = in_code;
            obj.begin_symbol = in_begin_symbol;
            obj.end_symbol = in_end_symbol;
            obj.cache_data = zeros(size(in_code));
        end

        function res = find(obj, in_pos)
            res = obj.cache_data(in_pos);
            if res == 0
                res = obj.find_raw(in_pos);
                obj.cache_data(in_pos) = res;
                obj.cache_data(res) = in_pos;
            end
        end
    end

    methods (Access = private)
        function res = find_raw(obj, in_pos)
            assert(...
                ismember(obj.code(in_pos), ...
                    [obj.begin_symbol, obj.end_symbol]));
            in_symbol = obj.code(in_pos);
            [target_symbol, search_dir, cont_cond] = ...
                get_search_params(in_symbol);
            cur_pos = in_pos+search_dir;
            cur_count = 1;
            res = 0;
            while res == 0 && cont_cond(cur_pos)
                cur_symbol = obj.code(cur_pos);
                if cur_symbol == in_symbol
                    cur_count = cur_count+1;
                elseif cur_symbol == target_symbol
                    cur_count = cur_count-1;
                    if cur_count == 0
                        res = cur_pos;
                    end
                end
                cur_pos = cur_pos+search_dir;
            end
            if res == 0
                error(...
                    'Could not find matching %s to %s at position %d', ...
                    target_symbol, obj.code(in_pos), in_pos);
            end
            function [target_symbol, search_dir, cont_cond] = ...
                    get_search_params(...
                        in_start_symbol)
                if in_start_symbol == obj.begin_symbol
                    target_symbol = obj.end_symbol;
                    search_dir = 1;
                    cont_cond = @(tmp_pos) tmp_pos <= length(obj.code);
                else
                    target_symbol = obj.begin_symbol;
                    search_dir = -1;
                    cont_cond = @(tmp_pos) tmp_pos >= 1;
                end
            end
        end
    end
end
