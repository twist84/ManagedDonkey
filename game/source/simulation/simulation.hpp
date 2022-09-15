#pragma once

#include "cseries/cseries.hpp"

enum e_simulation_abort_reason
{
	_simulation_abort_reason_exiting_in_game = 0,
	_simulation_abort_reason_exiting_in_match,
	_simulation_abort_reason_failed_to_find_next_map,
	_simulation_abort_reason_playing_film,
	_simulation_abort_reason_reset_failed,
	_simulation_abort_reason_fatal_error,
	_simulation_abort_reason_lost_connection,
	_simulation_abort_reason_out_of_sync,
	_simulation_abort_reason_film_ended,
	_simulation_abort_reason_film_playback_error,

	k_simulation_abort_reason_count
};

struct c_simulation_world;
struct c_simulation_watcher;
struct c_simulation_type_collection;
struct s_simulation_globals
{
	bool initialized;
	bool fatal_error;
	byte __unknown2;
	bool aborted;

	dword network_time_since_abort;

	c_enum<e_simulation_abort_reason, long, k_simulation_abort_reason_count> abort_reason;

	bool simulation_reset;
	bool simulation_reset_in_progress;
	byte __unknownE;

	bool simulation_prepare_to_load_saved_game;

	bool recording_film;
	byte __unknown11;
	byte __unknown12;
	byte __unknown13;

	c_simulation_world* world;
	c_simulation_watcher* watcher;
	c_simulation_type_collection* type_collection;

	dword __unknown20;

	char simulation_status[256];

	byte simulation_paused;
	byte __unknown125;
	byte __unknown126;
	byte __unknown127;
};
static_assert(sizeof(s_simulation_globals) == 0x128);

extern s_simulation_globals& simulation_globals;