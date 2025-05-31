#pragma once

#include "cseries/cseries.hpp"
#include "multithreading/synchronized_value.hpp"
#include "tag_files/files.hpp"

struct s_create_file_task
{
	wchar_t file_path[256];
	uns32 desired_access;
	uns32 share_mode;
	uns32 creation_disposition;
	uns32 flags_and_attributes;
	s_file_handle volatile* file;
	bool always_open;
};
static_assert(sizeof(s_create_file_task) == 0x218);

struct s_read_position_task
{
	s_file_handle file_handle;
	void* buffer;
	int32 buffer_size;
	int32 file_offset;
	c_synchronized_long* size;
	int32 buffer_offset;
};
static_assert(sizeof(s_read_position_task) == 0x18);

struct s_write_position_task
{
	s_file_handle file_handle;
	void* buffer;
	int32 size;
	uns32 offset;
	c_synchronized_long* success;
	uns32 total_bytes_written;
	c_flags<e_write_position_flags, uns32, k_write_position_flags> flags;
	bool __unknown1C;
};
static_assert(sizeof(s_write_position_task) == 0x20);

struct s_write_position_task2
{
	s_file_handle file_handle;
	int32 buffer_length;
	void* buffer;
	bool* success;
};
static_assert(sizeof(s_write_position_task2) == 0x10);

struct s_copy_position_task
{
	s_file_handle source_file;
	s_file_handle destination_file;
	int32 source_offset;
	int32 destination_offset;
	void* buffer;
	int32 buffer_size;
	c_synchronized_long* success;
	int32 buffer_offset;
	int32 __unknown20;
	bool __unknown24;
};
static_assert(sizeof(s_copy_position_task) == 0x28);

struct s_set_file_size_task
{
	s_file_handle file_handle;
	int32 file_size;
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
	bool find_files_start_called;
	char directory[256];
	s_find_file_data* find_file_data;
	uns32 find_files_flags;
	int32 maximum_count;
	s_file_reference* out_references;
	int32* out_reference_count;
	c_synchronized_long* success;
};
static_assert(sizeof(s_enumerate_files_task) == 0x11C);

struct s_read_entire_file_task
{
	wchar_t path[256];
	void* buffer;
	uns32 buffer_size;
	uns32 volatile* file_size_result;
	c_synchronized_long* success;
	s_file_handle file_handle;
	uns32 file_size;
};
static_assert(sizeof(s_read_entire_file_task) == 0x218);

struct s_write_buffer_to_file_task
{
	wchar_t path[256];
	const void* buffer;
	uns32 buffer_size;
	int32 dst_on_utility_drive;
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
	uns32 volatile* file_size;
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
	e_dvd_find_files_stage stage;
	s_find_file_data* enumeration_data;
};
static_assert(sizeof(s_configuration_enumeration_task) == 0x8);

struct s_dlc_enumeration_task
{
	e_controller_index controller_index;
	e_dlc_find_files_stage stage;
	int32 content_item_index;
	s_find_file_data* enumeration_data;
};
static_assert(sizeof(s_dlc_enumeration_task) == 0x10);

struct s_async_task;
struct s_async_simple_callback_task
{
	e_async_completion(__cdecl* callback)(s_async_task* task, void* data, int32 data_size);
	byte callback_data[0x11C - sizeof(int16)];
	c_enum<int32, int16, 0, 0x120> callback_data_size;
};
static_assert(sizeof(s_async_simple_callback_task) == 0x120);

struct s_load_image_from_file_task
{
	enum e_state
	{
		_state_starting = 0,
		_state_reading,
		_state_decompressing,
		_state_done,
	};

	e_state state;
	s_file_reference* file;
	uns32 file_size;
	char* load_buffer;
	int32 load_buffer_length;
	int32 storage_item_index;
	e_custom_bitmap_desired_aspect_ratio desired_aspect_ratio;
	c_synchronized_long* cancelled;
	c_synchronized_long* success;
	bool image_source_was_dlc;
};
static_assert(sizeof(s_load_image_from_file_task) == 0x28);

struct s_load_image_from_buffer_task
{
	const char* buffer;
	int32 buffer_length;
	int32 storage_item_index;
	e_custom_bitmap_desired_aspect_ratio desired_aspect_ratio;
	c_synchronized_long* success;
};
static_assert(sizeof(s_load_image_from_buffer_task) == 0x14);

struct s_async_task
{
	union
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
		s_dlc_enumeration_task dlc_enumeration_task;
		s_async_simple_callback_task simple_callback_task;
		s_load_image_from_file_task load_image_from_file_task;
		s_load_image_from_buffer_task load_image_from_buffer_task;

		byte storage[k_maximum_async_task_data_size];
	};
};
static_assert(sizeof(s_async_task) == k_maximum_async_task_data_size);

using async_work_callback_t = e_async_completion __cdecl(s_async_task*);

struct s_async_queue_element
{
	e_async_priority priority;
	int32 task_id;
	s_async_task task;
	e_async_category category;
	async_work_callback_t* work_callback;
	c_synchronized_long* done;
	s_async_queue_element* next;
};
static_assert(sizeof(s_async_queue_element) == 0x238);

struct s_async_globals
{
	int32 __unknown0;
	s_async_queue_element task_list[25];
	s_async_queue_element* free_list;
	s_async_queue_element* work_list;
	s_async_queue_element* temp_list;
	int32 tasks_in_queue;

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
extern uns32 __cdecl async_main(void* thread_params);
extern int32 __cdecl async_task_add(e_async_priority priority, s_async_task* task, e_async_category category, e_async_completion(*work_callback)(s_async_task*), c_synchronized_long* done);
extern int32 __cdecl async_task_add_ex(e_async_priority priority, s_async_task* task, e_async_category category, e_async_completion(*work_callback)(s_async_task*), c_synchronized_long* done, bool a6);
extern bool __cdecl async_task_change_priority(int32 task_id, e_async_priority priority);
extern int32 __cdecl async_tasks_in_queue();
extern bool __cdecl async_test_completion_flag(c_synchronized_long* completion_flag);
extern bool __cdecl async_usable();
extern bool __cdecl async_work_function();
extern void __cdecl async_yield_until_done_function(c_synchronized_long* done, bool(*yield_function)(c_synchronized_long*), bool idle, bool networking, bool spinner, e_yield_reason yield_reason);
extern void __cdecl free_list_add(s_async_queue_element* element);
extern s_async_queue_element* __cdecl free_list_get_and_remove(bool block_if_task_list_is_full);
extern void __cdecl internal_async_yield_until_done(c_synchronized_long* done, bool idle_sound, bool show_debug_progress, const char* file, int32 line);
extern void __cdecl internal_async_yield_until_done_attributed(c_synchronized_long* done, bool idle_sound, bool show_debug_progress, e_yield_reason yield_reason, const char* file, int32 line);
extern void __cdecl internal_async_yield_until_done_with_networking(c_synchronized_long* done, bool idle_sound, bool show_debug_progress, const char* file, int32 line);
extern bool __cdecl simple_yield_function(c_synchronized_long* done);
extern int32 __cdecl work_list_add(s_async_queue_element* element);
extern void __cdecl work_list_add_internal_assumes_locked_does_not_set_id_does_not_resume(s_async_queue_element* element);
extern s_async_queue_element* __cdecl work_list_get();
extern void __cdecl work_list_lock_internal();
extern void __cdecl work_list_remove(s_async_queue_element* element);
extern void __cdecl work_list_remove_internal_assumes_locked_does_not_clear_id_does_not_suspend(s_async_queue_element* element);
extern void __cdecl work_list_unlock();

