#pragma once

#include "cseries/cseries.hpp"
#include "multithreading/synchronized_value.hpp"
#include "tag_files/files.hpp"

enum
{
	k_maximum_async_task_data_size = 0x220
};

enum e_async_category
{
	_async_category_none = 0,
	_async_category_background_copy,
	_async_category_cache_file_misc,
	_async_category_cache_file_texture,
	_async_category_cache_file_geometry,
	_async_category_cache_file_sound,
	_async_category_cache_file_animation,
	_async_category_text,
	_async_category_saved_games,
	_async_category_online_files,

	k_async_category_count
};

enum e_async_priority
{
	_async_priority_background_idle_loading = 0,
	_async_priority_background_user_task,
	_async_priority_background_important_loading,
	_async_priority_background_important_user_task,
	_async_priority_predicted,
	_async_priority_important_non_blocking,
	_async_priority_blocking_below_sound,
	_async_priority_sound,
	_async_priority_very_important_non_blocking,
	_async_priority_blocking_generic,
	_async_priority_blocking_render,
	_async_priority_blocking_animation,

	k_async_priorities_count
};

enum e_async_completion
{

};

struct s_create_file_task
{
	wchar_t file_path[256];
	dword desired_access;
	dword share_mode;
	dword creation_disposition;
	dword flags_and_attributes;
	s_file_handle volatile* file;
	bool always_open;
};
static_assert(sizeof(s_create_file_task) == 0x218);

struct s_read_position_task
{
	s_file_handle file;
	void* buffer;
	long buffer_size;
	long file_offset;
	c_synchronized_long* size;
	long buffer_offset;
};
static_assert(sizeof(s_read_position_task) == 0x18);

enum e_write_position_flags
{
	_write_position_flush_file_bit = 0,

	k_write_position_flags
};

struct s_write_position_task
{
	s_file_handle file;
	void* buffer;
	long size;
	dword offset;
	c_synchronized_long* success;
	dword total_bytes_written;
	c_flags<e_write_position_flags, dword, k_write_position_flags> flags;
	bool __unknown1C;
};
static_assert(sizeof(s_write_position_task) == 0x20);

struct s_write_position_task2
{
	s_file_handle file;
	long buffer_length;
	void* buffer;
	bool* success;
};
static_assert(sizeof(s_write_position_task2) == 0x10);

struct s_copy_position_task
{
	s_file_handle source_file;
	s_file_handle destination_file;
	long source_offset;
	long destination_offset;
	void* buffer;
	long buffer_size;
	c_synchronized_long* success;
	long buffer_offset;
	long __unknown20;
	bool __unknown24;
};
static_assert(sizeof(s_copy_position_task) == 0x28);

struct s_set_file_size_task
{
	s_file_handle file;
	long file_size;
	c_synchronized_long* success;
};
static_assert(sizeof(s_set_file_size_task) == 0xC);

struct s_delete_file_task
{
	char file_path[256];
	bool directory;
	c_synchronized_long* success;
};
static_assert(sizeof(s_delete_file_task) == 0x108);

struct s_enumerate_files_task
{
	char directory[260];
	s_find_file_data* find_file_data;
	dword find_file_flags;
	long maximum_count;
	s_file_reference* references;
	long* reference_count;
	c_synchronized_long* success;
};
static_assert(sizeof(s_enumerate_files_task) == 0x11C);

struct s_read_entire_file_task
{
	wchar_t file_path[256];
	void* buffer;
	dword buffer_size;
	dword volatile* size;
	c_synchronized_long* success;
	s_file_handle file;
	dword file_size;
};
static_assert(sizeof(s_read_entire_file_task) == 0x218);

struct s_write_buffer_to_file_task
{
	wchar_t file_path[256];
	void const* buffer;
	dword size;
	long __unknown208;
	c_synchronized_long* success;
	s_file_handle file;
};
static_assert(sizeof(s_write_buffer_to_file_task) == 0x214);

struct s_close_file_task
{
	s_file_handle file;
};
static_assert(sizeof(s_close_file_task) == 0x4);

struct s_get_file_size_task
{
	s_file_handle file;
	long volatile* file_size;
};
static_assert(sizeof(s_get_file_size_task) == 0x8);

struct s_file_raw_handle_based_task
{
	s_file_handle file;
};
static_assert(sizeof(s_file_raw_handle_based_task) == 0x4);

//struct s_font_loading_state;
//struct s_font_loading_task
//{
//	s_font_loading_state* loading_state;
//};
//static_assert(sizeof(s_font_loading_task) == 0x4);

//struct s_configuration_enumeration_task
//{
//	long enumeration_index;
//	s_find_file_data* find_file_data;
//};
//static_assert(sizeof(s_configuration_enumeration_task) == 0x8);

union s_async_task
{
	s_create_file_task create_file;
	s_read_position_task read_position;
	s_write_position_task write_position;
	s_copy_position_task copy_position;
	s_set_file_size_task set_file_size;
	s_delete_file_task delete_file;
	s_enumerate_files_task enumerate_files;
	s_read_entire_file_task read_entire_file;
	s_write_buffer_to_file_task write_buffer_to_file;
	s_close_file_task close_file;
	s_get_file_size_task get_file_size;
	s_file_raw_handle_based_task file_raw_handle_based;

	byte storage[k_maximum_async_task_data_size];
};
static_assert(sizeof(s_async_task) == k_maximum_async_task_data_size);

using async_work_callback_t = e_async_completion __cdecl(s_async_task*);

struct s_async_task_element
{
	e_async_priority priority;
	long task_index;
	s_async_task task;
	e_async_category category;
	async_work_callback_t* work_callback;
	c_synchronized_long* done;
	s_async_task_element* next;
};
static_assert(sizeof(s_async_task_element) == 0x238);

struct s_async_globals
{
	long __unknown0;
	s_async_task_element task_list[25];
	s_async_task_element* free_list;
	s_async_task_element* work_list;
	s_async_task_element* temp_list;
	long __unknown3788;
};
static_assert(sizeof(s_async_globals) == 0x378C);

extern s_async_globals& g_async_globals;

extern bool __cdecl async_usable();
extern void __cdecl internal_async_yield_until_done(c_synchronized_long* done, bool with_idle, bool with_spinner, char const* file, long line);

