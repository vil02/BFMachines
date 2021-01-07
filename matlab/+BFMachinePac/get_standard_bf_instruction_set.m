function res = get_standard_bf_instruction_set()
    res.instruction_type = 'char';
    res.move_left = '<';
    res.move_right = '>';
    res.increase_value = '+';
    res.decrease_value = '-';
    res.read_value = ',';
    res.read_value = '.';
    res.begin_loop = '[';
    res.end_loop = ']';
end