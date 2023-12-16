#pragma once

#include "multithreading/synchronized_value.hpp"
#include "tag_files/files.hpp"

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

struct s_create_file_task
{
	wchar_t file_name[256];
	unsigned long desired_access;
	unsigned long share_mode;
	unsigned long creation_disposition;
	unsigned long flags_and_attributes;
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

struct s_write_position_task
{
	s_file_handle file;
	long buffer_length;
	void* buffer;
	bool* success;
};
static_assert(sizeof(s_write_position_task) == 0x10);

struct s_copy_position_task
{
	s_file_handle source_file;
	s_file_handle destination_file;
	long source_offset;
	long destination_offset;
	byte* buffer;
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
	char file_name[256];
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
	wchar_t file_name[256];
	void* buffer;
	long buffer_size;
	long* size;
	c_synchronized_long* success;
	s_file_handle file;
	long file_size;
};
static_assert(sizeof(s_read_entire_file_task) == 0x218);

struct s_write_buffer_to_file_task
{
	wchar_t filename[256];
	void const* buffer;
	long size;
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

