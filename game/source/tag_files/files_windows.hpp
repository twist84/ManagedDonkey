#pragma once

#include "cseries/cseries.hpp"
#include "text/unicode.hpp"

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

struct s_find_file_data
{
	struct s_find_file_state
	{
		s_file_handle handles[16];

		struct // WIN32_FIND_DATAW
		{
			dword dwFileAttributes;
			s_file_last_modification_date ftCreationTime;
			s_file_last_modification_date ftLastAccessTime;
			s_file_last_modification_date ftLastWriteTime;
			dword nFileSizeHigh;
			dword nFileSizeLow;
			dword dwReserved0;
			dword dwReserved1;
			wchar_t  cFileName[260];
			wchar_t  cAlternateFileName[14];
#ifdef _MAC
			dword dwFileType;
			dword dwCreatorType;
			word  wFinderFlags;
#endif
		} find_data;
	};

	dword_flags flags;

	short depth;
	short location;

	c_static_wchar_string<256> path;
	c_static_wchar_string<256> search_spec;

	s_find_file_state active_find_file_state;

	dword __unknown698;
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

struct s_file_reference;
extern void suppress_file_errors(bool suppress);
extern bool file_errors_suppressed();

extern bool __cdecl file_get_size(s_file_reference* file_reference, dword* out_file_size);
extern bool __cdecl file_read(s_file_reference* file_reference, dword size, bool print_error, void* buffer);
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

extern void find_files_end(s_find_file_data* data);
extern bool find_files_next(s_find_file_data* data, s_file_reference* out_file, struct s_file_last_modification_date* out_date);
extern void find_files_start(s_find_file_data* data, dword_flags flags, s_file_reference const* file);
extern void find_files_start_with_search_spec(s_find_file_data* data, dword_flags flags, s_file_reference const* file, char const* search_spec);