function change_value(memory, in_position, in_value_change)
    value_type = memory.get_value_type();
    new_value = cast(...
        memory.get_value(in_position)+cast(in_value_change, value_type), ...
        value_type);
    memory.set_value(in_position, new_value);
end
