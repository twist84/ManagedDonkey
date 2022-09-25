#pragma once

#include "cseries/cseries.hpp"

struct s_file_last_modification_date
{
	dword low;
	dword hi;
};
static_assert(sizeof(s_file_last_modification_date) == 0x8);

struct s_file_handle
{
	void* handle;
};
static_assert(sizeof(s_file_handle) == 0x4);

struct s_indirect_file
{
	// map_file_index?
	void* handle;
};
static_assert(sizeof(s_indirect_file) == 0x4);

struct s_file_reference;
extern void suppress_file_errors(bool suppress);
extern bool file_errors_suppressed();

extern bool __cdecl file_close(s_file_reference* file_reference);
extern int __cdecl file_compare_last_modification_dates(s_file_last_modification_date const* date1, s_file_last_modification_date const* date2);
extern bool __cdecl file_copy_to(s_file_reference* file_reference, s_file_reference* other, bool fail_if_not_exists);
extern bool __cdecl file_copy_to_recursive(s_file_reference* file_reference, s_file_reference* other);
extern bool __cdecl file_create(s_file_reference* file_reference);
extern void __cdecl file_date_format_for_output(s_file_last_modification_date* date, char* buffer, long buffer_size);
extern bool __cdecl file_delete(s_file_reference* file_reference);
extern void __cdecl file_error(char const* file_function, s_file_reference* file0, s_file_reference* file1, bool suppress_error);
extern bool __cdecl file_exists(s_file_reference const* file_reference);
extern bool __cdecl file_open(s_file_reference*, dword open_flags, dword* error);