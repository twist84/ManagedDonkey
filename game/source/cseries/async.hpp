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
	_async_completion_unknown0 = 0,
	_async_completion_done,
	_async_completion_thread_exit,

	k_async_completion_count
};

enum e_yield_reason
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
	s_file_handle file_handle;
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
	s_file_handle file_handle;
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
	s_file_handle file_handle;
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
	s_file_handle file_handle;
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
	s_file_handle file_handle;
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
	s_file_handle file_handle;
};
static_assert(sizeof(s_write_buffer_to_file_task) == 0x214);

struct s_close_file_task
{
	s_file_handle file_handle;
};
static_assert(sizeof(s_close_file_task) == 0x4);

struct s_get_file_size_task
{
	s_file_handle file_handle;
	dword volatile* file_size;
};
static_assert(sizeof(s_get_file_size_task) == 0x8);

struct s_file_raw_handle_based_task
{
	s_file_handle file_handle;
};
static_assert(sizeof(s_file_raw_handle_based_task) == 0x4);

struct s_font_loading_state;
struct s_font_loading_task
{
	s_font_loading_state* loading_state;
};
static_assert(sizeof(s_font_loading_task) == 0x4);

struct s_configuration_enumeration_task
{
	long enumeration_index;
	s_find_file_data* find_file_data;
};
static_assert(sizeof(s_configuration_enumeration_task) == 0x8);

union s_async_task;
struct s_simple_callback_task
{
	e_async_completion(__cdecl* callback)(s_async_task* task, void* data, long data_size);
	byte callback_data[0x11C - sizeof(short)];
	c_enum<long, short, 0, 0x120> callback_data_size;
};
static_assert(sizeof(s_simple_callback_task) == 0x120);

union s_async_task
{
	s_create_file_task create_file_task;
	s_read_position_task read_position_task;
	s_write_position_task write_position_task;
	s_copy_position_task copy_position_task;
	s_set_file_size_task set_file_size_task;
	s_delete_file_task delete_file_task;
	s_enumerate_files_task enumerate_files_task;
	s_read_entire_file_task read_entire_file_task;
	s_write_buffer_to_file_task write_buffer_to_file_task;
	s_close_file_task close_file_task;
	s_get_file_size_task get_file_size_task;
	s_file_raw_handle_based_task file_raw_handle_based_task;
	s_font_loading_task font_loading_task;
	s_configuration_enumeration_task configuration_enumeration_task;
	s_simple_callback_task simple_callback_task;

	byte storage[k_maximum_async_task_data_size];
};
static_assert(sizeof(s_async_task) == k_maximum_async_task_data_size);

using async_work_callback_t = e_async_completion __cdecl(s_async_task*);

struct s_async_queue_element
{
	e_async_priority priority;
	long task_id;
	s_async_task task;
	e_async_category category;
	async_work_callback_t* work_callback;
	c_synchronized_long* done;
	s_async_queue_element* next;
};
static_assert(sizeof(s_async_queue_element) == 0x238);

struct s_async_globals
{
	long __unknown0;
	s_async_queue_element task_list[25];
	s_async_queue_element* free_list;
	s_async_queue_element* work_list;
	s_async_queue_element* temp_list;
	long tasks_in_queue;

	// Added back by us
	static c_synchronized_long work_delay_milliseconds;
};
static_assert(sizeof(s_async_globals) == 0x378C);

extern s_async_globals& async_globals;

extern bool __cdecl async_busy_hint();
extern bool __cdecl async_category_in_queue(e_async_category category);
extern void __cdecl async_dispose();
extern void __cdecl async_idle();
extern void __cdecl async_initialize();
extern dword __cdecl async_main(void* thread_params);
extern long __cdecl async_task_add(e_async_priority priority, s_async_task* task, e_async_category category, e_async_completion(*work_callback)(s_async_task*), c_synchronized_long* done);
extern long __cdecl async_task_add_ex(e_async_priority priority, s_async_task* task, e_async_category category, e_async_completion(*work_callback)(s_async_task*), c_synchronized_long* done, bool a6);
extern bool __cdecl async_task_change_priority(long task_id, e_async_priority priority);
extern long __cdecl async_tasks_in_queue();
extern bool __cdecl async_test_completion_flag(c_synchronized_long* completion_flag);
extern bool __cdecl async_usable();
extern bool __cdecl async_work_function();
extern void __cdecl async_yield_until_done_function(c_synchronized_long* done, bool(*yield_function)(c_synchronized_long*), bool idle, bool networking, bool spinner, e_yield_reason yield_reason);
extern void __cdecl free_list_add(s_async_queue_element* element);
extern s_async_queue_element* __cdecl free_list_get_and_remove(bool a1);
extern void __cdecl internal_async_yield_until_done(c_synchronized_long* done, bool idle, bool spinner, char const* file, long line);
extern void __cdecl internal_async_yield_until_done_attributed(c_synchronized_long* done, bool idle, bool spinner, e_yield_reason yield_reason, char const* file, long line);
extern void __cdecl internal_async_yield_until_done_with_networking(c_synchronized_long* done, bool idle, bool spinner, char const* file, long line);
extern bool __cdecl simple_yield_function(c_synchronized_long* done);
extern s_async_queue_element* __cdecl sub_508BD0();
extern void __cdecl sub_508C00(s_async_queue_element* element);
extern void __cdecl work_list_remove_internal_assumes_locked_does_not_clear_id_does_not_suspend(s_async_queue_element* element);

