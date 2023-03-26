#include "tag_files/files.hpp"

#include "cseries/console.hpp"

#include <string.h>

file_reference_info* file_reference_get_info(s_file_reference* info)
{
	ASSERT(info);
	ASSERT(info->signature == FILE_REFERENCE_SIGNATURE);
	//ASSERT(VALID_BITS(info->flags, NUMBER_OF_REFERENCE_INFO_FLAGS));
	ASSERT(info->location >= NONE && info->location < NUMBER_OF_FILE_REFERENCE_LOCATIONS);

	return info;
}

bool __cdecl file_create_parent_directories_if_not_present(struct s_file_reference const* info)
{
	return INVOKE(0x00527FF0, file_create_parent_directories_if_not_present, info);
}

s_file_reference* __cdecl file_reference_agnostic_create(s_file_reference* info, short location)
{
	ASSERT(info);
	ASSERT(location >= NONE && location < NUMBER_OF_FILE_REFERENCE_LOCATIONS);

	memset(info, 0, sizeof(s_file_reference));
	info->signature = FILE_REFERENCE_SIGNATURE;
	info->location = location;

	return info;

	//return INVOKE(0x00528500, file_reference_agnostic_create, file_reference, location);
}

s_file_reference* __cdecl file_reference_create_from_path(s_file_reference* file_reference, char const* path, bool a3)
{
	return INVOKE(0x00528550, file_reference_create_from_path, file_reference, path, a3);
}

s_file_reference* __cdecl file_reference_copy(s_file_reference* info, s_file_reference const* other)
{
	memcpy(info, other, sizeof(file_reference_info));
	return info;

	//return INVOKE(0x00528530, file_reference_copy, info, other);
}

s_file_reference* __cdecl file_reference_add_directory(s_file_reference* file_reference, wchar_t const* directory)
{
	return INVOKE(0x00528490, file_reference_add_directory, file_reference, directory);
}

//s_file_reference* __cdecl file_reference_remove_directory(s_file_reference* file_reference)

s_file_reference* __cdecl file_reference_set_name(s_file_reference* file_reference, char const* name)
{
	return INVOKE(0x005288B0, file_reference_set_name, file_reference, name);
}

//s_file_reference* __cdecl file_reference_remove_name(s_file_reference*)
//long __cdecl find_files(unsigned long, s_file_reference const*, long, s_file_reference*)
//bool __cdecl file_is_readable(s_file_reference*, unsigned long*)
//void __cdecl file_trim(s_file_reference*, long)

void* __cdecl file_read_into_memory_with_null_padding(s_file_reference* file_reference, dword* out_size, dword pad_size)
{
	return INVOKE(0x00528350, file_read_into_memory_with_null_padding, file_reference, out_size, pad_size);
}

void* __cdecl file_read_into_memory(s_file_reference* file_reference, dword* out_size)
{
	return INVOKE(0x00528320, file_read_into_memory, file_reference, out_size);
}

bool __cdecl file_read_into_buffer(s_file_reference* file_reference, void* buffer, dword buffer_length)
{
	return INVOKE(0x005282D0, file_read_into_buffer, file_reference, buffer, buffer_length);
}

//void __cdecl file_printf(s_file_reference*, char const*, ...)
//void __cdecl file_vprintf(s_file_reference*, char const*, char*)

wchar_t* file_reference_get_name(s_file_reference const* file_reference, dword_flags flags, c_static_wchar_string<256>* out_name, long name_length)
{
	return INVOKE(0x005286E0, file_reference_get_name, file_reference, flags, out_name, name_length);
}
