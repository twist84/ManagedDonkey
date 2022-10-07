#include "tag_files/files_windows.hpp"

#include "cseries/console.hpp"
#include "memory/module.hpp"
#include "memory/thread_local.hpp"

#include <windows.h>
#include <assert.h>

HOOK_DECLARE(0x00529B00, file_get_size);
HOOK_DECLARE(0x0052A7E0, file_read);
HOOK_DECLARE(0x00528B60, file_close);
HOOK_DECLARE(0x00528C40, file_compare_last_modification_dates);
HOOK_DECLARE(0x00528C60, file_copy_to);
HOOK_DECLARE(0x00528DD0, file_copy_to_recursive);
HOOK_DECLARE(0x00528FB0, file_create);
HOOK_DECLARE(0x00529130, file_date_format_for_output);
HOOK_DECLARE(0x00529170, file_delete);
HOOK_DECLARE(0x005294F0, file_error);
HOOK_DECLARE(0x005295F0, file_exists);
HOOK_DECLARE(0x0052A220, file_open);

HOOK_DECLARE(0x0052B450, find_files_end);
HOOK_DECLARE(0x0052B4A0, find_files_next);
HOOK_DECLARE(0x0052B830, find_files_start);
HOOK_DECLARE(0x0052B850, find_files_start_with_search_spec);

void suppress_file_errors(bool suppress)
{
    FUNCTION_BEGIN(true);

    if (get_tls())
        get_tls()->g_file_errors_suppressed = suppress;
}

bool file_errors_suppressed()
{
    FUNCTION_BEGIN(true);

    if (get_tls())
        return get_tls()->g_file_errors_suppressed;

    return true;
}

bool file_get_size(s_file_reference* file_reference, dword* out_file_size)
{
    //bool result = false;
    //HOOK_INVOKE(result =, file_get_size, file_reference, out_file_size);
    //return result;

    assert(file_reference);
    assert(out_file_size);

    WIN32_FILE_ATTRIBUTE_DATA file_info{};
    if (GetFileAttributesExA(file_reference->path, GetFileExInfoStandard, &file_info))
    {
        *out_file_size = file_info.nFileSizeLow;
        return true;
    }

    file_error(__FUNCTION__, file_reference, nullptr, false);

    return false;
}

bool __cdecl file_read(s_file_reference* file_reference, dword size, bool print_error, void* buffer)
{
    //bool result = false;
    //HOOK_INVOKE(result =, file_read, file_reference, size, print_error, buffer);
    //return result;

    assert(file_reference);
    assert(buffer);
    
    unsigned long bytes_read = 0;
    bool result = false;
    
    if (!size)
        return true;
    
    if (ReadFile(file_reference->handle.handle, buffer, size, &bytes_read, NULL))
    {
        if (size == bytes_read)
            result = true;
        else
            SetLastError(ERROR_HANDLE_EOF);
    }
    
    file_reference->position += bytes_read;

    if (!result)
        file_error(__FUNCTION__, file_reference, nullptr, print_error);

    return result;
}

bool __cdecl file_close(s_file_reference* file_reference)
{
    //bool result = false;
    //HOOK_INVOKE(result =, file_close, file_reference);
    //return result;

    if (CloseHandle(file_reference->handle.handle))
    {
        file_reference->handle.handle = INVALID_HANDLE_VALUE;
        file_reference->position = 0;

        return true;
    }

    file_error(__FUNCTION__, file_reference, nullptr, false);

    return false;
}

int __cdecl file_compare_last_modification_dates(s_file_last_modification_date const* date1, s_file_last_modification_date const* date2)
{
    int result = 0;
    HOOK_INVOKE(result =, file_compare_last_modification_dates, date1, date2);
    return result;
}

bool __cdecl file_copy_to(s_file_reference* file_reference, s_file_reference* other, bool fail_if_not_exists)
{
    bool result = false;
    HOOK_INVOKE(result =, file_copy_to, file_reference, other, fail_if_not_exists);
    return result;
}

bool __cdecl file_copy_to_recursive(s_file_reference* file_reference, s_file_reference* other)
{
    bool result = false;
    HOOK_INVOKE(result =, file_copy_to_recursive, file_reference, other);
    return result;
}

