#pragma once

#include "cseries/language.hpp"
#include "game/game_progression.hpp"
#include "networking/session/network_session_parameters_session.hpp"
#include "networking/session/network_session_parameters_game.hpp"
#include "networking/session/network_session_parameters_game_variant.hpp"
#include "networking/session/network_session_parameters_map_variant.hpp"
#include "networking/session/network_session_parameters_ui.hpp"
#include "shell/shell.hpp"

struct s_network_session_parameter_map
{
	long campaign_id;
	long map_id;
	c_static_string<128> scenario_path;
	s_campaign_armaments campaign_armaments;
	s_campaign_game_progression campaign_progression;
	s_hub_progression hub_progression;
};
static_assert(sizeof(s_network_session_parameter_map) == 0x200);

enum e_network_ui_state_flags
{
	_network_ui_state_flag_game_setup_changing_bit = 0,

	k_network_ui_state_flag_count
};

struct s_network_ui_state
{
	c_flags<e_network_ui_state_flags, word, k_network_ui_state_flag_count> flags;
};
static_assert(sizeof(s_network_ui_state) == 0x2);

struct s_saved_film_description
{
	long film_category;
	long campaign_id;
	long map_id;
	short campaign_insertion_point;
	bool campaign_survival_enabled;
	long difficulty;
	c_static_wchar_string<256> film_path;
	c_static_wchar_string<128> film_name;
	long controller_index;
	long length_seconds;
};
static_assert(sizeof(s_saved_film_description) == 0x31C);

struct c_network_session;
struct c_network_observer;
struct c_network_session_parameters
{
	struct s_network_session_parameters_internal
	{
		c_network_session_parameter_session_mode session_mode;
		c_network_session_parameter_session_size session_size;
		c_generic_network_session_parameter<s_network_session_privacy_mode> privacy_mode;
		c_network_session_parameter_ui_game_mode ui_game_mode;
		c_generic_network_session_parameter<long> dedicated_server_session_state;
		c_generic_network_session_parameter<s_network_session_parameter_map> map;
		c_network_session_parameter_initial_participants initial_participants;
		c_generic_network_session_parameter<qword> game_instance;
		c_generic_network_session_parameter<dword> random_seed;
		c_generic_network_session_parameter<e_language> language;
		c_generic_network_session_parameter<long> determinism_version;
		c_generic_network_session_parameter<e_network_game_simulation_protocol> game_simulation_protocol;
		c_generic_network_session_parameter<e_campaign_difficulty_level> campaign_difficulty;
		c_generic_network_session_parameter<c_flags<e_campaign_skulls_primary, dword, k_campaign_skulls_primary_count>> campaign_active_skulls_primary;
		c_generic_network_session_parameter<c_flags<e_campaign_skulls_secondary, dword, k_campaign_skulls_secondary_count>> campaign_active_skulls_secondary;
		c_generic_network_session_parameter<short> campaign_metagame_scoring;
		c_generic_network_session_parameter<short> campaign_insertion_point;
		c_generic_network_session_parameter<s_network_ui_state> ui_state;
		c_generic_network_session_parameter<bool> game_end;
		c_generic_network_session_parameter<e_network_game_start_mode> start_mode;
		c_network_session_parameter_game_variant game_variant;
		c_network_session_parameter_map_variant map_variant;
		c_generic_network_session_parameter<s_saved_film_description> saved_film;

		//c_network_session_parameter_saved_film_game_options saved_film_game_options;
		c_static_array<byte, 0x5A740> saved_film_game_options;

		c_network_session_parameter_game_start_status game_start_status;
		c_network_session_parameter_countdown_timer countdown_timer;

		byte __data[0x2010];
	};
	static_assert(sizeof(s_network_session_parameters_internal) == 0xB7858);

	s_network_session_parameters_internal m_parameters_internal;
	c_network_session* m_session;
	c_network_observer* m_observer;
	c_network_session_parameter_base* m_parameter_interfaces[49];
	dword_flags m_flags;
	dword m_initial_parameters_update_mask;
	long : 32;
};
static_assert(sizeof(c_network_session_parameters) == 0xB7930);
