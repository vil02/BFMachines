function change_value(memory, in_position, in_value_change)
    new_value = ...
        memory.cast_to_value_type(...
            memory.get_value(in_position)+in_value_change);
    memory.set_value(in_position, new_value);
end
