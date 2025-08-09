#pragma once

#include "cseries/async_buffer.hpp"

enum e_saved_film_revert_type;
struct s_saved_film_hud_interface_state;
struct s_simulation_update_metadata;

enum
{
	k_saved_film_history_chapter_archive_count = 10,
	k_saved_film_history_local_archive_count = 6,
};

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

extern c_async_stored_buffer_set<1>* saved_film_history_buffer_acquire();
extern c_async_stored_buffer_set<1>* saved_film_history_buffer_get_without_acquire();
extern void saved_film_history_buffer_release();
extern bool saved_film_history_can_revert_by_type(e_saved_film_revert_type revert_type);
extern void saved_film_history_dispose_from_saved_film_playback();
extern void saved_film_history_flush_game_state();
extern void saved_film_history_get_hud_interface_state(s_saved_film_hud_interface_state* state);
extern int32 saved_film_history_get_target_record_index_by_revert_type(e_saved_film_revert_type revert_type);
extern int32 saved_film_history_get_target_record_index_by_tick(int32 tick_index);
extern void saved_film_history_initialize();
extern void saved_film_history_initialize_for_saved_film_playback();
extern void saved_film_history_initialize_internal();
extern void saved_film_history_memory_dispose();
extern void saved_film_history_memory_initialize();
extern void saved_film_history_notify_initial_gamestate_loaded();
extern bool saved_film_history_ready_for_revert_or_reset();
extern void saved_film_history_render_debug();
extern bool saved_film_history_revert_by_film_tick(int32 revert_seek_tick);
extern bool saved_film_history_revert_by_index(int32 revert_index);
extern bool saved_film_history_revert_by_type(e_saved_film_revert_type revert_type);
extern bool saved_film_history_revert_internal(int32 target_record_index);
extern bool saved_film_history_should_flush_gamestate(int32 update_number);
extern bool saved_film_history_time_for_chapter_archive(int32 film_tick);
extern void saved_film_history_update();
extern void saved_film_history_update_after_simulation_update(const struct simulation_update* update, const s_simulation_update_metadata* metadata);
extern void saved_film_history_update_before_simulation_update(bool disable_adding_history_records);

