#include "saved_games/saved_film_history.hpp"

#include "game/game.hpp"
#include "rasterizer/rasterizer.hpp"
#include "saved_games/game_state_procs.hpp"
#include "saved_games/saved_film.hpp"
#include "saved_games/saved_film_manager.hpp"
#include "text/draw_string.hpp"

s_saved_film_history_globals saved_film_history_globals{};

const char* const k_saved_film_history_file_path = "sf_history.blob";
const real32 k_saved_film_history_double_tap_seconds = 0.5f;

//c_saved_film_history_record_manager::c_saved_film_history_record_manager()
//int32 c_saved_film_history_record_manager::chapter_count()
//void c_saved_film_history_record_manager::clear_window_records()
//void c_saved_film_history_record_manager::commit_working_record()
//int32 c_saved_film_history_record_manager::evict_oldest_local_record()
//int32 c_saved_film_history_record_manager::find_record_by_film_tick(int32 film_tick)
//int32 c_saved_film_history_record_manager::get_closest_record_greater_than_tick(int32 film_tick)
//int32 c_saved_film_history_record_manager::get_closest_record_less_than_tick(int32 film_tick, bool chapters_only)
//s_saved_film_history_archive_record* c_saved_film_history_record_manager::get_current_working_record()
//int32 c_saved_film_history_record_manager::get_current_working_record_index()
//s_saved_film_history_archive_record* c_saved_film_history_record_manager::get_new_working_record(bool chapter)
//s_saved_film_history_archive_record* c_saved_film_history_record_manager::get_record(int32 record_index)
//void c_saved_film_history_record_manager::initialize()
//void c_saved_film_history_record_manager::initialize_record(int32 record_index)
//int32 c_saved_film_history_record_manager::local_count()
//bool c_saved_film_history_record_manager::valid(int32 record_index)
//void c_saved_film_history_record_manager::validate()

//s_saved_film_history_globals::s_saved_film_history_globals()
//c_async_stored_buffer_set<1>* saved_film_history_buffer_acquire()
//c_async_stored_buffer_set<1>* saved_film_history_buffer_get_without_acquire()

void saved_film_history_buffer_release()
{
	//c_saved_film_scratch_memory::get()->release(_system_film_history);
}

bool saved_film_history_can_revert_by_type(e_saved_film_revert_type revert_type)
{
	return saved_film_history_get_target_record_index_by_revert_type(revert_type) != NONE;
}

void saved_film_history_dispose_from_saved_film_playback()
{
	saved_film_history_initialize_internal();
}

void saved_film_history_flush_game_state()
{
	game_state_call_before_save_procs(_use_insecure_signature_flag);
	game_state_call_after_save_procs(_use_insecure_signature_flag);
}

void saved_film_history_get_hud_interface_state(s_saved_film_hud_interface_state* state)
{
	// $TODO: implement me properly

	// stop-gap `saved_film_history_get_hud_interface_state` implementation
	if (saved_film_manager_is_reading())
	{
		int32 length_in_ticks = saved_film_manager_globals.saved_film.get_length_in_ticks();
		int32 current_tick = saved_film_manager_globals.saved_film.get_current_tick();

		state->duration_in_seconds = game_ticks_to_seconds((real32)length_in_ticks);
		state->marker_position_in_seconds = game_ticks_to_seconds((real32)current_tick);

		real32 theta = length_in_ticks <= 0 ? 0.0f : state->marker_position_in_seconds / state->duration_in_seconds;

		state->current_position_theta = theta;
		state->buffered_theta = theta;
	}
}

int32 saved_film_history_get_target_record_index_by_revert_type(e_saved_film_revert_type revert_type)
{
	// $TODO: implement me
	return NONE;
}

int32 saved_film_history_get_target_record_index_by_tick(int32 tick_index)
{
	// $TODO: implement me
	return NONE;
}

void saved_film_history_initialize()
{
	saved_film_history_globals.history_buffer = NULL;
	saved_film_history_initialize_internal();
}

void saved_film_history_initialize_for_saved_film_playback()
{
	saved_film_history_initialize_internal();
	saved_film_history_globals.estimated_length_in_ticks = game_options_get()->playback_length_in_ticks;
}

