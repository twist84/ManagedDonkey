#include "saved_games/saved_film_history.hpp"

#include "game/game.hpp"
#include "interface/chud/chud.hpp"
#include "rasterizer/rasterizer.hpp"
#include "saved_games/game_state_procs.hpp"
#include "saved_games/saved_film.hpp"
#include "saved_games/saved_film_manager.hpp"
#include "saved_games/saved_film_scratch_memory.hpp"
#include "text/draw_string.hpp"

s_saved_film_history_globals saved_film_history_globals{};

const char* const k_saved_film_history_file_path = "sf_history.blob";
const real32 k_saved_film_history_double_tap_seconds = 0.5f;

c_saved_film_history_record_manager::c_saved_film_history_record_manager()
{
	c_saved_film_history_record_manager::initialize();
}

int32 c_saved_film_history_record_manager::chapter_count()
{
	int32 count = 0;
	for (int32 record_index = 0; record_index < m_archive_records.get_count(); record_index++)
	{
		const s_saved_film_history_archive_record* record = &m_archive_records[record_index];
		if (record->valid && record->chapter)
		{
			count++;
		}
	}
	return count;
}

void c_saved_film_history_record_manager::clear_window_records()
{
	event(_event_message, "clearing window records");

	c_saved_film_history_record_manager::validate();

	for (int32 record_index = 0; record_index < m_archive_records.get_count(); record_index++)
	{
		s_saved_film_history_archive_record* record = c_saved_film_history_record_manager::get_record(record_index);
		if (record->valid && record->in_window)
		{
			if (record->chapter)
			{
				record->in_window = false;
			}
			else
			{
				c_saved_film_history_record_manager::initialize_record(record_index);
			}
		}
	}
}

void c_saved_film_history_record_manager::commit_working_record()
{
	int32 current_working_record_index = c_saved_film_history_record_manager::get_current_working_record_index();
	ASSERT(current_working_record_index != NONE);
	c_saved_film_history_record_manager::validate();
	m_archive_records[current_working_record_index].working = false;
	m_archive_records[current_working_record_index].valid = true;
}

int32 c_saved_film_history_record_manager::evict_oldest_local_record()
{
	int32 oldest_local_record_index = NONE;
	int32 oldest_tick = 0;
	for (int32 record_index = 0; record_index < m_archive_records.get_count(); record_index++)
	{
		s_saved_film_history_archive_record* current_record = &m_archive_records[record_index];
		if (current_record->valid && !current_record->chapter)
		{
			if (oldest_local_record_index == NONE || current_record->film_tick < oldest_tick)
			{
				oldest_local_record_index = record_index;
				oldest_tick = current_record->film_tick;
			}
		}
	}
	ASSERT(oldest_local_record_index != NONE);
	s_saved_film_history_archive_record* oldest_local_record = &m_archive_records[oldest_local_record_index];
	for (int32 record_index = 0; record_index < m_archive_records.get_count(); record_index++)
	{
		s_saved_film_history_archive_record* current_record = &m_archive_records[record_index];
		if (current_record->valid && current_record->film_tick < oldest_local_record->film_tick)
		{
			current_record->in_window = false;
		}
	}
	return oldest_local_record_index;
}

int32 c_saved_film_history_record_manager::find_record_by_film_tick(int32 film_tick)
{
	int32 found_record_index = NONE;
	c_saved_film_history_record_manager::validate();
	for (int32 record_index = 0; record_index < m_archive_records.get_count(); record_index++)
	{
		const s_saved_film_history_archive_record* current_record = &m_archive_records[record_index];
		if (current_record->valid && current_record->film_tick == film_tick)
		{
			ASSERT(found_record_index == NONE);
			found_record_index = record_index;
		}
	}
	return found_record_index;
}

int32 c_saved_film_history_record_manager::get_closest_record_greater_than_tick(int32 film_tick)
{
	int32 closest_record_index = NONE;
	int32 closest_tick_index = 0;
	c_saved_film_history_record_manager::validate();
	for (int32 record_index = 0; record_index < m_archive_records.get_count(); record_index++)
	{
		const s_saved_film_history_archive_record* record = &m_archive_records[record_index];
		if (record->valid && record->film_tick > film_tick)
		{
			if (closest_record_index == NONE ||
				record->film_tick - film_tick < closest_tick_index - film_tick)
			{
				closest_record_index = record_index;
				closest_tick_index = record->film_tick;
			}
		}
	}
	return closest_record_index;
}

