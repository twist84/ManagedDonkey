#pragma once

struct debug_symbol
{
	uint32 address;
	uint32 rva_base;
	uint32 name_string_offset;
	uint32 library_object_string_offset;
};

struct debug_symbol_table
{
	int32 number_of_symbols;
	char* string_storage;
	debug_symbol* symbols;
};

struct _stack_walk_globals
{
	int32 fixup;
	uint8 disregard_symbol_names;
	debug_symbol_table symbol_table;
};

struct s_file_reference;
struct _CONTEXT;

//extern uint32 base_address_from_symbol_name(char const*, struct debug_symbol_table*);
extern char const* symbol_name_from_address(uint32 routine_address, int32* out_symbol_offset);
//extern void free_symbol_table(struct debug_symbol_table*);
//extern bool load_symbol_table(char const*, struct debug_symbol_table*);
//extern void stack_walk_print(struct s_file_reference*, struct _CONTEXT*, int32, uint32 const*);
extern void stack_walk_with_context(s_file_reference* file, int16 levels_to_ignore, _CONTEXT* context);
//extern void stack_walk_to_buffer(int16, int32, uint32*);
extern void stack_walk_to_file(s_file_reference* file, int16 levels_to_ignore);
extern void stack_walk(int16 levels_to_ignore);
//extern void stack_walk_disregard_symbol_names(bool);
//extern void stack_walk_dispose();
//extern void stack_walk_initialize();

