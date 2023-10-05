#pragma once

#include "cseries/cseries.hpp"
#include "tag_files/files_windows.hpp"
#include "text/unicode.hpp"

#define FILE_REFERENCE_SIGNATURE 'filo'
#define NUMBER_OF_FILE_REFERENCE_LOCATIONS 2

#define DATASTORE_MAX_DATA_SIZE 256
#define DATASTORE_MAX_FIELD_NAME_SIZE 256

enum e_file_reference_flags
{
	_file_reference_flag_is_file_name = 0,
	_file_reference_flag_open_for_write,
};

enum e_name
{
	_name_directory_bit = 0,
	_name_parent_directory_bit,
	_name_file_bit,
	_name_extension_bit,

	k_name_count
};

struct file_reference_info
{
	tag signature;
	word_flags flags;
	short location;
	c_static_string<k_tag_long_string_length> path;
};
static_assert(sizeof(file_reference_info) == 0x108);

struct s_file_reference : file_reference_info
{
	s_file_handle handle;
	long position;
};
static_assert(sizeof(s_file_reference) == 0x110);

extern file_reference_info* __cdecl file_reference_get_info(s_file_reference* info);
extern void __cdecl directory_create_or_delete_contents(char const* directory);
extern bool __cdecl file_create_parent_directories_if_not_present(s_file_reference const* info);
extern bool __cdecl file_is_readable(s_file_reference* reference, dword* error);
extern void __cdecl file_printf(s_file_reference* reference, char const* format, ...);
extern bool __cdecl file_read_into_buffer(s_file_reference* reference, void* buffer, dword buffer_length);
extern void* __cdecl file_read_into_memory(s_file_reference* reference, dword* out_size);
extern void* __cdecl file_read_into_memory_with_null_padding(s_file_reference* reference, dword* out_size, dword pad_size);
extern s_file_reference* __cdecl file_reference_add_directory(s_file_reference* reference, char const* directory);
extern s_file_reference* __cdecl file_reference_add_directory_wide(s_file_reference* reference, wchar_t const* directory);
extern s_file_reference* __cdecl file_reference_agnostic_create(s_file_reference* info, short location);
extern s_file_reference* __cdecl file_reference_copy(s_file_reference* info, s_file_reference const* other);
extern s_file_reference* __cdecl file_reference_create_from_path(s_file_reference* reference, char const* path, bool a3);
extern s_file_reference* __cdecl file_reference_create_from_path_wide(s_file_reference* reference, wchar_t const* path, bool a3);
extern wchar_t* __cdecl file_reference_get_fullpath_wide(s_file_reference const* reference, wchar_t* out_full_path, long full_path_length);
extern char* __cdecl file_reference_get_fullpath(s_file_reference const* reference, char* out_full_path, long full_path_length);
extern short __cdecl file_reference_get_location(s_file_reference const* reference);
extern char* __cdecl file_reference_get_name(s_file_reference const* reference, dword_flags flags, char* out_name, long name_length);
extern wchar_t* __cdecl file_reference_get_name_wide(s_file_reference const* reference, dword_flags flags, wchar_t* out_name, long name_length);
extern char const* __cdecl file_reference_get_path_for_debugging(s_file_reference const* reference);
extern s_file_reference* __cdecl file_reference_remove_directory(s_file_reference* reference);
extern s_file_reference* __cdecl file_reference_remove_name(s_file_reference* reference);
extern s_file_reference* __cdecl file_reference_set_name(s_file_reference* reference, char const* name);
extern s_file_reference* __cdecl file_reference_set_name_wide(s_file_reference* reference, wchar_t const* name);
extern bool __cdecl file_references_equal(s_file_reference const* reference_a, s_file_reference const* reference_b);
extern void __cdecl file_trim(s_file_reference* reference, long size);
extern void __cdecl file_vprintf(s_file_reference* reference, char const* format, char* list);
extern long __cdecl find_files(dword flags, s_file_reference const* directory, long maximum_count, s_file_reference* references);

