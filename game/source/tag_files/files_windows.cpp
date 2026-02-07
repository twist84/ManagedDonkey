#include "tag_files/files_windows.hpp"

#include "cseries/cseries_events.hpp"
#include "memory/module.hpp"
#include "memory/thread_local.hpp"

#include <windows.h>
#include <time.h>

HOOK_DECLARE(0x00528B60, file_close);
HOOK_DECLARE(0x00528C40, file_compare_last_modification_dates);
HOOK_DECLARE(0x00528C60, file_copy_to);
HOOK_DECLARE(0x00528DD0, file_copy_to_recursive);
HOOK_DECLARE(0x00528FB0, file_create);
HOOK_DECLARE(0x00529130, file_date_format_for_output);
HOOK_DECLARE(0x00529170, file_delete);
HOOK_DECLARE(0x005292F0, file_delete_recursive);
HOOK_DECLARE(0x005294F0, file_error);
//HOOK_DECLARE(0x005295D0, file_error);
HOOK_DECLARE(0x005295F0, file_exists);
//HOOK_DECLARE(0x00529750, file_get_creation_date);
//HOOK_DECLARE(0x005298C0, file_get_eof);
//HOOK_DECLARE(0x00529980, file_get_last_modification_date);
//HOOK_DECLARE(0x00529AF0, file_get_position);
HOOK_DECLARE(0x00529B00, file_get_size);
//HOOK_DECLARE(0x00529C60, file_handle_is_valid);
//HOOK_DECLARE(0x00529C80, file_is_directory);
//HOOK_DECLARE(0x00529CA0, file_last_modification_date_to_time);
//HOOK_DECLARE(0x00529DE0, file_location_get_full_path);
//HOOK_DECLARE(0x00529E50, file_location_get_full_path);
//HOOK_DECLARE(0x00529EC0, file_location_get_full_path_wide);
//HOOK_DECLARE(0x00529F10, file_location_get_full_path_wide);
//HOOK_DECLARE(0x0052A0C0, file_move_to);
HOOK_DECLARE(0x0052A220, file_open);
//HOOK_DECLARE(0x0052A4D0, file_path_add_extension);
//HOOK_DECLARE(0x0052A530, file_path_add_name);
//HOOK_DECLARE(0x0052A590, file_path_add_name);
//HOOK_DECLARE(0x0052A5F0, file_path_remove_name);
//HOOK_DECLARE(0x0052A640, file_path_split_wide);
//HOOK_DECLARE(0x0052A710, file_path_split);
HOOK_DECLARE(0x0052A7E0, file_read);
HOOK_DECLARE(0x0052A850, file_read_from_position);
HOOK_DECLARE(0x0052A9A0, file_reference_create);
//HOOK_DECLARE(0x0052AA50, file_reference_create_appdata);
//HOOK_DECLARE(0x0052AA50, file_reference_create_temporary);
//HOOK_DECLARE(0x0052AAF0, file_reference_create_temporary_from_path);
HOOK_DECLARE(0x0052AC60, file_reference_get_file_handle);
//HOOK_DECLARE(0x0052AC80, file_rename);
//HOOK_DECLARE(0x0052ACB0, file_rename_wide);
//HOOK_DECLARE(0x0052AEC0, file_set_eof);
HOOK_DECLARE(0x0052B060, file_set_position);
//HOOK_DECLARE(0x0052B0D0, file_set_writeable);
//HOOK_DECLARE(0x0052B250, file_write);
//HOOK_DECLARE(0x0052B350, file_write_to_position);
HOOK_DECLARE(0x0052B450, find_files_end);
HOOK_DECLARE(0x0052B4A0, find_files_next);
//HOOK_DECLARE(0x0052B830, find_files_start);
//HOOK_DECLARE(0x0052B850, find_files_start_with_search_spec);
//HOOK_DECLARE(0x0052B980, get_current_file_time);
//HOOK_DECLARE(0x0052BA20, invalidate_file_handle);

bool string_is_absolute_path(const char* s)
{
	return s < csstrstr(s, ":\\");
}

void __cdecl suppress_file_errors(bool suppress)
{
	g_file_errors_suppressed = suppress;
}

bool __cdecl file_errors_suppressed()
{
	return g_file_errors_suppressed;
}

bool __cdecl file_close(s_file_reference* reference)
{
	//bool result = false;
	//HOOK_INVOKE(result =, file_close, reference);
	//return result;

	if (CloseHandle(reference->handle.handle))
	{
		invalidate_file_handle(&reference->handle);
		reference->file_location = 0;

		return true;
	}

	file_error(__FUNCTION__, reference, nullptr, false);

	return false;
}

