#pragma once

#include "cseries/async_buffer.hpp"

struct s_saved_film_history_archive_record
{
	int32 film_file_position;
	int32 film_tick;
	int32 history_file_position;
	int32 update_number;
	uns32 game_state_checksum;
	int32 determinism_file_position;
	int32 determinism_file_index;
	bool chapter;
	bool in_window;
	bool working;
	bool valid;
};
static_assert(sizeof(s_saved_film_history_archive_record) == 0x20);

struct c_saved_film_history_record_manager
{
	c_static_array<s_saved_film_history_archive_record, 16> m_archive_records;
};
static_assert(sizeof(c_saved_film_history_record_manager) == sizeof(s_saved_film_history_archive_record) * 16);

struct s_saved_film_history_globals
{
	c_async_stored_buffer_set<1>* history_buffer;
	c_saved_film_history_record_manager record_manager;
	int32 estimated_length_in_ticks;
	uns32 last_revert_time;
	bool reverted_last_tick;
};
static_assert(sizeof(s_saved_film_history_globals) == 0x210);

extern void saved_film_history_render_debug();

