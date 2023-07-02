#pragma once

struct s_file_reference;
struct _CONTEXT;

extern char const* symbol_name_from_address(unsigned long routine_address, long* out_symbol_offset);
extern void stack_walk_with_context(s_file_reference* file, short levels_to_ignore, _CONTEXT* context);
extern void stack_walk_to_file(s_file_reference* file, short levels_to_ignore);
extern void stack_walk(short levels_to_ignore);

