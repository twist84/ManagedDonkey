#include "networking/logic/network_session_interface.hpp"

#include "cseries/console.hpp"
#include "networking/logic/network_life_cycle.hpp"
#include "networking/session/network_session.hpp"
#include "networking/session/network_session_parameter_type_collection.hpp"

#include <assert.h>

REFERENCE_DECLARE(0x019A0328, s_network_session_interface_globals, session_interface_globals);

long __cdecl network_squad_session_get_countdown_timer()
{
	c_network_session* active_squad_session = nullptr;
	if (network_life_cycle_in_squad_session(&active_squad_session) && active_squad_session->established())
	{
		c_network_session_parameters* parameters = active_squad_session->get_session_parameters();
		c_network_session_parameter_countdown_timer* countdown_timer = 
			static_cast<c_network_session_parameter_countdown_timer*>(parameters->m_parameter_interfaces[_network_session_parameter_type_countdown_timer]);
		return countdown_timer->get_countdown_timer();
	}

    return -1;
}

e_gui_game_mode __cdecl network_life_cycle_squad_session_get_ui_game_mode()
{
	return INVOKE(0x00435640, network_life_cycle_squad_session_get_ui_game_mode);
}

void __cdecl network_session_interface_handle_message(long session_network_message)
{
	INVOKE(0x004365D0, network_session_interface_handle_message, session_network_message);
}

bool __cdecl network_squad_session_can_set_game_settings()
{
	//return INVOKE(0x00438590, network_squad_session_can_set_game_settings);

	c_network_session* in_squad_session = nullptr;
	return network_life_cycle_in_squad_session(&in_squad_session)
		&& in_squad_session->established()
		&& in_squad_session->is_leader()
		&& in_squad_session->session_mode() == _network_session_mode_idle;
}

bool __cdecl network_squad_session_set_campaign_difficulty(e_campaign_difficulty_level campaign_difficulty)
{
	return INVOKE(0x00439510, network_squad_session_set_campaign_difficulty, campaign_difficulty);
}

bool __cdecl network_squad_session_set_campaign_insertion_point(short campaign_insertion_point)
{
	return INVOKE(0x00439580, network_squad_session_set_campaign_insertion_point, campaign_insertion_point);
}

bool __cdecl network_squad_session_set_closed_by_user(bool closed_by_user)
{
	return INVOKE(0x00439660, network_squad_session_set_closed_by_user, closed_by_user);
}

//bool __cdecl network_squad_session_set_closed_status(enum e_network_session_closed_status closed_status)
bool __cdecl network_squad_session_set_closed_status(long closed_status)
{
	return INVOKE(0x004396B0, network_squad_session_set_closed_status, closed_status);
}

//bool __cdecl network_squad_session_set_coop_game_options(e_campaign_id campaign_id, e_map_id map_id, char const* scenario_path)
bool __cdecl network_squad_session_set_coop_game_options(long campaign_id, long map_id, char const* scenario_path)
{
	return INVOKE(0x004396F0, network_squad_session_set_coop_game_options, campaign_id, map_id, scenario_path);
}

bool __cdecl network_squad_session_set_film(s_saved_film_description const* film)
{
	return INVOKE(0x00439800, network_squad_session_set_film, film);
}

bool __cdecl network_squad_session_set_game_variant(c_game_variant const* game_variant)
{
	return INVOKE(0x00439860, network_squad_session_set_game_variant, game_variant);
}

//bool network_squad_session_set_map(e_campaign_id campaign_id, e_map_id map_id, char const* scenario_path)
bool __cdecl network_squad_session_set_map(long campaign_id, long map_id, char const* scenario_path)
{
	return INVOKE(0x00439A00, network_squad_session_set_map, campaign_id, map_id, scenario_path);
}

bool __cdecl network_squad_session_set_map_variant(c_map_variant const* map_variant)
{
	return INVOKE(0x00439AB0, network_squad_session_set_map_variant, map_variant);
}

bool __cdecl network_squad_session_set_maximum_player_count(long maximum_player_count)
{
	return INVOKE(0x00439BA0, network_squad_session_set_maximum_player_count, maximum_player_count);
}

//bool __cdecl network_squad_session_set_privacy_mode(e_network_game_privacy privacy_mode)
bool __cdecl network_squad_session_set_privacy_mode(long privacy_mode)
{
	return INVOKE(0x00439C60, network_squad_session_set_privacy_mode, privacy_mode);
}

bool __cdecl network_squad_session_set_simulation_protocol(e_network_game_simulation_protocol simulation_protocol)
{
	return INVOKE(0x00439CB0, network_squad_session_set_simulation_protocol, simulation_protocol);
}

bool __cdecl network_squad_session_set_start_mode(e_network_game_start_mode start_mode)
{
	return INVOKE(0x00439D00, network_squad_session_set_start_mode, start_mode);
}

bool __cdecl network_squad_session_set_ui_game_mode(e_gui_game_mode ui_game_mode)
{
	return INVOKE(0x00439D50, network_squad_session_set_ui_game_mode, ui_game_mode);
}

bool __cdecl network_squad_session_set_ui_state(s_network_ui_state const* ui_state)
{
	return INVOKE(0x00439DA0, network_squad_session_set_ui_state, ui_state);
}

bool __cdecl network_squad_session_whack_to_offline_session_class()
{
	return INVOKE(0x00439DF0, network_squad_session_whack_to_offline_session_class);
}

bool __cdecl network_squad_session_local_peer_is_leader()
{
	return INVOKE(0x00455320, network_squad_session_local_peer_is_leader);
}

char const* k_network_session_mode_names[k_network_session_mode_count]
{
	"none",
	"idle",
	"setup",
	"in_game",
	"end_game",
	"post_game",
	"matchmaking_start",
	"matchmaking_searching",
	"matchmaking_gathering",
	"matchmaking_slave",
	"matchmaking_disbanding",
	"matchmaking_arbitrating",
	"matchmaking_choosing_game"
};

char const* network_session_mode_get_name(long session_mode)
{
	if (session_mode < _network_session_mode_none || session_mode >= k_network_session_mode_count)
		return "<invalid 'network_session_mode'>";

	return k_network_session_mode_names[session_mode];
}

bool __cdecl network_squad_session_set_session_mode(e_network_session_mode session_mode)
{
	bool success = false;
	//if (network_squad_session_can_set_game_settings())
	if (network_squad_session_local_peer_is_leader())
	{
		c_network_session* in_squad_session = nullptr;
		if (network_life_cycle_in_squad_session(&in_squad_session))
		{
			assert(in_squad_session);

			success = in_squad_session->get_session_parameters()->m_parameters_internal.session_mode.request_change(session_mode);
			assert(success);
		}
	}

	return success;
}

