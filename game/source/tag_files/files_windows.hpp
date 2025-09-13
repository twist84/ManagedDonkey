#pragma once

#include "cseries/cseries.hpp"
#include "text/unicode.hpp"

enum
{
	_find_files_recursive_bit = 0,
	_find_files_enumerate_directories_bit,
	_find_files_enumerate_files_with_directories_bit,

	NUMBER_OF_FIND_FILES_FLAGS,
};

enum e_file_open_flags
{
	_file_open_flag_desired_access_read = 0,
	_file_open_flag_desired_access_write,
	_file_open_flag_set_file_end_and_close,
	_file_open_flag_share_mode_read,
	_file_open_flag_print_error,
	_file_open_flag_flags_and_attributes_write,
	_file_open_flag_flags_and_attributes_delete_on_close,
	_file_open_flag_flags_and_attributes_random_access,
	_file_open_flag_flags_and_attributes_sequecial_scan,
};

enum e_file_open_error
{
	_file_open_error_none = 0,
	_file_open_error_file_not_found,
	_file_open_error_access_denied,
	_file_open_error_path_not_found,
	_file_open_error_invalid_drive,
	_file_open_error_sharing_violation,
	_file_open_error_unknown,

	k_file_open_error_count
};

enum : uns32
{
	k_file_attribute_readonly               = 0x00000001,
	k_file_attribute_hidden                 = 0x00000002,
	k_file_attribute_system                 = 0x00000004,
	k_file_attribute_directory              = 0x00000010,
	k_file_attribute_archive                = 0x00000020,
	k_file_attribute_device                 = 0x00000040,
	k_file_attribute_normal                 = 0x00000080,
	k_file_attribute_temporary              = 0x00000100,
	k_file_attribute_sparse_file            = 0x00000200,
	k_file_attribute_reparse_point          = 0x00000400,
	k_file_attribute_compressed             = 0x00000800,
	k_file_attribute_offline                = 0x00001000,
	k_file_attribute_not_content_indexed    = 0x00002000,
	k_file_attribute_encrypted              = 0x00004000,
	k_file_attribute_integrity_stream       = 0x00008000,
	k_file_attribute_virtual                = 0x00010000,
	k_file_attribute_no_scrub_data          = 0x00020000,
	k_file_attribute_ea                     = 0x00040000,
	k_file_attribute_pinned                 = 0x00080000,
	k_file_attribute_unpinned               = 0x00100000,
	k_file_attribute_recall_on_open         = 0x00040000,
	k_file_attribute_recall_on_data_access  = 0x00400000,
};

struct s_file_last_modification_date
{
	uns32 low;
	uns32 hi;
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

struct s_find_file_data
{
	struct s_find_file_state
	{
		s_file_handle handles[16];

		struct // WIN32_FIND_DATAW
		{
			uns32 dwFileAttributes;
			s_file_last_modification_date ftCreationTime;
			s_file_last_modification_date ftLastAccessTime;
			s_file_last_modification_date ftLastWriteTime;
			uns32 nFileSizeHigh;
			uns32 nFileSizeLow;
			uns32 dwReserved0;
			uns32 dwReserved1;
			wchar_t  cFileName[260];
			wchar_t  cAlternateFileName[14];
#if defined(_MAC)
			uns32 dwFileType;
			uns32 dwCreatorType;
			uns16  wFinderFlags;
#endif
		} find_data;
	};

	uns32 flags;

	int16 depth;
	int16 location;

	c_static_wchar_string<256> path;
	c_static_wchar_string<256> search_spec;

	s_find_file_state active_find_file_state;

