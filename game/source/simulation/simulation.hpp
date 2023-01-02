#pragma once

#include "cseries/cseries.hpp"
#include "game/player_control.hpp"
#include "game/players.hpp"
#include "replication/replication_encoding.hpp"
#include "simulation/simulation_queue.hpp"
#include "units/units.hpp"

class c_wait_for_render_thread
{
public:
	c_wait_for_render_thread(char const* file, long line);
	~c_wait_for_render_thread();

protected:
	dword m_flags;
};

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

struct simulation_machine_update
{
	dword machine_valid_mask;
	c_static_array<s_machine_identifier, 17> machine_identifiers;
};
static_assert(sizeof(simulation_machine_update) == 0x114);

struct s_simulation_camera_update
{
	long camera_type;
	long player_absolute_index;
	real_point3d position;
	vector3d forward;
	bool zoomed;
};
static_assert(sizeof(s_simulation_camera_update) == 0x24);

struct simulation_update
{
	long update_number;
	word_flags flags;
	dword_flags player_flags;

	dword_flags action_test_flags;
	c_static_array<player_action, 16> player_actions;

	dword valid_actor_mask;
	c_static_array<long, 16> actor_unit_indices;
	c_static_array<unit_control_data, 16> actor_control;

	bool machine_update_exists;
	simulation_machine_update machine_update;

	dword valid_player_prediction_mask;
	s_player_prediction player_prediction[16];

	long verify_game_time;
	long verify_random;

	//s_determinism_verification determinism_verification;

	dword valid_camera_mask;
	c_static_array<s_simulation_camera_update, 1> camera_update;

	c_simulation_queue bookkeeping_simulation_queue;
	c_simulation_queue game_simulation_queue;
};
static_assert(sizeof(simulation_update) == 0x1658);

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

	// used in `simulation_update_write_to_buffer`
	simulation_update update;
	dword_flags update_flags;
};
static_assert(sizeof(s_simulation_globals) == 0x1784);

extern s_simulation_globals& simulation_globals;

extern bool __cdecl simulation_starting_up();

