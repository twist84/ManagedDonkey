#pragma once

#include "cseries/cseries.hpp"
#include "tag_files/files_windows.hpp"

#define FILE_REFERENCE_SIGNATURE 'filo'
#define NUMBER_OF_FILE_REFERENCE_LOCATIONS 2

enum e_file_reference_flags
{
	_file_reference_flag_is_file_name = 0,
	_file_reference_flag_open_for_write,
};

struct file_reference_info
{
	dword signature;
	word_flags flags;
	short location;
	long_string path;
};
static_assert(sizeof(file_reference_info) == 0x108);

struct s_file_reference : file_reference_info
{
	s_file_handle handle;
	long position;
};
static_assert(sizeof(s_file_reference) == 0x110);

extern file_reference_info* file_reference_get_info(s_file_reference* info);
extern s_file_reference* __cdecl file_reference_agnostic_create(s_file_reference* info, short location);
extern s_file_reference* __cdecl file_reference_create_from_path(s_file_reference*, char const*, bool);
extern s_file_reference* __cdecl file_reference_copy(s_file_reference*, s_file_reference const*);
extern s_file_reference* __cdecl file_reference_add_directory(s_file_reference*, wchar_t const*);
//extern s_file_reference* __cdecl file_reference_remove_directory(s_file_reference*);
extern s_file_reference* __cdecl file_reference_set_name(s_file_reference* file_reference, char const* name);
//extern s_file_reference* __cdecl file_reference_remove_name(s_file_reference*);
//extern long __cdecl find_files(dword, s_file_reference const*, long, s_file_reference*);
//extern bool __cdecl file_is_readable(s_file_reference*, dword*);
//extern void __cdecl file_trim(s_file_reference*, long);
extern void* __cdecl file_read_into_memory_with_null_padding(s_file_reference*, dword*, dword);
extern void* __cdecl file_read_into_memory(s_file_reference*, dword*);
extern bool __cdecl file_read_into_buffer(s_file_reference*, void*, dword);
//extern void __cdecl file_printf(s_file_reference*, char const*, ...);
//extern void __cdecl file_vprintf(s_file_reference*, char const*, char*);