int32 c_saved_film_history_record_manager::get_closest_record_less_than_tick(int32 film_tick, bool chapters_only)
{
	int32 closest_record_index = NONE;
	int32 closest_tick_index = 0;
	c_saved_film_history_record_manager::validate();
	for (int32 record_index = 0; record_index < m_archive_records.get_count(); record_index++)
	{
		const s_saved_film_history_archive_record* record = &m_archive_records[record_index];
		if (record->valid && record->film_tick <= film_tick)
		{
			if (closest_record_index == NONE ||
				film_tick - record->film_tick < film_tick - closest_tick_index &&
				(!chapters_only || record->chapter))
			{
				closest_record_index = record_index;
				closest_tick_index = record->film_tick;
			}
		}
	}
	return closest_record_index;
}

s_saved_film_history_archive_record* c_saved_film_history_record_manager::get_current_working_record()
{
	int32 current_working_record_index = c_saved_film_history_record_manager::get_current_working_record_index();
	s_saved_film_history_archive_record* working_record = &m_archive_records[current_working_record_index];
	c_saved_film_history_record_manager::validate();
	return working_record;
}

int32 c_saved_film_history_record_manager::get_current_working_record_index()
{
	int32 current_working_record_index = NONE;
	c_saved_film_history_record_manager::validate();
	for (int32 record_index = 0; record_index < m_archive_records.get_count(); record_index++)
	{
		const s_saved_film_history_archive_record* current_record = &m_archive_records[record_index];
		if (current_record->working)
		{
			ASSERT(current_working_record_index == NONE);
			ASSERT(!current_record->valid);
			current_working_record_index = record_index;
		}
	}
	return current_working_record_index;
}

s_saved_film_history_archive_record* c_saved_film_history_record_manager::get_new_working_record(bool chapter)
{
	int32 new_working_record_index = NONE;
	ASSERT(get_current_working_record_index() == NONE);
	if (chapter)
	{
		ASSERT(chapter_count() <= (k_saved_film_history_chapter_archive_count - 1));
	}
	c_saved_film_history_record_manager::validate();
	if (chapter || c_saved_film_history_record_manager::local_count() != k_saved_film_history_local_archive_count)
	{
		for (int32 record_index = 0; record_index < m_archive_records.get_count(); record_index++)
		{
			const s_saved_film_history_archive_record* current_record = &m_archive_records[record_index];
			if (!current_record->valid)
			{
				new_working_record_index = record_index;
				break;
			}
		}
	}
	if (new_working_record_index == NONE)
	{
		ASSERT(!chapter);
		new_working_record_index = c_saved_film_history_record_manager::evict_oldest_local_record();
	}
	ASSERT(new_working_record_index != NONE);
	c_saved_film_history_record_manager::initialize_record(new_working_record_index);
	s_saved_film_history_archive_record* current_working_record = &m_archive_records[new_working_record_index];
	current_working_record->history_file_position = 0x7F0000 * new_working_record_index;
	current_working_record->chapter = chapter;
	current_working_record->in_window = true;
	current_working_record->working = true;
	c_saved_film_history_record_manager::validate();
	return current_working_record;
}

s_saved_film_history_archive_record* c_saved_film_history_record_manager::get_record(int32 record_index)
{
	s_saved_film_history_archive_record* record = &m_archive_records[record_index];
	ASSERT(record->valid);
	c_saved_film_history_record_manager::validate();
	return record;
}

void c_saved_film_history_record_manager::initialize()
{
	for (int32 record_index = 0; record_index < m_archive_records.get_count(); record_index++)
	{
		c_saved_film_history_record_manager::initialize_record(record_index);
	}
}

void c_saved_film_history_record_manager::initialize_record(int32 record_index)
{
	s_saved_film_history_archive_record* record = &m_archive_records[record_index];
	csmemset(record, 0, sizeof(s_saved_film_history_archive_record));
	record->film_file_position = NONE;
	record->history_file_position = NONE;
	record->update_number = NONE;
}

int32 c_saved_film_history_record_manager::local_count()
{
	int32 count = 0;
	for (int32 record_index = 0; record_index < m_archive_records.get_count(); record_index++)
	{
		const s_saved_film_history_archive_record* record = &m_archive_records[record_index];
		if (record->valid && !record->chapter)
		{
			count++;
		}
	}
	return count;
}

