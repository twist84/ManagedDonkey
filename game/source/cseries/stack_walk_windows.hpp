#pragma once

struct debug_symbol
{
	uns32 address;
	uns32 rva_base;
	uns32 name_string_offset;
	uns32 library_object_string_offset;
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
	uns8 disregard_symbol_names;
	debug_symbol_table symbol_table;
};

struct s_file_reference;
struct _CONTEXT;

//extern uns32 base_address_from_symbol_name(const char*, struct debug_symbol_table*);
extern const char* symbol_name_from_address(uns32 routine_address, int32* out_symbol_offset);
//extern void free_symbol_table(struct debug_symbol_table*);
//extern bool load_symbol_table(const char*, struct debug_symbol_table*);
//extern void stack_walk_print(struct s_file_reference*, struct _CONTEXT*, int32, const uns32*);
extern void stack_walk_with_context(s_file_reference* file, int16 levels_to_ignore, _CONTEXT* context);
//extern void stack_walk_to_buffer(int16, int32, uns32*);
extern void stack_walk_to_file(s_file_reference* file, int16 levels_to_ignore);
extern void stack_walk(int16 levels_to_ignore);
//extern void stack_walk_disregard_symbol_names(bool);
//extern void stack_walk_dispose();
//extern void stack_walk_initialize();

