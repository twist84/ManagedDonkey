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
	uns16 flags;
	int16 location;
	char path[256];
};
COMPILE_ASSERT(sizeof(file_reference_info) == 0x108);

struct s_file_reference : file_reference_info
{
	s_file_handle handle;
	uns32 file_location;
};
COMPILE_ASSERT(sizeof(s_file_reference) == 0x110);

extern file_reference_info* __cdecl file_reference_get_info(s_file_reference* info);
extern void __cdecl directory_create_or_delete_contents(const char* directory);
extern bool __cdecl file_create_parent_directories_if_not_present(const s_file_reference* info);
extern bool __cdecl file_is_readable(s_file_reference* reference, uns32* error);
extern void __cdecl file_printf(s_file_reference* reference, const char* format, ...);
extern bool __cdecl file_read_into_buffer(s_file_reference* reference, void* buffer, uns32 buffer_length);
extern void* __cdecl file_read_into_memory(s_file_reference* reference, uns32* out_size);
extern void* __cdecl file_read_into_memory_with_null_padding(s_file_reference* reference, uns32* out_size, uns32 pad_size);
extern s_file_reference* __cdecl file_reference_add_directory(s_file_reference* reference, const char* directory);
extern s_file_reference* __cdecl file_reference_add_directory_wide(s_file_reference* reference, const wchar_t* directory);
extern s_file_reference* __cdecl file_reference_agnostic_create(s_file_reference* info, int16 location);
extern s_file_reference* __cdecl file_reference_copy(s_file_reference* destination, const s_file_reference* source);
extern s_file_reference* __cdecl file_reference_create_from_path(s_file_reference* reference, const char* path, bool directory);
extern s_file_reference* __cdecl file_reference_create_from_path_wide(s_file_reference* reference, const wchar_t* path, bool directory);
extern wchar_t* __cdecl file_reference_get_fullpath_wide(const s_file_reference* reference, wchar_t* out_name, int32 name_length);
extern char* __cdecl file_reference_get_fullpath(const s_file_reference* reference, char* out_name, int32 name_length);
extern int16 __cdecl file_reference_get_location(const s_file_reference* reference);
extern char* __cdecl file_reference_get_name(const s_file_reference* reference, uns32 flags, char* out_name, int32 name_length);
extern wchar_t* __cdecl file_reference_get_name_wide(const s_file_reference* reference, uns32 flags, wchar_t* out_name, int32 name_length);
extern const char* __cdecl file_reference_get_path_for_debugging(const s_file_reference* reference);
extern s_file_reference* __cdecl file_reference_remove_directory(s_file_reference* reference);
extern s_file_reference* __cdecl file_reference_remove_name(s_file_reference* reference);
extern s_file_reference* __cdecl file_reference_set_name(s_file_reference* reference, const char* name);
extern s_file_reference* __cdecl file_reference_set_name_wide(s_file_reference* reference, const wchar_t* name);
extern bool __cdecl file_references_equal(const s_file_reference* reference_a, const s_file_reference* reference_b);
extern void __cdecl file_trim(s_file_reference* reference, int32 size);
extern void __cdecl file_vprintf(s_file_reference* reference, const char* format, char* list);
extern int32 __cdecl find_files(uns32 flags, const s_file_reference* directory, int32 maximum_count, s_file_reference* references);

