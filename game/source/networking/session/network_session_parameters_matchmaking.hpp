#pragma once

#include "cseries/cseries.hpp"
#include "networking/session/network_session_parameters_generic.hpp"
#include "networking/logic/life_cycle/life_cycle_matchmaking_quality.hpp"
#include "networking/transport/transport_security.hpp"
#include "text/unicode.hpp"

struct s_game_hopper_custom_category
{
	uns16 category_identifier;
	int16 category_image_index;
	c_static_string<32> category_name;
};
COMPILE_ASSERT(sizeof(s_game_hopper_custom_category) == 0x24);

struct s_network_session_matchmaking_hopper_entry
{
	uns16 hopper_identifier;
	uns16 category_identifier;
	uns16 presence_identifier;
	int32 hopper_image_index;
	bool is_hopper_visible;
	bool is_hopper_playable;
	bool is_hopper_ranked;
	bool has_xp_penalty;
	c_static_string<32> hopper_name;
};
COMPILE_ASSERT(sizeof(s_network_session_matchmaking_hopper_entry) == 0x30);

struct s_network_session_matchmaking_hopper_list
{
	int32 hopper_load_status;
	int32 session_membership_version;
	uns32 visible_category_mask;
	int32 category_count;
	s_game_hopper_custom_category categories[4];
	int32 hopper_count;
	s_network_session_matchmaking_hopper_entry hoppers[32];
};
COMPILE_ASSERT(sizeof(s_network_session_matchmaking_hopper_list) == 0x6A4);

class c_network_session_parameter_matchmaking_hopper_list :
	public c_network_session_parameter_base,
	public c_generic_network_session_parameter_data<s_network_session_matchmaking_hopper_list>
{
};
COMPILE_ASSERT(sizeof(c_network_session_parameter_matchmaking_hopper_list) == 0xD78);

struct s_network_session_matchmaking_hopper
{
	uns16 hopper_identifier;
	int8 xlast_index;
	bool is_ranked;
	bool veto_allowed;
	bool rematch_allowed;
	bool team_game;
	int32 draw_probability;
	real32 beta;
	real32 tau;
	int32 experience_base_increment;
	int32 experience_penalty_decrement;
	int32 require_hosts_on_multiple_teams;
	int32 selection_time;
	int32 max_party_size;
	int32 rematch_timer;
	int32 repeated_opponents_to_consider_for_penalty;
	int32 repeated_opponents_experience_threshold;
	int32 repeated_opponents_skill_throttle_start;
	int32 repeated_opponents_skill_throttle_stop;
	int32 pre_match_voice_configuration;
	int32 in_match_voice_configuration;
	int32 post_match_voice_configuration;
	bool restrict_open_channel;
};
COMPILE_ASSERT(sizeof(s_network_session_matchmaking_hopper) == 0x4C);

class c_network_session_parameter_matchmaking_hopper :
	public c_network_session_parameter_base,
	public c_generic_network_session_parameter_data<s_network_session_matchmaking_hopper>
{
};
COMPILE_ASSERT(sizeof(c_network_session_parameter_matchmaking_hopper) == 0xC8);

struct s_replicated_life_cycle_matchmaking_progress_searching_for_match
{
	int32 search_stage;
	int32 searching_skill_min;
	int32 searching_skill_max;
	int32 contacted_session_count;
	int32 potential_session_count;
	int32 search_queries_submitted;
};
COMPILE_ASSERT(sizeof(s_replicated_life_cycle_matchmaking_progress_searching_for_match) == 0x18);

struct s_replicated_life_cycle_matchmaking_progress_assembling_match
{
	int32 give_up_timer_seconds;
	int32 give_up_timer_maximum_seconds;
	int32 qos_listener_data_replies_sent;
};
COMPILE_ASSERT(sizeof(s_replicated_life_cycle_matchmaking_progress_assembling_match) == 0xC);

enum e_matchmaking_configuring_match_state
{
	_matchmaking_configuring_match_loading = 0,
	_matchmaking_configuring_match_veto,
	_matchmaking_configuring_match_vetoed_loading,
	_matchmaking_configuring_match_countdown,
	_matchmaking_configuring_match_veto_failed_countdown,
	_matchmaking_configuring_match_vetoed_countdown,

	k_matchmaking_configuring_match_state_count
};

struct s_replicated_life_cycle_matchmaking_progress_configuring_match
{
	e_matchmaking_configuring_match_state state;
	int32 map_load_progress_percentage;
	int32 countdown_timer;
	int32 current_veto_count;
	int32 max_veto_count;
	int32 current_game_map_index;
	bool encountered_load_failure;
};
COMPILE_ASSERT(sizeof(s_replicated_life_cycle_matchmaking_progress_configuring_match) == 0x1C);

struct s_replicated_life_cycle_progress_post_match
{
	bool countdown_running;
	int32 countdown;
};
COMPILE_ASSERT(sizeof(s_replicated_life_cycle_progress_post_match) == 0x8);

struct s_replicated_life_cycle_matchmaking_progress
{
	e_life_cycle_matchmaking_progress_type progress_type;

	union
	{
		s_replicated_life_cycle_matchmaking_progress_searching_for_match searching_for_match;
		s_replicated_life_cycle_matchmaking_progress_assembling_match assembling_match;
		s_replicated_life_cycle_matchmaking_progress_configuring_match configuring_match;
		s_replicated_life_cycle_progress_post_match post_match;
	};
};
COMPILE_ASSERT(sizeof(s_replicated_life_cycle_matchmaking_progress) == 0x20);

