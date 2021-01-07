function out_data = extend_by_appending_needed(...
        in_data, in_target_size, default_value)
    fields_needed = in_target_size-length(in_data);
    assert(fields_needed > 0);
    out_data = [in_data, repmat(default_value, 1, fields_needed)];
end