	uns32 __unknown698;
};

struct s_file_reference;
struct tm;

extern bool string_is_absolute_path(const char* s);
extern void __cdecl suppress_file_errors(bool suppress);
extern bool __cdecl file_errors_suppressed();
extern bool __cdecl file_close(s_file_reference* reference);
extern int __cdecl file_compare_last_modification_dates(const s_file_last_modification_date* date1, const s_file_last_modification_date* date2);
extern bool __cdecl file_copy_to(const s_file_reference* reference, s_file_reference* other, bool fail_if_not_exists);
extern bool __cdecl file_copy_to_recursive(s_file_reference* reference, s_file_reference* other);
extern bool __cdecl file_create(s_file_reference* reference);
extern void __cdecl file_date_format_for_output(s_file_last_modification_date* date, char* buffer, int32 buffer_size);
extern bool __cdecl file_delete(s_file_reference* reference);
extern bool __cdecl file_delete_recursive(s_file_reference* reference);
extern void __cdecl file_error(const char* file_function, s_file_reference* reference_a, s_file_reference* reference_b, bool suppress_error);
//extern void __cdecl file_error(const char* file_function, s_file_reference* reference, bool suppress_error);
extern bool __cdecl file_exists(const s_file_reference* reference);
extern bool __cdecl file_get_creation_date(const s_file_reference* reference, struct s_file_last_modification_date* date);
extern uns32 __cdecl file_get_eof(const s_file_reference* reference);
extern bool __cdecl file_get_last_modification_date(const s_file_reference* reference, struct s_file_last_modification_date* date);
extern uns32 __cdecl file_get_position(const s_file_reference* reference);
extern bool __cdecl file_get_size(s_file_reference* reference, uns32* out_file_size);
extern bool __cdecl file_handle_is_valid(s_file_handle handle);
extern bool __cdecl file_is_directory(const s_file_reference* reference);
extern bool __cdecl file_last_modification_date_to_time(const s_file_last_modification_date* date, tm* time, bool is_local);
//extern void __cdecl file_location_get_full_path(int16 location, const char* path, wchar_t(&out_full_path)[256]);
//extern void __cdecl file_location_get_full_path(int16 location, const char* path, wchar_t* out_full_path, int32 full_path_length);
//extern void __cdecl file_location_get_full_path_wide(int16 location, const wchar_t* path, wchwchar_tar_t(&out_full_path)[256]);
//extern void __cdecl file_location_get_full_path_wide(int16 location, const wchar_t* path, wchar_t* out_full_path, int32 full_path_length);
extern bool __cdecl file_move_to(const s_file_reference* reference, const s_file_reference* other);
extern bool __cdecl file_open(s_file_reference* reference, uns32 open_flags, uns32* error);
extern void __cdecl file_path_add_extension(wchar_t* path, int32 maximum_path_length, const wchar_t* extension);
//extern void __cdecl file_path_add_name(wchar_t(&path)[256], const wchar_t* name);
extern void __cdecl file_path_add_name(wchar_t* path, int32 maximum_path_length, const wchar_t* name);
extern void __cdecl file_path_remove_name(wchar_t* path);
extern void __cdecl file_path_split_wide(wchar_t* path, wchar_t** directory, wchar_t** parent_directory, wchar_t** filename, wchar_t** extension, bool is_file_name);
extern void __cdecl file_path_split(char* path, char** directory, char** parent_directory, char** filename, char** extension, bool is_file_name);
extern bool __cdecl file_read(s_file_reference* reference, uns32 size, bool print_error, void* buffer);
extern bool __cdecl file_read_from_position(s_file_reference* reference, uns32 offset, uns32 size, bool print_error, void* buffer);
extern void __cdecl file_reference_create(s_file_reference* reference, int16 location);
extern bool __cdecl file_reference_create_appdata(s_file_reference* reference);
extern bool __cdecl file_reference_create_temporary(s_file_reference* reference);
extern bool __cdecl file_reference_create_temporary_from_path(s_file_reference* reference, const char* path);
extern bool __cdecl file_reference_get_file_handle(s_file_reference* reference, s_file_handle* out_file_handle);
extern bool __cdecl file_rename(s_file_reference* reference, const char* name);
extern bool __cdecl file_rename_wide(s_file_reference* reference, const wchar_t* name);
extern bool __cdecl file_set_eof(s_file_reference* reference, uns32 offset);
extern bool __cdecl file_set_position(s_file_reference* reference, uns32 offset, bool print_error);
extern bool __cdecl file_set_writeable(s_file_reference* reference, bool writeable);
extern bool __cdecl file_write(s_file_reference* reference, uns32 size, const void* buffer);
extern bool __cdecl file_write_to_position(s_file_reference* reference, uns32 offset, uns32 size, const void* buffer);
extern void __cdecl find_files_end(s_find_file_data* data);
extern bool __cdecl find_files_next(s_find_file_data* data, s_file_reference* out_file, s_file_last_modification_date* out_date);
extern void __cdecl find_files_start(s_find_file_data* data, uns32 flags, const s_file_reference* file);
extern void __cdecl find_files_start_with_search_spec(s_find_file_data* data, uns32 flags, const s_file_reference* file, const char* search_spec);
extern void __cdecl get_current_file_time(s_file_last_modification_date* date);
extern void __cdecl invalidate_file_handle(s_file_handle* handle);
extern void find_files_recursive(void* userdata, s_file_reference* directory, uns32 open_flags, bool(*file_handler)(void*, s_file_reference*));

extern void file_activity_debug_render();