bool c_saved_film_history_record_manager::valid(int32 record_index)
{
	return m_archive_records[record_index].valid;
}

void c_saved_film_history_record_manager::validate()
{
	int32 chapter_count = 0;
	int32 local_count = 0;
	int32 working_count = 0;
	for (int32 record_index = 0; record_index < m_archive_records.get_count(); record_index++)
	{
		const s_saved_film_history_archive_record* record = &m_archive_records[record_index];
		if (record->valid)
		{
			if (record->chapter)
			{
				chapter_count++;
			}
			else
			{
				local_count++;
			}
			if (record->working)
			{
				working_count = 1;
			}
		}
	}
	ASSERT(chapter_count <= k_saved_film_history_chapter_archive_count);
	ASSERT(local_count <= k_saved_film_history_local_archive_count);
	ASSERT(working_count == 0 || working_count == 1);
}

c_async_stored_buffer_set<1>* saved_film_history_buffer_acquire()
{
	c_async_stored_buffer_set<1>* history_buffer = nullptr;

	c_saved_film_scratch_memory::s_system_data* system_data = c_saved_film_scratch_memory::get()->try_and_acquire(c_saved_film_scratch_memory::_system_film_history);
	if (system_data)
	{
		history_buffer = &system_data->buffer_set;
	}

	return history_buffer;
}

c_async_stored_buffer_set<1>* saved_film_history_buffer_get_without_acquire()
{
	c_async_stored_buffer_set<1>* history_buffer = saved_film_history_buffer_acquire();
	saved_film_history_buffer_release();
	return history_buffer;
}

void saved_film_history_buffer_release()
{
	c_saved_film_scratch_memory::get()->release(c_saved_film_scratch_memory::_system_film_history);
}

bool saved_film_history_can_revert_by_type(e_saved_film_revert_type revert_type)
{
	int32 target_record_index = saved_film_history_get_target_record_index_by_revert_type(revert_type);
	return target_record_index != NONE;
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
	int32 current_tick = saved_film_manager_get_current_tick_estimate();

	switch (revert_type)
	{
	case _saved_film_revert_backwards:
		return saved_film_history_globals.record_manager.get_closest_record_less_than_tick(current_tick, false);
	case _saved_film_revert_forwards:
		return saved_film_history_globals.record_manager.get_closest_record_greater_than_tick(current_tick);
	case _saved_film_revert_none:
	default:
		return NONE;
	}
}

int32 saved_film_history_get_target_record_index_by_tick(int32 tick_index)
{
	return saved_film_history_globals.record_manager.get_closest_record_less_than_tick(tick_index, false);
}

void saved_film_history_initialize()
{
	saved_film_history_globals.history_buffer = nullptr;
	saved_film_history_initialize_internal();
}

void saved_film_history_initialize_for_saved_film_playback()
{
	saved_film_history_initialize_internal();
	saved_film_history_globals.estimated_length_in_ticks = game_options_get()->playback_length_in_ticks;
}

void saved_film_history_initialize_internal()
{
	saved_film_history_globals.record_manager.initialize();
	saved_film_history_globals.estimated_length_in_ticks = 0;
	saved_film_history_globals.last_revert_time = system_milliseconds();
	saved_film_history_globals.reverted_last_tick = false;
}

void saved_film_history_memory_dispose()
{
	if (c_saved_film_scratch_memory::get()->valid())
	{
		c_saved_film_scratch_memory::get()->block_until_not_busy();
		saved_film_history_buffer_get_without_acquire()->close_file();
	}

	if (saved_film_history_globals.history_buffer)
	{
		saved_film_history_globals.history_buffer->block_until_not_busy();
		saved_film_history_buffer_release();
		saved_film_history_globals.history_buffer = nullptr;
	}
}

void saved_film_history_memory_initialize()
{
	c_static_wchar_string<260> file;
	file.set_char(k_saved_film_history_file_path);

	if (!saved_film_history_buffer_get_without_acquire()->open_file(file.get_string(), _async_buffer_file_access_read_write, _async_buffer_disposition_create_always))
	{
		VASSERT(false, "failed to allocate for saved film history");
		event(_event_critical, "networking:saved_film:history: failed to allocate for saved film history");
	}
}

