#pragma once

struct debug_symbol
{
	unsigned long address;
	unsigned long rva_base;
	unsigned long name_string_offset;
	unsigned long library_object_string_offset;
};

struct debug_symbol_table
{
	long number_of_symbols;
	char* string_storage;
	debug_symbol* symbols;
};

struct _stack_walk_globals
{
	long fixup;
	unsigned char disregard_symbol_names;
	debug_symbol_table symbol_table;
};

struct s_file_reference;
struct _CONTEXT;

//extern unsigned long base_address_from_symbol_name(char const*, struct debug_symbol_table*);
extern char const* symbol_name_from_address(unsigned long routine_address, long* out_symbol_offset);
//extern void free_symbol_table(struct debug_symbol_table*);
//extern bool load_symbol_table(char const*, struct debug_symbol_table*);
//extern void stack_walk_print(struct s_file_reference*, struct _CONTEXT*, long, unsigned long const*);
extern void stack_walk_with_context(s_file_reference* file, short levels_to_ignore, _CONTEXT* context);
//extern void stack_walk_to_buffer(short, long, unsigned long*);
extern void stack_walk_to_file(s_file_reference* file, short levels_to_ignore);
extern void stack_walk(short levels_to_ignore);
//extern void stack_walk_disregard_symbol_names(bool);
//extern void stack_walk_dispose();
//extern void stack_walk_initialize();

