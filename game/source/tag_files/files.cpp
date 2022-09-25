#include "tag_files/files.hpp"

#include "cseries/console.hpp"

#include <assert.h>
#include <string.h>

file_reference_info* file_reference_get_info(s_file_reference* info)
{
	assert(info);
	assert(info->signature == FILE_REFERENCE_SIGNATURE);
	//assert(VALID_BITS(info->flags, NUMBER_OF_REFERENCE_INFO_FLAGS));
	assert(info->location >= NONE && info->location < NUMBER_OF_FILE_REFERENCE_LOCATIONS);

	return info;
}

s_file_reference* __cdecl file_reference_agnostic_create(s_file_reference* info, short location)
{
	FUNCTION_BEGIN(true);

	assert(info);
	assert(location >= NONE && location < NUMBER_OF_FILE_REFERENCE_LOCATIONS);

	memset(info, 0, sizeof(s_file_reference));
	info->signature = FILE_REFERENCE_SIGNATURE;
	info->location = location;

	return info;

	//return INVOKE(0x00528500, file_reference_agnostic_create, file_reference, location);
}

s_file_reference* __cdecl file_reference_create_from_path(s_file_reference* file_reference, char const* path, bool a3)
{
	FUNCTION_BEGIN(true);

	return INVOKE(0x00528550, file_reference_create_from_path, file_reference, path, a3);
}

s_file_reference* __cdecl file_reference_copy(s_file_reference* info, s_file_reference const* other)
{
	FUNCTION_BEGIN(true);

	memcpy(info, other, sizeof(file_reference_info));
	return info;

	//return INVOKE(0x00528530, file_reference_copy, info, other);
}

s_file_reference* __cdecl file_reference_add_directory(s_file_reference* file_reference, wchar_t const* directory)
{
	FUNCTION_BEGIN(true);

	return INVOKE(0x00528490, file_reference_add_directory, file_reference, directory);
}

//s_file_reference* __cdecl file_reference_remove_directory(s_file_reference* file_reference)

s_file_reference* __cdecl file_reference_set_name(s_file_reference* file_reference, char const* name)
{
	FUNCTION_BEGIN(true);

	return INVOKE(0x005288B0, file_reference_set_name, file_reference, name);
}

//s_file_reference* __cdecl file_reference_remove_name(s_file_reference*)
//long __cdecl find_files(unsigned long, s_file_reference const*, long, s_file_reference*)
//bool __cdecl file_is_readable(s_file_reference*, unsigned long*)
//void __cdecl file_trim(s_file_reference*, long)

void* __cdecl file_read_into_memory_with_null_padding(s_file_reference* file_reference, dword* out_size, dword pad_size)
{
	FUNCTION_BEGIN(true);

	return INVOKE(0x00528350, file_read_into_memory_with_null_padding, file_reference, out_size, pad_size);
}

void* __cdecl file_read_into_memory(s_file_reference* file_reference, dword* out_size)
{
	FUNCTION_BEGIN(true);

	return INVOKE(0x00528320, file_read_into_memory, file_reference, out_size);
}

bool __cdecl file_read_into_buffer(s_file_reference* file_reference, void* buffer, dword buffer_length)
{
	FUNCTION_BEGIN(true);

	return INVOKE(0x005282D0, file_read_into_buffer, file_reference, buffer, buffer_length);
}

//void __cdecl file_printf(s_file_reference*, char const*, ...)
//void __cdecl file_vprintf(s_file_reference*, char const*, char*)