void saved_film_history_initialize_internal()
{
	//saved_film_history_globals.record_manager.initialize();
	saved_film_history_globals.estimated_length_in_ticks = 0;
	saved_film_history_globals.last_revert_time = system_milliseconds();
	saved_film_history_globals.reverted_last_tick = false;
}

void saved_film_history_memory_dispose()
{
	// $TODO: implement me
}

void saved_film_history_memory_initialize()
{
	// $TODO: implement me
}

void saved_film_history_notify_initial_gamestate_loaded()
{
	// $TODO: implement me
}

bool saved_film_history_ready_for_revert_or_reset()
{
	// $TODO: implement me

	return false;
}

void saved_film_history_render_debug()
{
	if (!game_is_playback() || !saved_film_manager_timestamp_enabled_internal())
	{
		return;
	}

	s_saved_film_hud_interface_state hud_state{};
	saved_film_manager_get_hud_interface_state(&hud_state);

	real_point3d box_points[4]
	{
		{ 100.0f, 100.0f, 0.0f },
		{ 300.0f, 100.0f, 0.0f },
		{ 300.0f, 125.0f, 0.0f },
		{ 100.0f, 125.0f, 0.0f }
	};

	c_rasterizer::draw_debug_line2d(box_points[0], box_points[1], 0xFFFFFFFF, 0xFFFFFFFF);
	c_rasterizer::draw_debug_line2d(box_points[1], box_points[2], 0xFFFFFFFF, 0xFFFFFFFF);
	c_rasterizer::draw_debug_line2d(box_points[2], box_points[3], 0xFFFFFFFF, 0xFFFFFFFF);
	c_rasterizer::draw_debug_line2d(box_points[3], box_points[0], 0xFFFFFFFF, 0xFFFFFFFF);

	box_points[1].y = 105.0f;
	box_points[0].y = 105.0f;
	box_points[2].y = 120.0f;
	box_points[3].y = 120.0f;

	box_points[1].x = 100.0f + (200.0f * hud_state.buffered_theta);
	box_points[2].x = 100.0f + (200.0f * hud_state.buffered_theta);

	if (hud_state.recording)
	{
		box_points[3].x = 100.0f + (200.0f * hud_state.recording_start_theta);
		box_points[0].x = box_points[3].x;
		box_points[2].x = 100.0f + (200.0f * hud_state.current_position_theta);
		box_points[1].x = box_points[2].x;

		c_rasterizer::draw_debug_line2d(box_points[0], box_points[1], 0xFFFF0000, 0xFFFF0000);
		c_rasterizer::draw_debug_line2d(box_points[1], box_points[2], 0xFFFF0000, 0xFFFF0000);
		c_rasterizer::draw_debug_line2d(box_points[2], box_points[3], 0xFFFF0000, 0xFFFF0000);
		c_rasterizer::draw_debug_line2d(box_points[3], box_points[0], 0xFFFF0000, 0xFFFF0000);
	}

	real_point3d triangle_points[3]
	{
		{  100.0f + (200.0f * hud_state.current_position_theta),         125.0f,        1.0f },
		{ (100.0f + (200.0f * hud_state.current_position_theta)) + 5.0f, 125.0f + 5.0f, 1.0f },
		{ (100.0f + (200.0f * hud_state.current_position_theta)) - 5.0f, 125.0f + 5.0f, 1.0f },
	};

	c_rasterizer::draw_debug_line2d(triangle_points[0], triangle_points[1], 0xFFFFFFFF, 0xFFFFFFFF);
	c_rasterizer::draw_debug_line2d(triangle_points[1], triangle_points[2], 0xFFFFFFFF, 0xFFFFFFFF);
	c_rasterizer::draw_debug_line2d(triangle_points[2], triangle_points[0], 0xFFFFFFFF, 0xFFFFFFFF);

	c_rasterizer_draw_string draw_string{};
	c_font_cache_mt_safe font_cache{};

	rectangle2d rectangle{};
	set_rectangle2d(&rectangle, (int16)triangle_points[0].x, (int16)(triangle_points[0].y + 25.0f), 1000, 1000);
	draw_string.set_bounds(&rectangle);

	char text[128]{};
	draw_string.draw(&font_cache, text);
}

