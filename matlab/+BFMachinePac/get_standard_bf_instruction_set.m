function res = get_standard_bf_instruction_set()
    res.instruction_type = 'char';
    res.move_left = '<';
    res.move_right = '>';
    res.decrease_value = '-';
    res.increase_value = '+';
    res.read_value = ',';
    res.print_value = '.';
    res.begin_loop = '[';
    res.end_loop = ']';
end