struct s_replicated_life_cycle_matchmaking_progress_search_criteria
{
	int32 party_skill_range_min;
	int32 party_skill_range_max;
	int32 searching_skill_range_min;
	int32 searching_skill_range_max;
	int32 searching_party_size_min;
	int32 searching_party_size_max;
	bool search_party_big_and_mixed;
	int32 hopper_required_player_count;
	int32 hopper_preferred_player_count;
};
COMPILE_ASSERT(sizeof(s_replicated_life_cycle_matchmaking_progress_search_criteria) == 0x24);

struct __declspec(align(4)) s_life_cycle_matchmaking_progress_common
{
	wchar_t hopper_name[32];
	int32 current_squad_player_count;
	int32 current_group_player_count;
	bool hopper_is_ranked;
	bool hopper_is_team;
};
COMPILE_ASSERT(sizeof(s_life_cycle_matchmaking_progress_common) == 0x4C);

struct s_life_cycle_matchmaking_progress_searching_for_match
{
	e_matchmaking_search_preference search_preference;
	s_replicated_life_cycle_matchmaking_progress_search_criteria search_criteria;
	s_replicated_life_cycle_matchmaking_progress_searching_for_match searching_progress;
};
COMPILE_ASSERT(sizeof(s_life_cycle_matchmaking_progress_searching_for_match) == 0x40);

struct s_life_cycle_matchmaking_progress_assembling_match
{
	e_matchmaking_search_preference search_preference;
	s_replicated_life_cycle_matchmaking_progress_search_criteria search_criteria;
	s_replicated_life_cycle_matchmaking_progress_assembling_match assembling_progress;
};
COMPILE_ASSERT(sizeof(s_life_cycle_matchmaking_progress_assembling_match) == 0x34);

struct s_life_cycle_matchmaking_progress_setting_up_match
{
	e_matchmaking_search_preference search_preference;
	s_replicated_life_cycle_matchmaking_progress_search_criteria search_criteria;
};
COMPILE_ASSERT(sizeof(s_life_cycle_matchmaking_progress_setting_up_match) == 0x28);

struct s_life_cycle_matchmaking_progress_configuring_match
{
	s_replicated_life_cycle_matchmaking_progress_configuring_match configuring_progress;
	int32 group_local_player_veto_mask;
	e_map_id map_id;
	wchar_t map_variant_name[32];
	e_game_engine_type game_engine_index;
	wchar_t game_variant_name[32];
};
COMPILE_ASSERT(sizeof(s_life_cycle_matchmaking_progress_configuring_match) == 0xA8);

struct __declspec(align(4)) s_life_cycle_progress_post_match
{
	s_replicated_life_cycle_progress_post_match post_match_progress;
	bool rematch_allowed;
	bool rematch_available_to_squad_leader;
	uns32 group_player_can_rematch_mask;
	uns32 group_player_chosen_to_rematch_mask;
	uns32 group_player_in_rematching_party_mask;
	bool is_preparing_for_rematch;
};
COMPILE_ASSERT(sizeof(s_life_cycle_progress_post_match) == 0x1C);

struct s_life_cycle_matchmaking_progress
{
	e_life_cycle_matchmaking_progress_type progress_type;
	s_life_cycle_matchmaking_progress_common common;
	union
	{
		s_life_cycle_matchmaking_progress_searching_for_match searching;
		s_life_cycle_matchmaking_progress_assembling_match assembling;
		s_life_cycle_matchmaking_progress_setting_up_match setting_up_match;
		s_life_cycle_matchmaking_progress_configuring_match configuring_match;
		s_life_cycle_progress_post_match post_match;
	};
};
COMPILE_ASSERT(sizeof(s_life_cycle_matchmaking_progress) == 0xF8);

struct s_network_session_parameter_matchmaking_rematch_team_data
{
	int32 host_peer_index;
	uns32 rematch_peer_mask;
	bool join_data_valid;
	s_transport_session_description host_description;
};
COMPILE_ASSERT(sizeof(s_network_session_parameter_matchmaking_rematch_team_data) == 0x3C);

struct s_network_session_parameter_matchmaking_rematch_data
{
	uns32 team_rematch_mask;
	c_static_array<s_network_session_parameter_matchmaking_rematch_team_data, 16> teams;
};
COMPILE_ASSERT(sizeof(s_network_session_parameter_matchmaking_rematch_data) == 0x3C4);

class c_network_session_parameter_matchmaking_rematch_data :
	public c_network_session_parameter_base,
	public c_generic_network_session_parameter_data<s_network_session_parameter_matchmaking_rematch_data>
{
};
COMPILE_ASSERT(sizeof(c_network_session_parameter_matchmaking_rematch_data) == 0x7B8);

struct s_matchmaking_single_hopper_statistics
{
	int32 total_online_population;
	int32 current_hopper_population;
};
COMPILE_ASSERT(sizeof(s_matchmaking_single_hopper_statistics) == 0x8);

class c_network_session_parameter_matchmaking_hopper_statistics :
	public c_network_session_parameter_base,
	public c_generic_network_session_parameter_data<s_matchmaking_single_hopper_statistics>
{
};
COMPILE_ASSERT(sizeof(c_network_session_parameter_matchmaking_hopper_statistics) == 0x40);

struct s_network_session_parameter_matchmaking_messaging
{
	int32 type;
	int32 display_cookie;
	int32 player_sequence_number;
};
COMPILE_ASSERT(sizeof(s_network_session_parameter_matchmaking_messaging) == 0xC);

class c_network_session_parameter_matchmaking_messaging :
	public c_network_session_parameter_base,
	public c_generic_network_session_parameter_data<s_network_session_parameter_matchmaking_messaging>
{
};
COMPILE_ASSERT(sizeof(c_network_session_parameter_matchmaking_messaging) == 0x48);