int __cdecl file_compare_last_modification_dates(const s_file_last_modification_date* date1, const s_file_last_modification_date* date2)
{
	int result = 0;
	HOOK_INVOKE(result =, file_compare_last_modification_dates, date1, date2);
	return result;
}

bool __cdecl file_copy_to(const s_file_reference* reference, s_file_reference* other, bool fail_if_not_exists)
{
	bool result = false;
	HOOK_INVOKE(result =, file_copy_to, reference, other, fail_if_not_exists);
	return result;
}

bool __cdecl file_copy_to_recursive(s_file_reference* reference, s_file_reference* other)
{
	bool result = false;
	HOOK_INVOKE(result =, file_copy_to_recursive, reference, other);
	return result;
}

bool __cdecl file_create(s_file_reference* reference)
{
	//bool result = false;
	//HOOK_INVOKE(result =, file_create, reference);
	//return result;

	ASSERT(reference);

	if (TEST_BIT(reference->flags, _file_reference_flag_is_file_name))
	{
		HANDLE handle = CreateFileA(reference->path, GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
		if (handle && handle != INVALID_HANDLE_VALUE)
		{
			CloseHandle(handle);
			return true;
		}
	}
	else if (CreateDirectoryA(reference->path, nullptr))
	{
		return true;
	}

	return false;
}

void __cdecl file_date_format_for_output(s_file_last_modification_date* date, char* buffer, int32 buffer_size)
{
	HOOK_INVOKE(, file_date_format_for_output, date, buffer, buffer_size);
	INVOKE(0x00529130, file_date_format_for_output, date, buffer, buffer_size);
}

bool __cdecl file_delete(s_file_reference* reference)
{
	bool result = false;
	HOOK_INVOKE(result =, file_delete, reference);
	return result;
}

bool __cdecl file_delete_recursive(s_file_reference* reference)
{
	bool result = false;
	HOOK_INVOKE(result =, file_delete_recursive, reference);
	return result;
}

void __cdecl file_error(const char* file_function, s_file_reference* reference_a, s_file_reference* reference_b, bool suppress_error)
{
	file_reference_info* info0 = file_reference_get_info(reference_a);
	file_reference_info* info1 = nullptr;
	if (reference_b)
	{
		info1 = file_reference_get_info(reference_b);
	}

	DWORD error_message_id = GetLastError();
	if (!file_errors_suppressed() && !suppress_error)
	{
		char system_message[1024]{};
		if (info1)
		{
			csnzprintf(system_message, sizeof(system_message), "%s('%s', '%s')", file_function, info0->path, info1->path);
		}
		else
		{
			csnzprintf(system_message, sizeof(system_message), "%s('%s')", file_function, info0->path);
		}

		char error_message[2048]{};
		get_error_message(error_message_id, error_message);

		event(_event_warning, "system: %s error 0x%08x '%s'", system_message, error_message_id, error_message);
	}
	SetLastError(ERROR_SUCCESS);

	//HOOK_INVOKE(, file_error, file_function, reference_a, reference_b, suppress_error);
}

//0x005295D0 void __cdecl file_error(const char* file_function, s_file_reference* reference, bool suppress_error)

bool __cdecl file_exists(const s_file_reference* reference)
{
	//bool result = false;
	//HOOK_INVOKE(result =, file_exists, reference);
	//return result;

	ASSERT(reference);

	return GetFileAttributesA(reference->path) != INVALID_FILE_ATTRIBUTES;
}

bool __cdecl file_get_creation_date(const s_file_reference* reference, struct s_file_last_modification_date* date)
{
	return INVOKE(0x00529750, file_get_creation_date, reference, date);
}

uns32 __cdecl file_get_eof(const s_file_reference* reference)
{
	return INVOKE(0x005298C0, file_get_eof, reference);
}

bool __cdecl file_get_last_modification_date(const s_file_reference* reference, struct s_file_last_modification_date* date)
{
	return INVOKE(0x00529980, file_get_last_modification_date, reference, date);
}

uns32 __cdecl file_get_position(const s_file_reference* reference)
{
	return INVOKE(0x00529AF0, file_get_position, reference);
}

bool __cdecl file_get_size(s_file_reference* reference, uns32* out_file_size)
{
	//bool result = false;
	//HOOK_INVOKE(result =, file_get_size, reference, out_file_size);
	//return result;

	ASSERT(reference);
	ASSERT(out_file_size);

	WIN32_FILE_ATTRIBUTE_DATA file_info{};
	if (GetFileAttributesExA(reference->path, GetFileExInfoStandard, &file_info))
	{
		*out_file_size = file_info.nFileSizeLow;
		return true;
	}

	file_error(__FUNCTION__, reference, nullptr, false);

	return false;
}

bool __cdecl file_handle_is_valid(s_file_handle handle)
{
	//return INVOKE(0x00529C60, file_handle_is_valid, handle);

	return handle.handle && handle.handle != INVALID_HANDLE_VALUE;
}

bool __cdecl file_is_directory(const s_file_reference* reference)
{
	//return INVOKE(0x00529C80, file_is_directory, reference);

	return !TEST_BIT(reference->flags, _file_reference_flag_is_file_name);
}

bool __cdecl file_last_modification_date_to_time(const s_file_last_modification_date* date, tm* time, bool is_local)
{
	ASSERT(date != nullptr);
	ASSERT(time != nullptr);

	return INVOKE(0x00529CA0, file_last_modification_date_to_time, date, time, is_local);
}

//0x00529DE0 void __cdecl file_location_get_full_path(int16 location, const char* path, wchar_t(&out_full_path)[256])
//0x00529E50 void __cdecl file_location_get_full_path(int16 location, const char* path, wchar_t* out_full_path, int32 full_path_length)
//0x00529EC0 void __cdecl file_location_get_full_path_wide(int16 location, const wchar_t* path, wchwchar_tar_t(&out_full_path)[256])
//0x00529F10 void __cdecl file_location_get_full_path_wide(int16 location, const wchar_t* path, wchar_t* out_full_path, int32 full_path_length)

bool __cdecl file_move_to(const s_file_reference* reference, const s_file_reference* other)
{
	return INVOKE(0x0052A0C0, file_move_to, reference, other);
}

bool __cdecl file_open(s_file_reference* reference, uns32 flags, uns32* error_code)
{
	//bool result = false;
	//HOOK_INVOKE(result =, file_open, reference, flags, error);
	//return result;

	ASSERT(reference != nullptr);
	ASSERT(error_code != nullptr);

	bool result = false;
	uns32 permission = 0;
	uns32 share_mode = 0;
	uns32 flags_and_attributes = FILE_READ_ATTRIBUTES;

	*error_code = _file_open_ok;

	if (TEST_BIT(flags, _permission_read_bit))
	{
		permission = GENERIC_READ;
	}
	if (TEST_BIT(flags, _permission_write_bit))
	{
		permission |= GENERIC_WRITE;
	}

	if (TEST_BIT(flags, _permission_write_allow_read_bit))
	{
		share_mode |= FILE_SHARE_READ;
	}
	if (TEST_BIT(flags, _permission_write_bit))
	{
		share_mode |= FILE_SHARE_WRITE;
	}

	if (TEST_BIT(flags, _file_open_temporary_bit))
	{
		flags_and_attributes = FILE_WRITE_ATTRIBUTES;
	}
	if (TEST_BIT(flags, _file_open_delete_on_close_bit))
	{
		flags_and_attributes = FILE_FLAG_DELETE_ON_CLOSE;
	}
	if (TEST_BIT(flags, _file_open_random_access_bit))
	{
		flags_and_attributes = FILE_FLAG_RANDOM_ACCESS;
	}
	if (TEST_BIT(flags, _file_open_sequential_scan_bit))
	{
		flags_and_attributes = FILE_FLAG_SEQUENTIAL_SCAN;
	}

	HANDLE file_handle = CreateFileA(reference->path, permission, share_mode, nullptr, OPEN_EXISTING, flags_and_attributes, nullptr);
	if (file_handle != nullptr && file_handle != INVALID_HANDLE_VALUE)
	{
		result = true;

		reference->handle.handle = file_handle;
		reference->file_location = 0;

		if (TEST_BIT(flags, _permission_write_append_bit))
		{
			reference->file_location = SetFilePointer(reference->handle.handle, 0, 0, FILE_END);
			if (reference->file_location == INVALID_SET_FILE_POINTER)
			{
				CloseHandle(reference->handle.handle);
				invalidate_file_handle(&reference->handle);
				reference->file_location = 0;

				result = false;
			}
		}
	}
	else
	{
		DWORD last_error = GetLastError();
		switch (last_error)
		{
		case ERROR_FILE_NOT_FOUND:
		{
			*error_code = _file_open_no_exist;
		}
		break;
		case ERROR_PATH_NOT_FOUND:
		{
			*error_code = _file_open_bad_path;
		}
		break;
		case ERROR_ACCESS_DENIED:
		{
			*error_code = _file_open_access_denied;
		}
		break;
		case ERROR_INVALID_DRIVE:
		{
			*error_code = _file_open_bad_drive;
		}
		break;
		case ERROR_SHARING_VIOLATION:
		{
			*error_code = _file_open_sharing_violation;
		}
		break;
		case ERROR_INVALID_NAME:
		{
			*error_code = _file_open_unknown_error;
		}
		break;
		default:
		{
			VASSERT_EXCEPTION(false, "unknown file error", false);
			*error_code = _file_open_unknown_error;
		}
		break;
		}
	}

	if (TEST_BIT(flags, _permission_write_bit))
	{
		reference->flags &= ~FLAG(_file_reference_flag_open_for_write);
	}

	if (!result)
	{
		file_error("file_open", reference, nullptr, TEST_BIT(flags, _file_open_silent_bit));
	}

	return result;
}

void __cdecl file_path_add_extension(wchar_t* path, int32 maximum_path_length, const wchar_t* extension)
{
	INVOKE(0x0052A4D0, file_path_add_extension, path, maximum_path_length, extension);
}

//0x0052A530 void __cdecl file_path_add_name(wchar_t(&path)[256], const wchar_t* name)

void __cdecl file_path_add_name(wchar_t* path, int32 maximum_path_length, const wchar_t* name)
{
	INVOKE(0x0052A590, file_path_add_name, path, maximum_path_length, name);
}

void __cdecl file_path_remove_name(wchar_t* path)
{
	INVOKE(0x0052A5F0, file_path_remove_name, path);
}

void __cdecl file_path_split_wide(wchar_t* path, wchar_t** directory, wchar_t** parent_directory, wchar_t** filename, wchar_t** extension, bool is_file_name)
{
	INVOKE(0x0052A640, file_path_split_wide, path, directory, parent_directory, filename, extension, is_file_name);
}

void __cdecl file_path_split(char* path, char** directory, char** parent_directory, char** filename, char** extension, bool is_file_name)
{
	INVOKE(0x0052A710, file_path_split, path, directory, parent_directory, filename, extension, is_file_name);
}

bool __cdecl file_read(s_file_reference* reference, uns32 size, bool print_error, void* buffer)
{
	//bool result = false;
	//HOOK_INVOKE(result =, file_read, reference, size, print_error, buffer);
	//return result;

	ASSERT(reference);
	ASSERT(buffer);

	uns32 bytes_read = 0;
	bool result = false;

	if (!size)
		return true;

	if (ReadFile(reference->handle.handle, buffer, size, &bytes_read, nullptr))
	{
		if (size == bytes_read)
			result = true;
		else
			SetLastError(ERROR_HANDLE_EOF);
	}

	reference->file_location += bytes_read;

	if (!result)
	{
		file_error(__FUNCTION__, reference, nullptr, print_error);
	}

	return result;
}

bool __cdecl file_read_from_position(s_file_reference* reference, uns32 offset, uns32 size, bool print_error, void* buffer)
{
	//bool result = false;
	//HOOK_INVOKE(result =, file_read_from_position, reference, size, print_error, buffer);
	//return result;

	ASSERT(reference);
	ASSERT(buffer);

	bool result = false;
	if (file_set_position(reference, offset, false))
	{
		result = file_read(reference, size, print_error, buffer);
	}

	return result;
}

void __cdecl file_reference_create(s_file_reference* reference, int16 location)
{
	file_reference_agnostic_create(reference, location);
	invalidate_file_handle(&reference->handle);
}

bool __cdecl file_reference_create_appdata(s_file_reference* reference)
{
	return INVOKE(0x0052AA50, file_reference_create_appdata, reference);
}

bool __cdecl file_reference_create_temporary(s_file_reference* reference)
{
	return INVOKE(0x0052AA50, file_reference_create_temporary, reference);
}

bool __cdecl file_reference_create_temporary_from_path(s_file_reference* reference, const char* path)
{
	return INVOKE(0x0052AAF0, file_reference_create_temporary_from_path, reference, path);
}

bool __cdecl file_reference_get_file_handle(s_file_reference* reference, s_file_handle* out_file_handle)
{
	*out_file_handle = reference->handle;

	return reference->handle.handle != nullptr;
}

bool __cdecl file_rename(s_file_reference* reference, const char* name)
{
	return INVOKE(0x0052AC80, file_rename, reference, name);
}

bool __cdecl file_rename_wide(s_file_reference* reference, const wchar_t* name)
{
	return INVOKE(0x0052ACB0, file_rename_wide, reference, name);
}

bool __cdecl file_set_eof(s_file_reference* reference, uns32 offset)
{
	return INVOKE(0x0052AEC0, file_set_eof, reference, offset);
}

bool __cdecl file_set_position(s_file_reference* reference, uns32 offset, bool print_error)
{
	if (reference->file_location == offset)
		return true;

	if (file_handle_is_valid(reference->handle))
	{
		reference->file_location = SetFilePointer(reference->handle.handle, offset, 0, 0);
	}
	else
	{
		SetLastError(ERROR_INVALID_HANDLE);
	}

	bool result = reference->file_location != INVALID_SET_FILE_POINTER;
	if (!result)
	{
		file_error(__FUNCTION__, reference, nullptr, print_error);
	}

	return result;
}

bool __cdecl file_set_writeable(s_file_reference* reference, bool writeable)
{
	return INVOKE(0x0052B0D0, file_set_writeable, reference, writeable);
}

bool __cdecl file_write(s_file_reference* reference, uns32 size, const void* buffer)
{
	return INVOKE(0x0052B250, file_write, reference, size, buffer);
}

bool __cdecl file_write_to_position(s_file_reference* reference, uns32 offset, uns32 size, const void* buffer)
{
	return INVOKE(0x0052B350, file_write_to_position, reference, offset, size, buffer);
}

void __cdecl find_files_end(s_find_file_data* data)
{
	int16 depth = data->depth;
	if (depth == 0)
	{
		s_file_handle* active_handle = &data->active_find_file_state.handles[depth];
		do
		{
			if (active_handle->handle)
			{
				if (file_handle_is_valid(*active_handle))
				{
					FindClose(active_handle->handle);
					invalidate_file_handle(active_handle);
				}
			}
			depth--;
			active_handle--;

		} while (depth == 0);
	}
}

bool __cdecl find_files_next(s_find_file_data* data, s_file_reference* out_file, s_file_last_modification_date* out_date)
{
	bool result = false;
	HOOK_INVOKE(result =, find_files_next, data, out_file, out_date);
	return result;
}

void __cdecl find_files_start(s_find_file_data* data, uns32 flags, const s_file_reference* file)
{
	find_files_start_with_search_spec(data, flags, file, "*.*");
}

void __cdecl find_files_start_with_search_spec(s_find_file_data* data, uns32 flags, const s_file_reference* file, const char* search_spec)
{
	for (int16 i = 0; i < NUMBEROF(data->active_find_file_state.handles); i++)
	{
		invalidate_file_handle(&data->active_find_file_state.handles[i]);
	}

	data->flags = flags;
	data->depth = 0;
	data->location = file->location;

	data->path.append_print(L"%hs", file->path);
	data->search_spec.append_print(L"%hs", search_spec);
}

void __cdecl get_current_file_time(s_file_last_modification_date* date)
{
	INVOKE(0x0052B980, get_current_file_time, date);
}

void __cdecl invalidate_file_handle(s_file_handle* handle)
{
	handle->handle = INVALID_HANDLE_VALUE;
}

void find_files_recursive(void* userdata, s_file_reference* directory, uns32 open_flags, bool(*file_handler)(void*, s_file_reference*))
{
	s_find_file_data find_file_data{};
	find_files_start(&find_file_data, FLAG(1) | FLAG(2), directory);

	s_file_reference found_file{};
	while (find_files_next(&find_file_data, &found_file, nullptr))
	{
#define string_is_equal(string_a, string_b) (csstrnlen(string_a, NUMBEROF(string_a)) == csstrnlen(string_a, NUMBEROF(string_a)) && csmemcmp(string_a, string_b, csstrnlen(string_a, NUMBEROF(string_a))) == 0)
		if (string_is_equal(found_file.path, ".") || string_is_equal(found_file.path, ".."))
		{
			continue;
		}
#undef string_is_equal

		if (find_file_data.active_find_file_state.find_data.dwFileAttributes & k_file_attribute_directory)
		{
			find_files_recursive(userdata, &found_file, open_flags, file_handler);
		}
		else
		{
			uns32 error = 0;
			if (file_open(&found_file, open_flags, &error))
			{
				file_handler(userdata, &found_file);

				file_close(&found_file);
			}
		}
	}

	find_files_end(&find_file_data);
}

void file_activity_debug_render()
{
}

