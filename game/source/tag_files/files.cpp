#include "tag_files/files.hpp"

#include <string.h>

file_reference_info* file_reference_get_info(s_file_reference* info)
{
	ASSERT(info);
	ASSERT(info->signature == FILE_REFERENCE_SIGNATURE);
	//ASSERT(VALID_BITS(info->flags, NUMBER_OF_REFERENCE_INFO_FLAGS));
	ASSERT(info->location >= NONE && info->location < NUMBER_OF_FILE_REFERENCE_LOCATIONS);

	return info;
}

bool __cdecl datastore_read(const char* file_name, const char* field_name, int32 length, void* destination)
{
	ASSERT(NULL != file_name);
	ASSERT(NULL != field_name);

	ASSERT('\\0' != file_name[0]);
	ASSERT('\\0' != field_name[0]);

	ASSERT(length <= DATASTORE_MAX_DATA_SIZE);
	ASSERT(strlen_debug(field_name) < DATASTORE_MAX_FIELD_NAME_SIZE);

	return INVOKE(0x00527BC0, datastore_read, file_name, field_name, length, destination);
}

bool __cdecl datastore_write(const char* file_name, const char* field_name, int32 length, const void* source)
{
	ASSERT(NULL != file_name);
	ASSERT(NULL != field_name);

	ASSERT('\\0' != file_name[0]);
	ASSERT('\\0' != field_name[0]);

	ASSERT(length <= DATASTORE_MAX_DATA_SIZE);
	ASSERT(strlen_debug(field_name) < DATASTORE_MAX_FIELD_NAME_SIZE);

	return INVOKE(0x00527D20, datastore_write, file_name, field_name, length, source);
}

void __cdecl directory_create_or_delete_contents(const char* directory)
{
	INVOKE(0x00527F30, directory_create_or_delete_contents, directory);
}

bool __cdecl file_create_parent_directories_if_not_present(const struct s_file_reference* info)
{
	return INVOKE(0x00527FF0, file_create_parent_directories_if_not_present, info);
}

bool __cdecl file_is_readable(s_file_reference* reference, uns32* error)
{
	return INVOKE(0x00528210, file_is_readable, reference, error);
}

//.text:00528240 ; 

void __cdecl file_printf(s_file_reference* reference, const char* format, ...)
{
	//INVOKE(0x00528270, file_printf, reference, format, ...);

	va_list list;
	va_start(list, format);
	file_vprintf(reference, format, list);
	va_end(list);
}

bool __cdecl file_read_into_buffer(s_file_reference* reference, void* buffer, uns32 buffer_length)
{
	return INVOKE(0x005282D0, file_read_into_buffer, reference, buffer, buffer_length);
}

void* __cdecl file_read_into_memory(s_file_reference* reference, uns32* out_size)
{
	return INVOKE(0x00528320, file_read_into_memory, reference, out_size);
}

void* __cdecl file_read_into_memory_with_null_padding(s_file_reference* reference, uns32* out_size, uns32 pad_size)
{
	return INVOKE(0x00528350, file_read_into_memory_with_null_padding, reference, out_size, pad_size);
}

s_file_reference* __cdecl file_reference_add_directory(s_file_reference* reference, const char* directory)
{
	return INVOKE(0x00528410, file_reference_add_directory, reference, directory);
}

s_file_reference* __cdecl file_reference_add_directory_wide(s_file_reference* reference, const wchar_t* directory)
{
	return INVOKE(0x00528490, file_reference_add_directory_wide, reference, directory);
}

s_file_reference* __cdecl file_reference_agnostic_create(s_file_reference* info, int16 location)
{
	//return INVOKE(0x00528500, file_reference_agnostic_create, reference, location);

	ASSERT(info);
	ASSERT(location >= NONE && location < NUMBER_OF_FILE_REFERENCE_LOCATIONS);

	csmemset(info, 0, sizeof(s_file_reference));
	info->signature = FILE_REFERENCE_SIGNATURE;
	info->location = location;

	return info;
}

s_file_reference* __cdecl file_reference_copy(s_file_reference* destination, const s_file_reference* source)
{
	//return INVOKE(0x00528530, file_reference_copy, destination, source);

	csmemcpy(destination, source, sizeof(file_reference_info));
	return destination;
}

s_file_reference* __cdecl file_reference_create_from_path(s_file_reference* reference, const char* path, bool directory)
{
	return INVOKE(0x00528550, file_reference_create_from_path, reference, path, directory);
}

s_file_reference* __cdecl file_reference_create_from_path_wide(s_file_reference* reference, const wchar_t* path, bool directory)
{
	return INVOKE(0x005285B0, file_reference_create_from_path_wide, reference, path, directory);
}

wchar_t* __cdecl file_reference_get_fullpath_wide(const s_file_reference* reference, wchar_t* out_name, int32 name_length)
{
	return INVOKE(0x005285F0, file_reference_get_fullpath_wide, reference, out_name, name_length);
}

char* __cdecl file_reference_get_fullpath(const s_file_reference* reference, char* out_name, int32 name_length)
{
	return INVOKE(0x00528630, file_reference_get_fullpath, reference, out_name, name_length);
}

int16 __cdecl file_reference_get_location(const s_file_reference* reference)
{
	return INVOKE(0x00528690, file_reference_get_location, reference);
}

char* file_reference_get_name(const s_file_reference* reference, uns32 flags, char* out_name, int32 name_length)
{
	return INVOKE(0x005286A0, file_reference_get_name, reference, flags, out_name, name_length);
}

wchar_t* file_reference_get_name_wide(const s_file_reference* reference, uns32 flags, wchar_t* out_name, int32 name_length)
{
	return INVOKE(0x005286E0, file_reference_get_name_wide, reference, flags, out_name, name_length);
}

const char* __cdecl file_reference_get_path_for_debugging(const s_file_reference* reference)
{
	return INVOKE(0x005287C0, file_reference_get_path_for_debugging, reference);
}

s_file_reference* __cdecl file_reference_remove_directory(s_file_reference* reference)
{
	return INVOKE(0x005287D0, file_reference_remove_directory, reference);
}

s_file_reference* __cdecl file_reference_remove_name(s_file_reference* reference)
{
	return INVOKE(0x00528820, file_reference_remove_name, reference);
}

s_file_reference* __cdecl file_reference_set_name(s_file_reference* reference, const char* name)
{
	return INVOKE(0x00528880, file_reference_set_name, reference, name);
}

s_file_reference* __cdecl file_reference_set_name_wide(s_file_reference* reference, const wchar_t* name)
{
	return INVOKE(0x005288B0, file_reference_set_name_wide, reference, name);
}

bool __cdecl file_references_equal(const s_file_reference* reference_a, const s_file_reference* reference_b)
{
	return INVOKE(0x00528930, file_references_equal, reference_a, reference_b);
}

void __cdecl file_trim(s_file_reference* reference, int32 size)
{
	INVOKE(0x005289A0, file_trim, reference, size);
}

void __cdecl file_vprintf(s_file_reference* reference, const char* format, char* list)
{
	INVOKE(0x00528A90, file_printf, reference, format, list);
}

int32 __cdecl find_files(uns32 flags, const s_file_reference* directory, int32 maximum_count, s_file_reference* references)
{
	return INVOKE(0x00528AF0, find_files, flags, directory, maximum_count, references);
}

