#include "game/game_globals.hpp"

#include "cseries/cseries.hpp"
#include "game/game_time.hpp"
#include "memory/thread_local.hpp"

#include <cassert>

namespace
{
	void(__cdecl* game_state_prepare_for_revert)() = reinterpret_cast<decltype(game_state_prepare_for_revert)>(0x00510040);
}

game_globals_storage* game_globals_get()
{
	s_thread_local_storage* tls = get_tls();
	if (!tls || !tls->game_globals)
		return nullptr;

	return tls->game_globals;
}

bool game_is_available(void)
{
	game_globals_storage* game_globals = game_globals_get();

	return game_globals && game_globals->map_active && game_globals->active_structure_bsp_mask;
}

s_game_cluster_bit_vectors* game_get_cluster_pvs()
{
	game_globals_storage* game_globals = game_globals_get();
	assert(game_globals && game_globals->map_active && game_globals->active_structure_bsp_mask != 0);

	return &game_globals->cluster_pvs;
}

s_game_cluster_bit_vectors* game_get_cluster_pvs_local()
{
	game_globals_storage* game_globals = game_globals_get();
	assert(game_globals && game_globals->map_active && game_globals->active_structure_bsp_mask != 0);

	return &game_globals->cluster_pvs_local;
}

s_game_cluster_bit_vectors* game_get_cluster_activation()
{
	game_globals_storage* game_globals = game_globals_get();
	assert(game_globals && game_globals->map_active && game_globals->active_structure_bsp_mask != 0);

	return &game_globals->cluster_activation;
}

// bool game_test_cluster_activation(s_cluster_reference* cluster_reference)

void game_pvs_enable_scripted_camera_pvs()
{
	game_globals_storage* game_globals = game_globals_get();
	if (game_globals)
		game_globals->scripted_camera_pvs = true;
}

void game_pvs_clear_scripted_camera_pvs()
{
	game_globals_storage* game_globals = game_globals_get();
	if (game_globals)
		game_globals->scripted_camera_pvs = false;
}

void game_pvs_scripted_set_object(datum_index object_index)
{
	game_globals_storage* game_globals = game_globals_get();
	if (!game_globals)
		return;

	if (object_index != -1)
	{
		game_globals->scripted = 1;
		game_globals->scripted_object_index = object_index;
	}
	game_globals->scripted = 0;
}

// void game_pvs_scripted_set_camera_point(short camera_point_index)

void game_pvs_scripted_clear()
{
	game_globals_storage* game_globals = game_globals_get();
	if (game_globals)
		game_globals->scripted = 0;
}

// void game_update_pvs()
// void game_won()
// bool game_is_won()

void game_lost(bool game_revert)
{
	game_globals_storage* game_globals = game_globals_get();
	assert(game_globals && game_globals->map_active);

	game_globals->game_revert = game_revert;
	if (game_revert)
	{
		if (!game_globals->game_lost)
		{
			// seconds_to_wait:
			// - halo 3: 5.0f
			// - halo reach: 6.0f
			// - halo online: 6.0f
			real seconds_to_wait = 5.0f;

			game_globals->game_lost = true;
			game_globals->game_lost_wait_time = game_seconds_to_ticks_round(seconds_to_wait);
			game_state_prepare_for_revert();
		}
	}
	else
	{
		if (game_globals->game_lost && !game_globals->game_revert)
			game_globals->game_lost = false;
	}
}

bool game_is_lost()
{
	game_globals_storage* game_globals = game_globals_get();
	assert(game_globals && game_globals->map_active);

	return game_globals->game_lost;
}

// custom like `game_is_finished_immediate`
bool game_is_lost_immediate()
{
	game_globals_storage* game_globals = game_globals_get();
	assert(game_globals && game_globals->map_active);

	return game_globals->game_lost && !game_globals->game_lost_wait_time;
}

void game_finish()
{
	game_globals_storage* game_globals = game_globals_get();
	assert(game_globals && game_globals->map_active);

	if (!game_globals->game_finished)
	{
		// seconds_to_wait:
		// - halo 3: 7.0f
		// - halo reach: game_is_campaign_or_survival() ? 1.0f : 7.0f
		// - halo online: 40.0f
		real seconds_to_wait = 7.0f;

		game_globals->game_finished = true;
		game_globals->game_finished_wait_time = game_seconds_to_ticks_round(seconds_to_wait);

		// halo online
		//if (!game_is_playback())
		//	game_sound_disable_at_game_finish();
	}
}

void game_finish_immediate()
{
	game_globals_storage* game_globals = game_globals_get();
	assert(game_globals && game_globals->map_active);

	if (!game_globals->game_finished)
	{
		game_finish();
		game_globals->game_finished_wait_time = 0;
	}
}

bool game_is_finished()
{
	game_globals_storage* game_globals = game_globals_get();
	assert(game_globals && game_globals->map_active);

	return game_globals->game_finished;
}

bool game_is_finished_immediate()
{
	game_globals_storage* game_globals = game_globals_get();
	assert(game_globals && game_globals->map_active);

	return game_globals->game_finished && !game_globals->game_finished_wait_time;
}