bool saved_film_history_revert_by_film_tick(int32 revert_seek_tick)
{
	ASSERT(game_is_playback());
	ASSERT(game_in_progress());

	int32 target_record_index = saved_film_history_get_target_record_index_by_tick(revert_seek_tick);
	if (target_record_index == NONE)
	{
		event(_event_warning, "saved_film:history: failed to get record index for seek index %d",
			revert_seek_tick);
		return false;
	}

	event(_event_message, "networking:saved_film:history: reverting by film tick [%d]",
		revert_seek_tick);
	return saved_film_history_revert_internal(target_record_index);
}

bool saved_film_history_revert_by_index(int32 revert_index)
{
	ASSERT(game_is_playback());
	ASSERT(game_in_progress());

	//if (!saved_film_history_globals.record_manager.valid())
	if (!saved_film_history_globals.record_manager.m_archive_records[revert_index].valid)
	{
		event(_event_warning, "saved_film:history: revert index %d record is not valid",
			revert_index);
		return false;
	}

	event(_event_message, "networking:saved_film:history: reverting by index [%d]",
		revert_index);
	return saved_film_history_revert_internal(revert_index);
}

bool saved_film_history_revert_by_type(e_saved_film_revert_type revert_type)
{
	ASSERT(game_is_playback());
	ASSERT(game_in_progress());

	int32 target_record_index = saved_film_history_get_target_record_index_by_revert_type(revert_type);
	if (target_record_index == NONE)
	{
		event(_event_warning, "saved_film:history: failed to get record index for revert type %d",
			revert_type);
		return false;
	}

	event(_event_message, "networking:saved_film:history: reverting by type [%d]",
		revert_type);

	return saved_film_history_revert_internal(target_record_index);
}

bool saved_film_history_revert_internal(int32 target_record_index)
{
	// $TODO: implement me
	return false;
}

bool saved_film_history_should_flush_gamestate(int32 update_number)
{
	return update_number == 300 * (update_number / 300);
}

bool saved_film_history_time_for_chapter_archive(int32 film_tick)
{
	// $TODO: implement me
	return false;
}

void saved_film_history_update()
{
	// $TODO: implement me
	if (!saved_film_history_globals.history_buffer || saved_film_history_globals.history_buffer->is_async_io_in_progress())
	{
		return;
	}

	event(_event_message, "networking:saved_film:history: releasing the history buffer");

	saved_film_history_buffer_release();
	saved_film_history_globals.history_buffer = NULL;
}

void saved_film_history_update_after_simulation_update(const struct simulation_update* update, const s_simulation_update_metadata * metadata)
{
	//ASSERT(update);
	//ASSERT(metadata);
	//
	//if (!game_in_progress() || !game_is_multiplayer() || !game_is_playback())
	//{
	//	return;
	//}
	//
	//int32 current_working_record_index = c_saved_film_history_record_manager::get_current_working_record_index(&saved_film_history_globals.record_manager);
	//if (current_working_record_index != NONE)
	//{
	//	s_saved_film_history_archive_record* current_working_record = saved_film_history_globals.record_manager.get_current_working_record();
	//	ASSERT(metadata->flags.test(_simulation_update_from_saved_film_bit));
	//
	//	current_working_record->film_file_position = metadata->saved_film_position;
	//	current_working_record->film_tick = metadata->saved_film_tick;
	//	current_working_record->update_number = update->update_number;
	//
	//	saved_film_history_globals.record_manager.commit_working_record();
	//
	//	event(_event_message, "networking:saved_film:history: building game state history %d post-update [film position %d tick %d update %d]",
	//		current_working_record_index,
	//		current_working_record->film_file_position,
	//		current_working_record->film_tick,
	//		current_working_record->update_number);
	//}
	//
	//saved_film_history_globals.reverted_last_tick = false;
}

void saved_film_history_update_before_simulation_update(bool disable_adding_history_records)
{
	// $TODO: implement me
}

