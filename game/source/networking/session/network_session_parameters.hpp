#pragma once

#include "cseries/language.hpp"
#include "game/game_progression.hpp"
#include "networking/logic/network_session_interface.hpp"
#include "networking/session/network_session_parameters_session.hpp"
#include "networking/session/network_session_parameters_game.hpp"
#include "networking/session/network_session_parameters_game_variant.hpp"
#include "networking/session/network_session_parameters_map_variant.hpp"
#include "networking/session/network_session_parameters_matchmaking.hpp"
#include "networking/session/network_session_parameters_saved_film_game_options.hpp"
#include "networking/session/network_session_parameters_ui.hpp"

struct s_network_session_parameter_map
{
	e_campaign_id campaign_id;
	e_map_id map_id;
	c_static_string<128> scenario_path;
	s_campaign_armaments campaign_armaments;
	s_campaign_game_progression campaign_progression;
	s_hub_progression hub_progression;
};
static_assert(sizeof(s_network_session_parameter_map) == 0x200);

enum e_network_ui_state_flags
{
	_network_ui_state_flag_game_setup_changing_bit = 0,

	k_network_ui_state_flags
};

struct s_network_ui_state
{
	c_flags<e_network_ui_state_flags, uns16, k_network_ui_state_flags> flags;
};
static_assert(sizeof(s_network_ui_state) == 0x2);

struct s_network_session_parameter_synchronous_out_of_sync
{
	bool out_of_sync;
	bool determinism_failure;
};
static_assert(sizeof(s_network_session_parameter_synchronous_out_of_sync) == 0x2);

struct s_network_session_parameter_request_campaign_quit
{
	int32 reason;
	int32 peer_index;
};
static_assert(sizeof(s_network_session_parameter_request_campaign_quit) == 0x8);

struct s_network_message_parameters_update;
class c_network_session;
class c_network_observer;
class c_network_session_parameters
{
public:
	struct s_network_session_parameters_internal
	{
		c_network_session_parameter_session_mode session_mode;
		c_network_session_parameter_session_size session_size;
		c_generic_network_session_parameter<s_network_session_privacy_mode> privacy_mode;
		c_network_session_parameter_ui_game_mode ui_game_mode;
		c_generic_network_session_parameter<int32> dedicated_server_session_state;
		c_generic_network_session_parameter<s_network_session_parameter_map> map;
		c_network_session_parameter_initial_participants initial_participants;
		c_generic_network_session_parameter<uns64> game_instance;
		c_generic_network_session_parameter<uns32> random_seed;
		c_generic_network_session_parameter<e_language> language;
		c_generic_network_session_parameter<int32> determinism_version;
		c_generic_network_session_parameter<e_network_game_simulation_protocol> game_simulation_protocol;
		c_generic_network_session_parameter<e_campaign_difficulty_level> campaign_difficulty;
		c_generic_network_session_parameter<c_flags<e_campaign_skulls_primary, uns32, k_campaign_skull_primary_count>> campaign_active_skulls_primary;
		c_generic_network_session_parameter<c_flags<e_campaign_skulls_secondary, uns32, k_campaign_skull_secondary_count>> campaign_active_skulls_secondary;
		c_generic_network_session_parameter<int16> campaign_metagame_scoring;
		c_generic_network_session_parameter<int16> campaign_insertion_point;
		c_generic_network_session_parameter<s_network_ui_state> ui_state;
		c_generic_network_session_parameter<bool> game_end;
		c_generic_network_session_parameter<e_network_game_start_mode> start_mode;
		c_network_session_parameter_game_variant game_variant;
		c_network_session_parameter_map_variant map_variant;
		c_generic_network_session_parameter<s_saved_film_description> saved_film;
		c_network_session_parameter_saved_film_game_options saved_film_game_options;
		c_network_session_parameter_game_start_status game_start_status;
		c_network_session_parameter_countdown_timer countdown_timer;
		c_generic_network_session_parameter<int32> voice_repeater;
		c_network_session_parameter_requested_remote_join_data requested_remote_join_data;
		c_network_session_parameter_remote_join_data remote_join_data;
		c_generic_network_session_parameter<uns64> matchmaking_arbitration_nonce;
		c_network_session_parameter_matchmaking_hopper_list matchmaking_hopper_list;
		c_generic_network_session_parameter<s_game_hopper_picked_game_collection> matchmaking_game_list;
		c_generic_network_session_parameter<int16> matchmaking_requested_hopper;
		c_network_session_parameter_matchmaking_hopper matchmaking_hopper;
		c_generic_network_session_parameter<bool> matchmaking_abort_requested;
		c_generic_network_session_parameter<e_matchmaking_search_preference> matchmaking_search_preference;
		c_generic_network_session_parameter<s_replicated_life_cycle_matchmaking_progress> matchmaking_progress;
		c_generic_network_session_parameter<s_replicated_life_cycle_matchmaking_progress_search_criteria> matchmaking_progress_search_criteria;
		c_generic_network_session_parameter<uns32> matchmaking_peer_evict_mask;
		c_network_session_parameter_matchmaking_rematch_data matchmaking_rematch_data;
		c_network_session_parameter_matchmaking_hopper_statistics matchmaking_hopper_statistics;
		c_generic_network_session_parameter<int32> matchmaking_tip;
		c_generic_network_session_parameter<bool> matchmaking_suppress_progress;
		c_generic_network_session_parameter<s_network_session_parameter_synchronous_out_of_sync> synchronous_out_of_sync;
		c_network_session_parameter_matchmaking_messaging matchmaking_messaging;
		c_generic_network_session_parameter<e_network_rough_quality> host_frame_quality;
		c_generic_network_session_parameter<s_network_session_parameter_request_campaign_quit> request_campaign_quit;
		c_generic_network_session_parameter<s_network_session_parameter_leader_properties> leader_properties;
		c_network_session_parameter_lobby_vote_set lobby_vote_set;
	};
	static_assert(sizeof(s_network_session_parameters_internal) == 0xB7858);

	bool handle_update(const s_network_message_parameters_update* update);
	bool parameters_transmitted_to_peers(uns64 parameter_mask) const;

	s_network_session_parameters_internal m_parameters;
	c_network_session* m_session;
	c_network_observer* m_observer;
	c_network_session_parameter_base* m_parameter_interfaces[49 /* k_network_session_parameter_type_count */];
	uns32 m_flags;
	uns32 m_initial_parameters_update_mask;
	int32 : 32;
};
static_assert(sizeof(c_network_session_parameters) == 0xB7930);