bool __cdecl file_create(s_file_reference* file_reference)
{
    //bool result = false;
    //HOOK_INVOKE(result =, file_create, file_reference);
    //return result;

    assert(file_reference);

    if (TEST_BIT(file_reference->flags, _file_reference_flag_is_file_name))
    {
        HANDLE handle = CreateFileA(file_reference->path, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
        if (handle && handle != INVALID_HANDLE_VALUE)
        {
            CloseHandle(handle);
            return true;
        }
    }
    else if (CreateDirectoryA(file_reference->path, NULL))
        return true;

    return false;
}

void __cdecl file_date_format_for_output(s_file_last_modification_date* date, char* buffer, long buffer_size)
{
    HOOK_INVOKE(, file_date_format_for_output, date, buffer, buffer_size);
    INVOKE(0x00529130, file_date_format_for_output, date, buffer, buffer_size);
}

bool __cdecl file_delete(s_file_reference* file_reference)
{
    FUNCTION_BEGIN(true);

    bool result = false;
    HOOK_INVOKE(result =, file_delete, file_reference);
    return result;
}

//void file_error(char const*, s_file_reference const*, s_file_reference const*, bool)
void __cdecl file_error(char const* file_function, s_file_reference* file0, s_file_reference* file1, bool suppress_error)
{
    FUNCTION_BEGIN(true);

    file_reference_info* info0 = file_reference_get_info(file0);
    file_reference_info* info1 = nullptr;
    if (file1)
        info1 = file_reference_get_info(file1);

    DWORD error_message_id = GetLastError();
    if (!file_errors_suppressed() && !suppress_error)
    {
        char system_message[1024]{};
        if (info1)
            csnzprintf(system_message, sizeof(system_message), "%s('%s', '%s')", file_function, info0->path, info1->path);
        else
            csnzprintf(system_message, sizeof(system_message), "%s('%s')", file_function, info0->path);

        char error_message[2048]{};
        get_error_message(error_message_id, error_message);

        c_console::write_line("system: %s error 0x%08x '%s'", system_message, error_message_id, error_message);
    }
    SetLastError(ERROR_SUCCESS);

    //HOOK_INVOKE(, file_error, file_function, file0, file1, suppress_error);
}
bool __cdecl file_exists(s_file_reference const* file_reference)
{
    FUNCTION_BEGIN(true);

    //bool result = false;
    //HOOK_INVOKE(result =, file_exists, file_reference);
    //return result;

    assert(file_reference);

    return GetFileAttributesA(file_reference->path) != INVALID_FILE_ATTRIBUTES;
}

bool __cdecl file_open(s_file_reference* file_reference, dword open_flags, dword* error)
{
    FUNCTION_BEGIN(true);

    //bool result = false;
    //HOOK_INVOKE(result =, file_open, file_reference, open_flags, error);
    //return result;

    assert(file_reference);
    assert(error);

    bool result = false;
    unsigned long desired_access = 0;
    unsigned long share_mode = 0;
    unsigned long flags_and_attributes = FILE_READ_ATTRIBUTES;

    *error = 0;

    if (TEST_BIT(open_flags, _file_open_flag_desired_access_read))
        desired_access = GENERIC_READ;
    if (TEST_BIT(open_flags, _file_open_flag_desired_access_write))
        desired_access |= GENERIC_WRITE;

    if (TEST_BIT(open_flags, _file_open_flag_share_mode_read))
        share_mode |= FILE_SHARE_READ;
    if (TEST_BIT(open_flags, _file_open_flag_desired_access_write))
        share_mode |= FILE_SHARE_WRITE;

    if (TEST_BIT(open_flags, _file_open_flag_flags_and_attributes_write))
        flags_and_attributes = FILE_WRITE_ATTRIBUTES;
    if (TEST_BIT(open_flags, _file_open_flag_flags_and_attributes_delete_on_close))
        flags_and_attributes = FILE_FLAG_DELETE_ON_CLOSE;
    if (TEST_BIT(open_flags, _file_open_flag_flags_and_attributes_random_access))
        flags_and_attributes = FILE_FLAG_RANDOM_ACCESS;
    if (TEST_BIT(open_flags, _file_open_flag_flags_and_attributes_sequecial_scan))
        flags_and_attributes = FILE_FLAG_SEQUENTIAL_SCAN;

    HANDLE handle = CreateFileA(file_reference->path, desired_access, share_mode, NULL, OPEN_EXISTING, flags_and_attributes, NULL);
    if (!handle || handle == INVALID_HANDLE_VALUE)
    {
        switch (GetLastError())
        {
        case ERROR_FILE_NOT_FOUND:
            *error = 1;
            break;
        case ERROR_PATH_NOT_FOUND:
            *error = 3;
            break;
        case ERROR_ACCESS_DENIED:
            *error = 2;
            break;
        case ERROR_INVALID_DRIVE:
            *error = 4;
            break;
        case ERROR_SHARING_VIOLATION:
            *error = 5;
            break;
        default:
            *error = 6;
            break;
        }
    }
    else
    {
        result = true;

        file_reference->handle.handle = handle;
        file_reference->position = 0;

        if (TEST_BIT(open_flags, _file_open_flag_set_file_end_and_close))
        {
            file_reference->position = SetFilePointer(file_reference->handle.handle, 0, 0, FILE_END);
            if (file_reference->position == INVALID_SET_FILE_POINTER)
            {
                CloseHandle(file_reference->handle.handle);
                file_reference->handle.handle = INVALID_HANDLE_VALUE;
                file_reference->position = 0;

                result = false;
            }
        }
    }

    if (TEST_BIT(open_flags, _file_open_flag_desired_access_write))
        file_reference->flags &= ~FLAG(_file_reference_flag_open_for_write);

    return result;
}

void find_files_end(s_find_file_data* data)
{
    short depth = data->depth;
    if (depth == 0)
    {
        s_file_handle* active_handle = &data->active_find_file_state.handles[depth];
        do
        {
            if (active_handle->handle)
            {
                if (active_handle->handle != INVALID_HANDLE_VALUE)
                {
                    FindClose(active_handle->handle);
                    active_handle->handle = INVALID_HANDLE_VALUE;
                }
            }
            --depth;
            --active_handle;

        } while (depth == 0);
    }
}

bool find_files_next(s_find_file_data* data, s_file_reference* out_file, s_file_last_modification_date* out_date)
{
    bool result = false;
    HOOK_INVOKE(result =, find_files_next, data, out_file, out_date);
    return result;
}

void find_files_start(s_find_file_data* data, dword_flags flags, s_file_reference const* file)
{
    find_files_start_with_search_spec(data, flags, file, "*.*");
}

void find_files_start_with_search_spec(s_find_file_data* data, dword_flags flags, s_file_reference const* file, char const* search_spec)
{
    for (short i = 0; i < NUMBEROF(data->active_find_file_state.handles); i++)
        data->active_find_file_state.handles[i].handle = INVALID_HANDLE_VALUE;

    data->flags = flags;
    data->depth = 0;
    data->location = file->location;

    data->path.append_print(L"%hs", file->path);
    data->search_spec.append_print(L"%hs", search_spec);
}