void saved_film_history_notify_initial_gamestate_loaded()
{
	if (!game_in_progress() || game_playback_get() != _game_playback_film)
	{
		return;
	}

	saved_film_history_globals.record_manager.clear_window_records();

	s_saved_film_history_archive_record* record = saved_film_history_globals.record_manager.get_new_working_record(true);
	record->film_file_position = saved_film_manager_get_position();
	record->film_tick = 0;
	record->update_number = 0;
	saved_film_history_globals.record_manager.commit_working_record();
}

bool saved_film_history_ready_for_revert_or_reset()
{
	bool ready = false;

	c_saved_film_scratch_memory::s_system_data* system_data = c_saved_film_scratch_memory::get()->try_and_acquire(c_saved_film_scratch_memory::_system_film_history);
	if (system_data)
	{
		saved_film_history_buffer_release();
		ready = true;
	}

	return ready;
}

void saved_film_history_render_debug()
{
	if (game_is_playback() && saved_film_manager_timestamp_enabled_internal())
	{
		s_saved_film_hud_interface_state hud_state{};
		saved_film_manager_get_hud_interface_state(&hud_state);

		real_point3d points[4]
		{
			{ 100.0f, 100.0f, 0.0f },
			{ 300.0f, 100.0f, 0.0f },
			{ 300.0f, 125.0f, 0.0f },
			{ 100.0f, 125.0f, 0.0f }
		};

		c_rasterizer::draw_debug_line2d(points[0], points[1], 0xFFFFFFFF, 0xFFFFFFFF);
		c_rasterizer::draw_debug_line2d(points[1], points[2], 0xFFFFFFFF, 0xFFFFFFFF);
		c_rasterizer::draw_debug_line2d(points[2], points[3], 0xFFFFFFFF, 0xFFFFFFFF);
		c_rasterizer::draw_debug_line2d(points[3], points[0], 0xFFFFFFFF, 0xFFFFFFFF);

		points[1].y = 105.0f;
		points[0].y = 105.0f;
		points[2].y = 120.0f;
		points[3].y = 120.0f;

		points[1].x = 100.0f + (200.0f * hud_state.buffered_theta);
		points[2].x = 100.0f + (200.0f * hud_state.buffered_theta);

		if (hud_state.recording)
		{
			points[3].x = 100.0f + (200.0f * hud_state.recording_start_theta);
			points[0].x = points[3].x;
			points[2].x = 100.0f + (200.0f * hud_state.current_position_theta);
			points[1].x = points[2].x;

			c_rasterizer::draw_debug_line2d(points[0], points[1], 0xFFFF0000, 0xFFFF0000);
			c_rasterizer::draw_debug_line2d(points[1], points[2], 0xFFFF0000, 0xFFFF0000);
			c_rasterizer::draw_debug_line2d(points[2], points[3], 0xFFFF0000, 0xFFFF0000);
			c_rasterizer::draw_debug_line2d(points[3], points[0], 0xFFFF0000, 0xFFFF0000);
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

		rectangle2d frame_bounds{};
		set_rectangle2d(&frame_bounds, (int16)triangle_points[0].x, (int16)(triangle_points[0].y + 25.0f), 1000, 1000);
		draw_string.set_bounds(&frame_bounds);

		char buffer[128]{};
		draw_string.draw(&font_cache, buffer);
	}
}

bool saved_film_history_revert_by_film_tick(int32 revert_seek_tick)
{
	ASSERT(game_is_playback());
	ASSERT(game_in_progress());

	bool success = false;
	int32 target_record_index = saved_film_history_get_target_record_index_by_tick(revert_seek_tick);
	if (target_record_index == NONE)
	{
		event(_event_warning, "saved_film:history: failed to get record index for seek index %d",
			revert_seek_tick);
	}
	else
	{
		event(_event_message, "networking:saved_film:history: reverting by film tick [%d]",
			revert_seek_tick);
		success = saved_film_history_revert_internal(target_record_index);
	}
	return success;
}

bool saved_film_history_revert_by_index(int32 revert_index)
{
	ASSERT(game_is_playback());
	ASSERT(game_in_progress());

	bool success = false;
	if (!saved_film_history_globals.record_manager.valid(revert_index))
	{
		event(_event_warning, "saved_film:history: revert index %d record is not valid",
			revert_index);
	}
	else
	{
		event(_event_message, "networking:saved_film:history: reverting by index [%d]",
			revert_index);
		success = saved_film_history_revert_internal(revert_index);
	}
	return success;
}

bool saved_film_history_revert_by_type(e_saved_film_revert_type revert_type)
{
	ASSERT(game_is_playback());
	ASSERT(game_in_progress());

	bool success = false;
	int32 target_record_index = saved_film_history_get_target_record_index_by_revert_type(revert_type);
	if (target_record_index == NONE)
	{
		event(_event_warning, "saved_film:history: failed to get record index for revert type %d",
			revert_type);
	}
	else
	{
		event(_event_message, "networking:saved_film:history: reverting by type [%d]",
			revert_type);

		success = saved_film_history_revert_internal(target_record_index);
	}
	return success;
}

bool saved_film_history_revert_internal(int32 target_record_index)
{
	if (!saved_film_history_globals.record_manager.valid(target_record_index))
	{
		event(_event_warning, "saved_film:history: revert target record %d is not valid",
			target_record_index);
		return false;
	}

	const s_saved_film_history_archive_record* record = saved_film_history_globals.record_manager.get_record(target_record_index);
	if (record->film_file_position == NONE)
	{
		event(_event_warning, "saved_film:history: revert target record %d has no film position",
			target_record_index);
		return false;
	}

	event(_event_message, "networking:saved_film:history: restoring film position %d tick %d (update %d)",
		record->film_file_position,
		record->film_tick,
		record->update_number);

	g_universal_saved_film_tick.set(record->film_tick);

	bool success = saved_film_manager_globals.saved_film.handle_revert(record->film_file_position, record->film_tick);
	if (!success)
	{
		event(_event_warning, "networking:saved_film:history: failed to handle film revert for record %d",
			target_record_index);
		return false;
	}

	saved_film_manager_notify_reverted_gamestate_loaded(target_record_index, record->update_number, nullptr, 0);
	return true;
}

bool saved_film_history_should_flush_gamestate(int32 update_number)
{
	return update_number == 300 * (update_number / 300);
}

bool saved_film_history_time_for_chapter_archive(int32 film_tick)
{
	if (film_tick <= 0)
	{
		return false;
	}

	if (saved_film_history_globals.record_manager.chapter_count() >= k_saved_film_history_chapter_archive_count)
	{
		return false;
	}

	return film_tick % 900 == 0;
}

void saved_film_history_update()
{
	if (saved_film_history_globals.history_buffer && !saved_film_history_globals.history_buffer->is_async_io_in_progress())
	{
		event(_event_message, "networking:saved_film:history: releasing the history buffer");

		saved_film_history_buffer_release();
		saved_film_history_globals.history_buffer = nullptr;
	}
}

void saved_film_history_update_after_simulation_update(const s_simulation_update* update, const s_simulation_update_metadata * metadata)
{
	ASSERT(update);
	ASSERT(metadata);
	
	if (game_in_progress() && game_is_multiplayer() && game_is_playback())
	{
		int32 working_record_index = saved_film_history_globals.record_manager.get_current_working_record_index();
		if (working_record_index != NONE)
		{
			s_saved_film_history_archive_record* working_record = saved_film_history_globals.record_manager.get_current_working_record();
			ASSERT(metadata->flags.test(_simulation_update_from_saved_film_bit));

			working_record->film_file_position = metadata->saved_film_position;
			working_record->film_tick = metadata->saved_film_tick;
			working_record->update_number = update->update_number;

			saved_film_history_globals.record_manager.commit_working_record();

			event(_event_message, "networking:saved_film:history: building game state history %d post-update [film position %d tick %d update %d]",
				working_record_index,
				working_record->film_file_position,
				working_record->film_tick,
				working_record->update_number);
		}

		saved_film_history_globals.reverted_last_tick = false;
	}
}

void saved_film_history_update_before_simulation_update(bool disable_adding_history_records)
{
	if (!game_in_progress() || game_playback_get() != _game_playback_film || disable_adding_history_records)
	{
		return;
	}

	int32 current_tick = saved_film_manager_get_current_tick_estimate();
	if (current_tick < 0)
	{
		return;
	}

	if (saved_film_history_time_for_chapter_archive(current_tick))
	{
		s_saved_film_history_archive_record* record = saved_film_history_globals.record_manager.get_new_working_record(true);
		record->film_file_position = saved_film_manager_get_position();
		record->film_tick = current_tick;
		record->update_number = NONE;
		saved_film_history_globals.record_manager.commit_working_record();
	}

	if (saved_film_history_should_flush_gamestate(current_tick))
	{
		saved_film_history_flush_game_state();
	}
}

